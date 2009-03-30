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
