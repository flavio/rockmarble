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
    int latitude() { return m_lat; }
    int longitude() { return m_long; }

    private:
      QString m_name;
      QString m_city;
      QString m_country;
      QString m_street;
      int m_lat;
      int m_long;
};

#endif // LOCATION_H
