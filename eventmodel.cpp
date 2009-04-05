/* This file is part of rockmarble
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public License
  * along with this library; see the file COPYING.LIB.  If not, write to
  * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  * Boston, MA 02110-1301, USA.
  */

#include "eventmodel.h"
#include "event.h"
#include "location.h"

EventModel::EventModel(EventList& events, QObject* parent)
  : QAbstractTableModel(parent)
{
  m_events = new EventList(events);
}

EventModel::~EventModel()
{
  foreach(Event* event, *m_events)
    delete event;
  m_events->clear();
  delete m_events;
}

int EventModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid())
    return 0;
  else
    return m_events->size();
}

int EventModel::columnCount ( const QModelIndex & parent) const
{
  if (parent.isValid())
    return 0;
  else {
    // country
    // city
    // location
    // date
    return 4;
  }
}

QModelIndex EventModel::index ( int row, int column, const QModelIndex & parent) const
{
  if (parent.isValid())
    return QModelIndex();

  if (row > m_events->size())
    return QModelIndex();

  if (column > this->columnCount())
    return QModelIndex();

  return createIndex(row, column, m_events);
}

QVariant EventModel::data ( const QModelIndex & index, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (!index.isValid())
    return QVariant();

  EventList* events = static_cast<EventList*>(index.internalPointer());

  if (index.row() > events->size())
    return QVariant();

  Event* event = events->at(index.row());
  switch (index.column())
  {
    case 0:
      return event->location()->country();
    case 1:
      return event->location()->city();
    case 2:
      return QString("%1 - %2").arg(event->location()->name()).arg(event->location()->street());
    case 3:
      return event->date().toString();
    default:
      return QVariant();
  }
}

QVariant EventModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section)
    {
      case CountryColumn:
        return tr("Country");
      case CityColumn:
        return tr("City");
      case LocationColumn:
        return tr("Location");
      case DateColumn:
        return tr("Date");
      default:
        return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags EventModel::flags ( const QModelIndex & index ) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool EventModel::getCoordinates(const QModelIndex& index, qreal* latitude, qreal* longitude)
{
  if (!index.isValid())
    return false;

  EventList* events = static_cast<EventList*>(index.internalPointer());

  if (index.row() > events->size())
    return false;

  Event* event = events->at(index.row());

  *latitude = event->location()->latitude();
  *longitude = event->location()->longitude();

  return true;
}

