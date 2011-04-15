/* This file is part of rockmarble
  *
  * Copyright (C) 2011 Flavio Castelli <flavio@castelli.name>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2  of  the  License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be  useful, but
  * WITHOUT  ANY  WARRANTY;  without  even  the   implied   warranty  of
  * MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  GNU
  * General Public License for more details.
  *
  * You should have received a copy of the GNU  General  Public  License
  * along with  this  program;  if  not,  write  to  the  Free  Software
  * Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
  */

#include "lastfm.h"

#include "datafetcher.h"
#include "event.h"

#include <qjson/parserrunnable.h>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>

StoreEvents::StoreEvents(const DBManager::Storage& storage,
                         const QVariant& data, QObject* parent)
  : QObject(parent)
{
  m_data = data;
  m_db = DBManager::instance(storage);
}

void StoreEvents::run()
{
  int artistID = -1;
  QVariantMap response = m_data.toMap();
  response = response["events"].toMap();
  QString artist = response["artist"].toString();

  if (artist.isEmpty()) {
    // last.fm changed their reply
    QVariantMap attr = response["@attr"].toMap();
    artist = attr["artist"].toString();
  }

  if (!artist.isEmpty())
    artistID = m_db->addArtist(artist, true);

  QVariantList events = response["event"].toList();

  foreach(QVariant event, events)
    m_db->addEvent(Event(event));

  if (artistID != -1)
    m_db->setArtistAllDataFetched(artistID, true);
  emit done();
}

class WriteArtistImage : public QRunnable
{
  public:
    WriteArtistImage(const DBManager::Storage& storage,
                     const QString& artist, const QByteArray& data) {
      m_db = DBManager::instance(storage);
      m_data = data;
      m_artist = artist;
    }

    void run() {
      QDir dir;
      QString imagesDir = QDir::homePath() + "/.rockmarble/images/";
      if (!dir.exists(imagesDir))
        dir.mkpath(imagesDir);

      int artistID = m_db->addArtist(m_artist);
      QFile file (QString("%1/%2").arg(imagesDir).arg(artistID));
      if (file.open(QFile::WriteOnly)) {
        file.write(m_data);
        file.close();
        m_db->setArtistHasImage(artistID, true);
      }
    }

  private:
    DBManager* m_db;
    QString m_artist;
    QByteArray m_data;
};

class StoreArtist : public QRunnable
{
  public:
    StoreArtist(const DBManager::Storage& storage, const QVariantMap& response)
    {
      m_db = DBManager::instance(storage);
      m_response = response;
    }

    void run() {
      QVariantMap topartists = m_response["topartists"].toMap();
      QVariantList artists = topartists["artist"].toList();

      foreach(QVariant entry, artists) {
        QVariantMap artist = entry.toMap();
        QString artistName = artist["name"].toString();
        m_db->addArtist(artistName, true);
      }
    }

  private:
    DBManager* m_db;
    QVariantMap m_response;
};

Lastfm::Lastfm(const DBManager::Storage& storage, QObject* parent)
  : QObject (parent), m_dbStorage(storage)
{
  // data fetcher
  m_df = DataFetcher::instance();
  connect (m_df, SIGNAL(getEventsForArtistReady(const QString&,
                                             bool, const QString&)),
           this, SLOT(slotEventsForArtistReady(const QString&,
                                               bool, const QString&)));
  connect (m_df, SIGNAL(getArtistInfoReady(QString,bool,QString)),
           this, SLOT(slotArtistInfoReady(QString,bool,QString)));
  connect (m_df, SIGNAL(getTopArtistsReady(QString,bool,QString)),
           this, SLOT(slotTopArtistsReady(QString,bool,QString)));
  connect (m_df, SIGNAL(getArtistImageReady(QString,QByteArray,bool,QString)),
           this, SLOT (slotArtistImageReady(QString,QByteArray,bool,QString)));
  connect (m_df, SIGNAL(getEventsNearLocationReady(QString,bool,QString)),
           this, SLOT (slotEventsNearLocationReady(QString,bool,QString)));
}

void Lastfm::getTopArtists(const QString &user) {
  m_df->getTopArtists(user);
}

void Lastfm::getEventsForArtist(const QString &artist) {
  m_df->getArtistEvents(artist);
}

void Lastfm::getArtistImage(const QString &artist) {
  m_df->getArtistInfo(artist);
}

void Lastfm::getEventsNearLocation(const double& latitude,
                                   const double& longitude,
                                   const int& distance)
{
  m_locationQuery.distance  = distance;
  m_locationQuery.latitude  = latitude;
  m_locationQuery.longitude = longitude;
  m_locationQuery.page      = 0;
  m_df->getEventsNearLocation(latitude, longitude, distance);
}

