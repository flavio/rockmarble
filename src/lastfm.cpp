#include "lastfm.h"

#include "datafetcher.h"
#include "dbmanager.h"
#include "event.h"

#include <qjson/parserrunnable.h>
#include <QtCore/QDebug>
#include <QtCore/QThreadPool>

Lastfm::Lastfm(QObject* parent)
  : QObject (parent) {
  // data fetcher
  m_df = DataFetcher::instance();
  connect (m_df, SIGNAL(getArtistEventsReady(QString,bool,QString)),
           this, SLOT(slotArtistEventsReady(QString,bool,QString)));
  connect (m_df, SIGNAL(getTopArtistsReady(QString,bool,QString)),
           this, SLOT(slotTopArtistsReady(QString,bool,QString)));
  connect (m_df, SIGNAL(getEventsNearLocationReady(QString,bool,QString)),
           this, SLOT (slotEventsNearLocationReady(QString,bool,QString)));
}

void Lastfm::getTopArtists(const QString &user) {
  m_df->getTopArtists(user);
}

void Lastfm::slotArtistEventsReady(QString data, bool successfull, QString errMsg) {
  if (successfull) {
    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
    parserRunnable->setData(data.toAscii());
    connect(parserRunnable, SIGNAL(parsingFinished(QVariant, bool, QString)),
            this, SLOT(slotArtistEventConverted(QVariant, bool, QString)));
    QThreadPool::globalInstance()->start(parserRunnable);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotArtistEventConverted(QVariant data, bool successfull, QString errMsg) {
  if (successfull) {
    DBManager* db = DBManager::instance();
    QVariantMap response = data.toMap();
    response = response["events"].toMap();
    QString artist = response["artist"].toString();

    if (artist.isEmpty()) {
      // last.fm changed their reply
      QVariantMap attr = response["@attr"].toMap();
      artist = attr["artist"].toString();
    }

    QVariantList events = response["event"].toList();

    foreach(QVariant event, events) {
      db->addEvent(Event(event));
    }
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotTopArtistsReady(QString data, bool successfull, QString errMsg) {
  if (successfull) {
    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
    parserRunnable->setData(data.toAscii());
    connect(parserRunnable, SIGNAL(parsingFinished(QVariant,bool,QString)),
            this, SLOT(slotTopArtistConverted(QVariant, bool, QString)));
    QThreadPool::globalInstance()->start(parserRunnable);
  } else {
    emit error(errMsg);
  }
}

void Lastfm::slotTopArtistConverted(QVariant data, bool successfull, QString errMsg) {
  if (successfull) {
    DBManager* db = DBManager::instance();
    QVariantMap response = data.toMap();
    if (!response.contains("error")) {
      QVariantMap topartists = response["topartists"].toMap();
      QVariantList artists = topartists["artist"].toList();

      foreach(QVariant entry, artists) {
        QVariantMap artist = entry.toMap();
        QString artistName = artist["name"].toString();
        db->addArtist(artistName, true);
        m_df->getArtistEvents(artistName);
      }
    }
    else {
      emit  error(response["message"].toString());
    }
  } else {
    emit error (errMsg);
  }
}

void Lastfm::slotEventsNearLocationReady(QString data, bool successfull, QString error) {
//  if (successfull) {
//    QJson::ParserRunnable* parserRunnable = new QJson::ParserRunnable();
//    parserRunnable->setData(data.toAscii());
//    connect(parserRunnable, SIGNAL(parsingFinished(QVariant,bool,QString)), this, SLOT(slotEventsNearLocationConverted(QVariant, bool, QString)));
//    QThreadPool::globalInstance()->start(parserRunnable);
//  } else {
//    showMessage(error, true);
//  }
}

void Lastfm::slotEventsNearLocationConverted(QVariant data, bool successfull, QString error) {
//  if (successfull) {
//    QVariantMap response = data.toMap();
//    if (!response.contains("error")) {
//      response = response["events"].toMap();
//      QString city = response["location"].toString();
//      int page = response["page"].toInt();
//      int totalPages = response["totalpages"].toInt();
//
//      if (response.contains("@attr"))
//      {
//        // last.fm changed the response
//        QVariantMap attr = response["@attr"].toMap();
//        city = attr["location"].toString();
//        page = attr["page"].toInt();
//        totalPages = attr["totalpages"].toInt();
//      } else {
//        city = response["location"].toString();
//        page = response["page"].toInt();
//        totalPages = response["totalpages"].toInt();
//      }
//
//      QVariantList events = response["event"].toList();
//
//      foreach(QVariant event, events) {
//        Event* e = new Event(event);
//        if (m_cities.contains(city, e))
//          delete e;
//        else
//          m_cities.insertMulti(city, e);
//      }
//
////      if (citiesList->findItems(city, Qt::MatchExactly).isEmpty())
////        new QListWidgetItem(city, citiesList);
//
//      if (page < totalPages){
//        m_df->getEventsNearLocation(city, ++page);
////        m_statusBar->showMessage(tr("More events near %1 to fetch (we are at page %2/%3)").arg(city).arg(page).arg(totalPages), 1200);
//
////        if ((citiesList->currentRow() != -1) && (citiesList->currentItem()->text().compare(city) == 0))
////          slotCurrentCityRowChanged(citiesList->currentRow());
//      }
////      else
////        m_statusBar->showMessage(tr("All event dates near %1 successfully retrieved").arg(city), 1200);
//    } else {
//      showMessage(response["message"].toString(), true);
//    }
//  } else {
//    showMessage(error, true);
//  }
}
