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

#ifndef ARTIST_H
#define ARTIST_H

#include <QString>
#include <QVariant>
#include <QVector>

class Event;

class Artist
{
  public:
    Artist(const QVariant& data);
    ~Artist();

    QString name() { return m_name; }
    QVector<Event*> events() { return m_events; }

  private:
    QString m_name;
    QVector<Event*> m_events;
};

#endif // ARTIST_H
