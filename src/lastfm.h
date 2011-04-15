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

#ifndef LASTFM_H
#define LASTFM_H

#include <QtCore/QObject>
#include <QtCore/QThreadPool>
#include <QtCore/QVariant>

#include "dbmanager.h"

class DataFetcher;

class StoreEvents : public QObject, public QRunnable
{
  Q_OBJECT
  public:
    StoreEvents(const DBManager::Storage& storage, const QVariant& data,
                QObject* parent = 0);

    void run();

  signals:
    void done();

  private:
    DBManager* m_db;
    QVariant m_data;
};

class Lastfm : public QObject
{
  Q_OBJECT
  public:
    Lastfm(const DBManager::Storage& storage, QObject* parent = 0);

    void getTopArtists(const QString& user);
    void getArtistImage(const QString& artist);
    void getEventsForArtist(const QString& artist);
    void getEventsNearLocation(const double& latitude, const double& longitude,
                               const int& distance = 0);

  private slots:
    void slotEventsForArtistReady(const QString&, bool, const QString&);
    void slotEventsForArtistConverted(QVariant, bool, QString);

    void slotArtistInfoReady(QString,bool,QString);
    void slotArtistInfoConverted(QVariant, bool, QString);

    void slotArtistImageReady(const QString& artist,
                              const QByteArray& data,
                              bool  error,
                              const QString& errMsg);

    void slotTopArtistsReady(QString,bool,QString);
    void slotTopArtistConverted(QVariant, bool, QString);

    void slotEventsNearLocationReady(QString,bool,QString);
    void slotEventsNearLocationConverted(QVariant, bool, QString);
    void slotAllEventsNearLocationStored();

  signals:
    void error(const QString& message);

private:
    DataFetcher* m_df;
    DBManager::Storage m_dbStorage;

    struct LocationQuery {
      double latitude;
      double longitude;
      int    distance;
      int    page;
    } m_locationQuery;

    void getEventsNearLocation(LocationQuery& query);
};

#endif // LASTFM_H
