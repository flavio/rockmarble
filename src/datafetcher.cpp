/*
** Copyright (C) 2008 Gilles Chauvin <gcnweb+nextshows@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2  of  the  License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be  useful, but
** WITHOUT  ANY  WARRANTY;  without  even  the   implied   warranty  of
** MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  GNU
** General Public License for more details.
**
** You should have received a copy of the GNU  General  Public  License
** along with  this  program;  if  not,  write  to  the  Free  Software
** Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#include "datafetcher.h"
// QtCore
#include <QtCore/QDebug>
#include <QtCore/QMetaEnum>
#include <QtCore/QUrl>
// QtGui
#include <QtGui/QApplication>

#include "defines.h"

DataFetcher::DataFetcher(QObject *parent)
  : QObject(parent),
  m_nam(new QNetworkAccessManager(this))
{
  qDebug() << Q_FUNC_INFO;
  connect(m_nam, SIGNAL(finished(QNetworkReply *)),
          this, SLOT(requestFinished(QNetworkReply *)));
} // ctor()

DataFetcher::~DataFetcher()
{
  qDebug() << Q_FUNC_INFO;
  delete m_nam;
} // dtor()


DataFetcher* DataFetcher::instance()
{
  qDebug() << Q_FUNC_INFO;
  static DataFetcher * dataFetcherInstance = 0;
  if (!dataFetcherInstance) {
    dataFetcherInstance = new DataFetcher(qApp);
  }

  return dataFetcherInstance;
}

void DataFetcher::getArtistImage(const QString &artist, const QString &url)
{
  qDebug() << Q_FUNC_INFO;

  QUrl urlEL(url);
  doRequest(urlEL, DataFetcher::ArtistImageRequest, artist);
}

void DataFetcher::getArtistEvents(const QString& artist)
{
  qDebug() << Q_FUNC_INFO;

  QUrl urlEL(QString("http://ws.audioscrobbler.com/2.0/?method=artist.getevents&api_key=%1&format=json").arg(API_KEY));
  urlEL.addQueryItem("artist", artist);
  doRequest(urlEL, DataFetcher::ArtistEventsRequest, artist);
}

void DataFetcher::getArtistInfo(const QString& artist)
{
  qDebug() << Q_FUNC_INFO;

  QUrl urlEL(QString("http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&api_key=%1&format=json").arg(API_KEY));
  urlEL.addQueryItem("artist", artist);
  doRequest(urlEL, DataFetcher::ArtistInfoRequest, artist);
}

void DataFetcher::getTopArtists(const QString& user)
{
  qDebug() << Q_FUNC_INFO;

  QUrl urlEL(QString("http://ws.audioscrobbler.com/2.0/?method=user.gettopartists&api_key=%1&format=json").arg(API_KEY));
  urlEL.addQueryItem("user", user);
  urlEL.addQueryItem("period", "overall");
  doRequest(urlEL, DataFetcher::TopArtistsRequest, user);
}

void DataFetcher::getEventsNearLocation(const QString& location, const int page)
{
  qDebug() << Q_FUNC_INFO;

  QUrl urlEL(QString("http://ws.audioscrobbler.com/2.0/?method=geo.getevents&api_key=%1&format=json").arg(API_KEY));
  urlEL.addQueryItem("location", location);
  if (page != -1) {
    QString pageParam;
    pageParam.setNum(page);
    urlEL.addQueryItem("page", pageParam);
  }
  doRequest(urlEL, DataFetcher::EventsNearLocationRequest, location);
}

void DataFetcher::doRequest(const QUrl &url,
                            DataFetcher::RequestType requestType,
                            const QString &string)
{
  qDebug() << Q_FUNC_INFO;
  qDebug() << "Going to call url" << url;
  QNetworkRequest request;
  QString httpUA = QString("rockmarble/%1 "
                           "(http://github.com/flavio/rockmarble/tree/master)")\
                          .arg(ROCKMARBLE_VERSION);
  request.setRawHeader("User-Agent", qPrintable(httpUA));
  request.setUrl(url);

  // Set some attributes
  // Request Type
  request.setAttribute(QNetworkRequest::Attribute(RequestTypeAttribute),
                       QVariant((int)requestType));

  switch(requestType) {
    case EventsNearLocationRequest:
      request.setAttribute(QNetworkRequest::Attribute(EventsNearLocationAttribute),
                           QVariant(string));
      break;
    case TopArtistsRequest:
      request.setAttribute(QNetworkRequest::Attribute(UserNameAttribute),
                           QVariant(string));
      break;
    case ArtistEventsRequest:
    case ArtistInfoRequest:
    case ArtistImageRequest:
      request.setAttribute(QNetworkRequest::Attribute(ArtistNameAttribute),
                           QVariant(string));
      break;
  }
  m_nam->get(request);
} // doRequest()

void DataFetcher::requestFinished(QNetworkReply *reply)
{
  qDebug() << Q_FUNC_INFO;
  DataFetcher::RequestType requestType = DataFetcher::RequestType(reply->request().attribute(QNetworkRequest::Attribute(RequestTypeAttribute)).toInt());

  switch(requestType) {
  case DataFetcher::ArtistImageRequest:
    {
      QString artistName = reply->request().attribute(QNetworkRequest::Attribute(ArtistNameAttribute)).toString();
      if (reply->error() == QNetworkReply::NoError) {
        QString errorMessage;
        QByteArray response (reply->readAll());
        emit getArtistImageReady(artistName, response, true, "");
      } else {
        QString errorText = QString("A network error occured while fetching "
                                    "raw data [%1]!").\
                                    arg(errorCodeToText(reply->error()));
        qCritical() << errorText;
        qCritical() << "URL requested:" << reply->request().url().toString();
        qCritical() << "URL processed:" << reply->url().toString();
        emit getArtistImageReady(artistName, "", false, errorText);
      }
      break;
    }
  case DataFetcher::ArtistEventsRequest:
    {
      QString artistName = reply->request().attribute(QNetworkRequest::Attribute(ArtistNameAttribute)).toString();
      if (reply->error() == QNetworkReply::NoError) {
        QString errorMessage;
        QString response (reply->readAll());
        emit getEventsForArtistReady(response, true, "");
      } else {
        QString errorText = QString("A network error occured while searching for artist \"%1\" [%2]!").arg(artistName).arg(errorCodeToText(reply->error()));
        qCritical() << errorText;
        qCritical() << "URL requested:" << reply->request().url().toString();
        qCritical() << "URL processed:" << reply->url().toString();
        emit getEventsForArtistReady("", false, errorText);
      }
      break;
    }
  case DataFetcher::ArtistInfoRequest:
    {
      QString artistName = reply->request().attribute(QNetworkRequest::Attribute(ArtistNameAttribute)).toString();
      if (reply->error() == QNetworkReply::NoError) {
        QString errorMessage;
        QString response (reply->readAll());
        emit getArtistInfoReady(response, true, "");
      } else {
        QString errorText = QString("A network error occured while fetching artist details \"%1\" [%2]!").arg(artistName).arg(errorCodeToText(reply->error()));
        qCritical() << errorText;
        qCritical() << "URL requested:" << reply->request().url().toString();
        qCritical() << "URL processed:" << reply->url().toString();
        emit getArtistInfoReady("", false, errorText);
      }
      break;
    }
  case DataFetcher::TopArtistsRequest:
    {
      QString userName = reply->request().attribute(QNetworkRequest::Attribute(UserNameAttribute)).toString();
      if (reply->error() == QNetworkReply::NoError) {
        QString errorMessage;
        QString response (reply->readAll());
        emit getTopArtistsReady(response, true, "");
      } else {
        QString errorText = QString("A network error occured while searching for top artists of user \"%1\" [%2]!").arg(userName).arg(errorCodeToText(reply->error()));
        qCritical() << errorText;
        qCritical() << "URL requested:" << reply->request().url().toString();
        qCritical() << "URL processed:" << reply->url().toString();
        emit getTopArtistsReady("", false, errorText);
      }
      break;
    }
  case DataFetcher::EventsNearLocationRequest:
    {
      QString locationName = reply->request().attribute(QNetworkRequest::Attribute(EventsNearLocationAttribute)).toString();
      if (reply->error() == QNetworkReply::NoError) {
        QString errorMessage;
        QString response (reply->readAll());
        emit getEventsNearLocationReady(response, true, "");
      } else {
        QString errorText = QString("A network error occured while searching for events near \"%1\" [%2]!").arg(locationName).arg(errorCodeToText(reply->error()));
        qCritical() << errorText;
        qCritical() << "URL requested:" << reply->request().url().toString();
        qCritical() << "URL processed:" << reply->url().toString();
        emit getEventsNearLocationReady("", false, errorText);
      }
      break;
    }
  default:
    qWarning("%s\nThis should never happen [%d]!!!", Q_FUNC_INFO, requestType);
  }

  reply->deleteLater();
} // requestFinished()

QString DataFetcher::errorCodeToText(QNetworkReply::NetworkError errorCode)
{
  qDebug() << Q_FUNC_INFO;
  QString errorName;
  QMetaObject meta = QNetworkReply::staticMetaObject;
  for (int i=0; i < meta.enumeratorCount(); ++i) {
    QMetaEnum m = meta.enumerator(i);
    if (m.name() == QLatin1String("NetworkError")) {
      errorName = QLatin1String(m.valueToKey(errorCode));
      break;
    }
  }

  return errorName;
} // errorCodeToText()
