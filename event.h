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

#ifndef EVENT_H
#define EVENT_H

#include <QDate>
#include <QVariant>
#include <QTime>

class Location;

class Event
{
public:
    Event(const QVariant& data);
    ~Event();

    QString artist() { return m_artist; }
    QDate date() { return m_date; }
    Location* location() { return m_location; }

    private:
      int m_id;
      QString m_artist;
      QString m_headliner;
      QString m_title;
      QString m_description;
      QDate m_date;
      QTime m_time;

      Location* m_location;
};

#endif // EVENT_H
