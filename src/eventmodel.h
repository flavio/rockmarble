/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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

#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>

class Event;

#define EventList QList<Event*>

class EventModel : public QAbstractTableModel
{
  public:
    EventModel(EventList& events, QObject* parent = 0);
    virtual ~EventModel();

    // artist
    // country
    // city
    // location
    // date
    enum Columns { HeadlinerColumn = 0, ArtistsColumn = 1, CountryColumn = 2,
                   CityColumn = 3, LocationColumn = 4, DateColumn = 5};

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;

    bool getCoordinates(const QModelIndex& index, qreal* latitude, qreal* longitude);

  private:
    EventList* m_events;
};

#endif // EVENTMODEL_H
