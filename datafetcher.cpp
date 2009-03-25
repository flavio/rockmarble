s/*
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


// Own
#include "datafetcher.h"
// QtCore
#include <QtCore/QDebug>
#include <QtCore/QMetaEnum>
#include <QtCore/QVariant>
#include <QtCore/QUrl>
// QtGui
#include <QtGui/QApplication>


/*
** public:
*/
DataFetcher* DataFetcher::instance()
{
    qDebug() << Q_FUNC_INFO;
    static DataFetcher * dataFetcherInstance = 0;
    if (!dataFetcherInstance) {
        dataFetcherInstance = new DataFetcher(qApp);
    }

    return dataFetcherInstance;
} // instance()

void DataFetcher::getArtistEvents(const QString& artist)
{
    qDebug() << Q_FUNC_INFO;

    // Cleanup
    clearShowData(showId);

    QUrl urlEL("http://ws.audioscrobbler.com/2.0/?method=artist.getevents&api_key=b25b959554ed76058ac220b7b2e0a026");
    urlEL.addQueryItem("artist", artist);
    doRequest(urlEL, DataFetcher::ArtistEventsRequest, artist);
}

/*
** private Q_SLOTS:
*/
void DataFetcher::requestFinished(QNetworkReply *reply)
{
    qDebug() << Q_FUNC_INFO;
    DataFetcher::RequestType requestType = DataFetcher::RequestType(reply->request().attribute(QNetworkRequest::Attribute(RequestTypeAttribute)).toInt());
    QString artistName = reply->request().attribute(QNetworkRequest::Attribute(ArtistNameAttribute)).toString();

    switch(requestType) {
    case DataFetcher::ArtistEventsRequest: {
        if (reply->error() == QNetworkReply::NoError) {
            bool success;
            QString errorMessage;
            int errorLine;
            int errorColumn;
            QString response (reply->readAll());
            success = true;
            if (success) {
                emit getArtistEventsReady(QVariant(response), true, QString());
            } else {
                QString errorText = QString("An error occured while parsing XML document for show \"%1\"!").arg(showName);
                qCritical() << errorText;
                qCritical() << "URL requested:" << reply->request().url().toString();
                qCritical() << "URL processed:" << reply->url().toString();
                //qCritical("%s [L:%d-C:%d]", qPrintable(errorMessage), errorLine, errorColumn);
                emit searchResultsReady(QVariant(), false, errorText);
            }
        } else {
            QString errorText = QString("A network error occured while searching for show \"%1\" [%2]!").arg(showName).arg(errorCodeToText(reply->error()));
            qCritical() << errorText;
            qCritical() << "URL requested:" << reply->request().url().toString();
            qCritical() << "URL processed:" << reply->url().toString();
            emit searchResultsReady(QVariant(), false, errorText);
        }
        break;
    }
    default:
        qWarning("%s\nThis should never happen [%d]!!!", Q_FUNC_INFO, requestType);
    }

    reply->deleteLater();
} // requestFinished()


/*
** private:
*/
DataFetcher::DataFetcher(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
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

void DataFetcher::doRequest(const QUrl &url, DataFetcher::RequestType requestType, const QString &artistName)
{
    qDebug() << Q_FUNC_INFO;
    QNetworkRequest request;
//    QString httpUA = QString("nextShows/%1 (http://nextshows.googlecode.com/)").arg(NEXTSHOWS_VERSION);
//    request.setRawHeader("User-Agent", qPrintable(httpUA));
    request.setUrl(url);

    // Set some attributes
    // Request Type
    request.setAttribute(QNetworkRequest::Attribute(RequestTypeAttribute), QVariant((int)requestType));
    // Artist name
    request.setAttribute(QNetworkRequest::Attribute(ArtistNameAttribute), QVariant(artistName));

    m_nam->get(request);
} // doRequest()

//void DataFetcher::emissionCheck(const int &showId)
//{
//    qDebug() << Q_FUNC_INFO;
//    // Ensure both requests (showinfo + episodelist) were made for this showId
//    int count = 0;
//    count = (m_showInfosHash.contains(showId)   || m_showInfosError.contains(showId))   ? count + 1 : count;
//    count = (m_episodeListHash.contains(showId) || m_episodeListError.contains(showId)) ? count + 1 : count;
//    if (count != 2) {
//        return;
//    }
//
//    // Are all the required data available ?
//    if (m_showInfosHash.contains(showId) && m_episodeListHash.contains(showId)) {
//        emit episodeListReady(m_showInfosHash.value(showId), m_episodeListHash.value(showId), true, QString());
//    } else {
//        // If we reach here, we assume there were at least one error at some point
//        QString errorText;
//        if (m_showInfosError.contains(showId)) {
//            errorText = m_showInfosError.value(showId);
//        }
//        else if (m_episodeListError.contains(showId)) {
//            errorText = m_episodeListError.value(showId);
//        }
//        else {
//            qWarning("%s\nThis should never happen!!!", Q_FUNC_INFO);
//        }
//        emit episodeListReady(NextShows::ShowInfos_t(), NextShows::EpisodeListList(), false, errorText);
//    }
//
//    clearShowData(showId); // Cleanup
//} // emissionCheck()

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

//void DataFetcher::clearShowData(const int &showId)
//{
//    qDebug() << Q_FUNC_INFO;
//    m_showInfosHash.remove(showId);
//    m_showInfosError.remove(showId);
//    m_episodeListHash.remove(showId);
//    m_episodeListError.remove(showId);
//} // clearShowData()

// EOF - vim:ts=4:sw=4:et:


