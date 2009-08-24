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

#include "location.h"

Location::Location(const QVariant& data )
{
  QVariantMap venue = data.toMap();
  m_name = venue["name"].toString();

  QVariantMap location = venue["location"].toMap();
  m_city = location["city"].toString();
  m_country = location["country"].toString();
  m_street = location["street"].toString();

  QVariantMap geo_point = location["geo:point"].toMap();
  m_lat = geo_point["geo:lat"].toDouble();
  m_long = geo_point["geo:long"].toDouble();
}

bool Location::operator==(const Location& l) const
{
  if (m_city != l.m_city)
    return false;

  if (m_country != l.m_country)
    return false;

  if (m_lat != l.m_lat)
    return false;

  if (m_long != l.m_long)
    return false;

  if (m_name != l.m_name)
    return false;

  if (m_street != l.m_street)
    return false;

  return true;
}

