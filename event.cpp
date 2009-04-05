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

#include "event.h"
#include "location.h"

Event::Event(const QVariant& data)
{
  QVariantMap event = data.toMap();

  m_id = event["id"].toInt();

  QVariantMap artists = event["artists"].toMap();
  if (artists["artist"].canConvert<QStringList>())
    m_artists = artists["artist"].toStringList();
  else
    m_artists << artists["artist"].toString();
  m_headliner = artists["headliner"].toString();

  m_title = event["title"].toString();
  m_date = QDate::fromString(event["startDate"].toString(), "ddd, dd MMM yyyy");
  m_time = QTime::fromString(event["startTime"].toString(), "hh:mm");

//  qDebug() << "start date" << event["startDate"] << "-->" << m_date;
//  qDebug() << "start time" << event["startTime"] << "-->" << m_time;

  m_location = new Location (event["venue"]);
}

Event::~Event() {
  delete m_location;
  m_location = 0;
}

