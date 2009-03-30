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
  m_lat = geo_point["geo:lat"].toInt();
  m_long = geo_point["geo:long"].toInt();
}
