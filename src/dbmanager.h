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
    void setArtistHasImage(const int& artistID, bool hasImage);
    void addEvent(const Event& event);
    int eventsWithArtistNum(const int& artistID);
    int eventsWithArtistInCountryNum(const int& artistID,
                                     const QString& country);
    QString artistFromID(const int& artistID);
    bool artistHasImage(const int& artistID);

    Event* eventFromID(const int& eventID);
    Location* locationFromID(const int& locationID);

  signals:
    void artistAdded(const QString& name, bool favourite);

  private:
    DBManager(QObject* parent = 0);
    void initDB();
    bool executeQuery(const QString&);
    bool executeQuery(QSqlQuery*);

    void addArtistToEvent(const QString& artist, const int& eventID);
    int addLocation(const Location* location);
    QStringList artistsFromEvent(const int& eventID);
};

#endif // DBMANAGER_H
