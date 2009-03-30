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
