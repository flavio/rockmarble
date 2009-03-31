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

#include <json_driver.hh>

#define API_KEY "b25b959554ed76058ac220b7b2e0a026"

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

void DataFetcher::getArtistEvents(const QString& artist)
{
  qDebug() << Q_FUNC_INFO;

  // Cleanup
  clearArtistData(artist);

  QUrl urlEL(QString("http://ws.audioscrobbler.com/2.0/?method=artist.getevents&api_key=%1&format=json").arg(API_KEY));
  urlEL.addQueryItem("artist", artist);
  doRequest(urlEL, DataFetcher::ArtistEventsRequest, artist);
}

void DataFetcher::getTopArtists(const QString& user)
{
  qDebug() << Q_FUNC_INFO;

  // Cleanup
  clearUserData(user);

  QUrl urlEL(QString("http://ws.audioscrobbler.com/2.0/?method=user.gettopartists&api_key=%1&format=json").arg(API_KEY));
  urlEL.addQueryItem("user", user);
  urlEL.addQueryItem("period", "overall");
  doRequest(urlEL, DataFetcher::TopArtistsRequest, user);
  }

void DataFetcher::doRequest(const QUrl &url, DataFetcher::RequestType requestType, const QString &artistName)
{
  qDebug() << Q_FUNC_INFO;
  QNetworkRequest request;
  //QString httpUA = QString("nextShows/%1 (http://nextshows.googlecode.com/)").arg(NEXTSHOWS_VERSION);
  //    request.setRawHeader("User-Agent", qPrintable(httpUA));
  request.setUrl(url);

  // Set some attributes
  // Request Type
  request.setAttribute(QNetworkRequest::Attribute(RequestTypeAttribute), QVariant((int)requestType));
  // Artist name
  request.setAttribute(QNetworkRequest::Attribute(ArtistNameAttribute), QVariant(artistName));

  m_nam->get(request);
} // doRequest()

void DataFetcher::requestFinished(QNetworkReply *reply)
{
  qDebug() << Q_FUNC_INFO;
  DataFetcher::RequestType requestType = DataFetcher::RequestType(reply->request().attribute(QNetworkRequest::Attribute(RequestTypeAttribute)).toInt());

  switch(requestType) {
  case DataFetcher::ArtistEventsRequest:
    {
      QString artistName = reply->request().attribute(QNetworkRequest::Attribute(ArtistNameAttribute)).toString();
      if (reply->error() == QNetworkReply::NoError) {
        QString errorMessage;
        QString response (reply->readAll());
        emit getArtistEventsReady(response, true, "");
      } else {
        QString errorText = QString("A network error occured while searching for artist \"%1\" [%2]!").arg(artistName).arg(errorCodeToText(reply->error()));
        qCritical() << errorText;
        qCritical() << "URL requested:" << reply->request().url().toString();
        qCritical() << "URL processed:" << reply->url().toString();
        emit getArtistEventsReady("", false, errorText);
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

void DataFetcher::clearArtistData(const QString& artistName)
{
  qDebug() << Q_FUNC_INFO;
  m_artistEventsHash.remove(artistName);
  m_artistEventsError.remove(artistName);
}

void DataFetcher::clearUserData(const QString& userName)
{
  qDebug() << Q_FUNC_INFO;
  m_topArtistsHash.remove(userName);
  m_topArtistsError.remove(userName);
}