void Lastfm::getEventsNearLocation(LocationQuery& query)
{
  m_locationQuery = query;
  m_df->getEventsNearLocation(m_locationQuery.latitude,
                              m_locationQuery.longitude,
                              m_locationQuery.distance,
                              m_locationQuery.page);
}

void Lastfm::slotEventsForArtistReady(const QString& data, bool successful,
                                      const QString& errMsg) {
  if (successful) {
    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
    parserRunnable->setData(data.toAscii());
    connect(parserRunnable, SIGNAL(parsingFinished(QVariant, bool, QString)),
            this, SLOT(slotEventsForArtistConverted(QVariant, bool, QString)));
    QThreadPool::globalInstance()->start(parserRunnable);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotEventsForArtistConverted(QVariant data, bool successful, QString errMsg) {
  if (successful) {
    StoreEvents* storeEvents = new StoreEvents(m_dbStorage, data);
    QThreadPool::globalInstance()->start(storeEvents);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotArtistInfoReady(QString data, bool successful, QString errMsg) {
  if (successful) {
    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
    parserRunnable->setData(data.toAscii());
    connect(parserRunnable, SIGNAL(parsingFinished(QVariant, bool, QString)),
            this, SLOT(slotArtistInfoConverted(QVariant, bool, QString)));
    QThreadPool::globalInstance()->start(parserRunnable);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotArtistInfoConverted(QVariant data, bool successful, QString errMsg) {
  if (successful) {
    QString url;
    QVariantMap artist = data.toMap().value("artist").toMap();
    QString artistName = artist.value("name").toString();
    QVariantList images = artist.value("image").toList();
    foreach (QVariant image, images) {
      if (image.toMap().value("size") == "medium") {
        url = image.toMap().value("#text").toString();
        break;
      }
    }
    if (!url.isEmpty())
      m_df->getArtistImage(artistName, url);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotArtistImageReady(const QString& artist, const QByteArray& data,
                                  bool successful, const QString& errMsg)
{
  if (successful) {
    WriteArtistImage* writer = new WriteArtistImage(m_dbStorage, artist, data);
    QThreadPool::globalInstance()->start(writer);
    getArtistImage(artist);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotTopArtistsReady(QString data, bool successful, QString errMsg){
  if (successful) {
    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
    parserRunnable->setData(data.toAscii());
    connect(parserRunnable, SIGNAL(parsingFinished(QVariant,bool,QString)),
            this, SLOT(slotTopArtistConverted(QVariant, bool, QString)));
    QThreadPool::globalInstance()->start(parserRunnable);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotTopArtistConverted(QVariant data, bool successful,
                                    QString errMsg) {
  if (successful) {
    QVariantMap response = data.toMap();
    if (!response.contains("error")) {
      StoreArtist* storeArtist = new StoreArtist(m_dbStorage, response);
      QThreadPool::globalInstance()->start(storeArtist);
    }
    else {
      emit error(response["message"].toString());
    }
  } else {
    emit error (errMsg);
  }
}

void Lastfm::slotEventsNearLocationReady(QString data, bool successful,
                                         QString errMsg) {
  if (successful) {
    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
    parserRunnable->setData(data.toAscii());
    connect(parserRunnable, SIGNAL(parsingFinished(QVariant,bool,QString)), this, SLOT(slotEventsNearLocationConverted(QVariant, bool, QString)));
    QThreadPool::globalInstance()->start(parserRunnable);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotEventsNearLocationConverted(QVariant data, bool successful,
                                             QString errMsg) {
  if (successful) {
    QVariantMap response = data.toMap();
    if (!response.contains("error")) {
      StoreEvents* storeEvents = new StoreEvents(DBManager::MEMORY, response);
      QThreadPool::globalInstance()->start(storeEvents);

      int page, totalPages;
      if (response.contains("@attr")) {
        // last.fm changed the response
        QVariantMap attr = response["@attr"].toMap();
        page = attr["page"].toInt();
        totalPages = attr["totalpages"].toInt();
      } else {
        page = response["page"].toInt();
        totalPages = response["totalpages"].toInt();
      }

      if (page < totalPages){
        // there are still events to download
        m_locationQuery.page++;
        this->getEventsNearLocation(m_locationQuery);
      } else {
        // all events have been downloaded
        connect (storeEvents, SIGNAL(done()),
                 this, SLOT(slotAllEventsNearLocationStored()));
      }
    }
    else {
      emit error(response["message"].toString());
    }
  } else {
    emit error (errMsg);
  }
}

void Lastfm::slotAllEventsNearLocationStored()
{
  DBManager::instance(m_dbStorage)->setAllArtistHaveAllData();
}
