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

/*
** This class is responsible for gathering data from the intarweb !
*/


#ifndef DATAFETCHER_H
#define DATAFETCHER_H

// QtCore
#include <QtCore/QHash>
#include <QtCore/QObject>
//#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QVariant>
// QtNetwork
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

// Forward declarations
class QUrl;

class DataFetcher  : public QObject
{
  Q_OBJECT

  public:
    static DataFetcher* instance();
    void getArtistEvents(const QString& artist);

  Q_SIGNALS:
    // SIG: getArtistEventsReady()
    // #1 : Reply converted to a QVarian object
    // #2 : Was the request successful ?
    // #3 : Error Message
    void getArtistEventsReady(QVariant, bool, QString);

  private Q_SLOTS:
    void requestFinished(QNetworkReply *);

  private:
    enum RequestType {
       ArtistEventsRequest
    };
    enum CustomAttribute {
       RequestTypeAttribute = int(QNetworkRequest::User),
       ArtistNameAttribute = int(QNetworkRequest::User+1)
    };

    DataFetcher(QObject *parent = 0);
    DataFetcher(const DataFetcher &);
    DataFetcher * operator=(DataFetcher const &);
    ~DataFetcher();

    void doRequest(const QUrl &url, RequestType requestType, const QString& artistName = "");

   // Test if we got all the necessary data before emitting signal
  // void emissionCheck(const int &showId);

    // Convert NetworkError code to QString
    QString errorCodeToText(QNetworkReply::NetworkError errorCode);

    void clearData(const QString& artistName);

    QNetworkAccessManager* m_nam;

    QHash<QString, QVariant> m_artistEventsHash;
    QHash<QString, QString>  m_artistEventsError;
};

#endif // DATAFETCHER_H
