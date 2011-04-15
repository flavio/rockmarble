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

#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>

#include "event.h"

class Location;
class ISqlQuery;

class DBManager : public QObject
{
  Q_OBJECT
  public:
    enum Storage {DISK, MEMORY};

    static DBManager* instance(const Storage& storage);
    static bool removeDatabase(const Storage& storage);
    static const QString databaseFile(const Storage& storage);

    int addArtist(const QString& name, bool favourite = false);
    void setArtistHasImage(const int& artistID, bool hasImage);
    void setArtistAllDataFetched(const int& artistID, bool done);
    void setAllArtistHaveAllData();
    void addEvent(const Event& event);
    void removeOldEvents(const QDate& upTo);
    void setEventStarred(const int& eventID, const bool& starred);
    int eventsWithArtistNum(const int& artistID);
    int eventsWithArtistInCountryNum(const int& artistID,
                                     const QString& country);
    QString artistNameFromID(const int& artistID);
    int artistIDFromName(const QString& artistName);
    bool artistHasImage(const int& artistID);
    bool artistHasAllData(const int& artistID);
    bool artistsPlaysInLocation(const int& artistID, const int& locationID);
    const QStringList incompleteArtists();
    const QStringList artistsWithoutImage();
    const QStringList artists(bool favourite);

    bool isArtistFavourite(const int& artistID);
    bool isArtistFavourite(const QString& artistName);

    Event* eventFromID(const int& eventID);
    Location* locationFromID(const int& locationID);

    QSqlDatabase database();
    bool isDatabaseEmpty();
    void cleanTables();

  signals:
    void artistAdded(const int artistID, bool favourite);
    void artistUpdated(const int artistID);
    void artistAddedToEvent(const int artistID, const int eventID);
    void locationCreated(const int locationID);
    void eventCreated(const int eventID);

  private:
    DBManager(const Storage& storage, QObject* parent = 0);
    DBManager(const DBManager&); //hide copy constructor
    DBManager& operator=(const DBManager&); //hide assignment operator

    void initDB(const Storage& storage);
    bool executeQuery(const QString&);
    bool executeQuery(const QString&, QSqlDatabase& db);
    bool executeQuery(ISqlQuery*);

    void addArtistToEvent(const QString& artist, const int& eventID);
    int addLocation(const Location* location);
    QStringList artistsFromEvent(const int& eventID);

    static DBManager* m_mainDatabase;
    static DBManager* m_currentLocationDatabase;

    Storage m_storage;
};

#endif // DBMANAGER_H
