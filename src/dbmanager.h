#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtCore/QString>
#include <QtCore/QObject>

#include "event.h"

class Location;
class QSqlQuery;

class DBManager : public QObject
{
  Q_OBJECT
  public:
    static DBManager* instance();

    int addArtist(const QString& name, bool favourite = false);
    void addEvent(const Event& event);
    int eventsWithArtistNum(const QString& artist);
    Event* eventFromID(const int& event_id);
    Location* locationFromID(const int& location_id);

  signals:
    void artistAdded(const QString& name);

  private:
    DBManager(QObject* parent = 0);
    void initDB();
    bool executeQuery(const QString&);
    bool executeQuery(QSqlQuery*);

    void addArtistToEvent(const QString& artist, const int& event_id);
    int addLocation(const Location* location);
    QStringList artistsFromEvent(const int& event_id);
};

#endif // DBMANAGER_H
