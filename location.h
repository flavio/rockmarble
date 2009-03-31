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

#ifndef LOCATION_H
#define LOCATION_H

#include <QVariant>

class Location
{
  public:
    Location(const QVariant& data);

    QString name() { return m_name; }
    QString city() { return m_city; }
    QString country() { return m_country; }
    QString street() { return m_street; }
    qreal latitude() { return m_lat; }
    qreal longitude() { return m_long; }

    private:
      QString m_name;
      QString m_city;
      QString m_country;
      QString m_street;
      qreal m_lat;
      qreal m_long;
};

#endif // LOCATION_H
