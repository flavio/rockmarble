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

#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

#include "dbmanager.h"
#include "isqlquery.h"

class EventModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    enum Mode {STARRED, ARTIST_BY_COUNTRY, ARTIST_NEAR_LOCATION};
    explicit EventModel(const DBManager::Storage& storage,
                        const int artistID, const QString& country,
                        QObject *parent = 0);
    explicit EventModel(const DBManager::Storage& storage,
                        bool starred,
                        QObject *parent = 0);
    explicit EventModel(const DBManager::Storage& storage,
                        const int artistID,
                        QObject *parent = 0);
    int rowCount() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

  private slots:
    void slotEventCreated(int eventID);

  private:
    void setup();
    QList<int> getEventIDs();
    ISqlQuery getQuery();

    QList<int> m_eventIDs;
    DBManager::Storage m_dbStorage;
    Mode m_mode;
    int m_artistID;
    QString m_country;
    bool m_starred;
};

#endif // EVENTMODEL_H
