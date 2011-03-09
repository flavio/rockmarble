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
    void setArtistAllDataFetched(const int& artistID, bool done);
    void addEvent(const Event& event);
    void setEventStarred(const int& eventID, const bool& starred);
    int eventsWithArtistNum(const int& artistID);
    int eventsWithArtistInCountryNum(const int& artistID,
                                     const QString& country);
    QString artistNameFromID(const int& artistID);
    int artistIDFromName(const QString& artistName);
    bool artistHasImage(const int& artistID);
    bool artistHasAllData(const int& artistID);

    bool isArtistFavourite(const int& artistID);

    Event* eventFromID(const int& eventID);
    Location* locationFromID(const int& locationID);

  signals:
    void artistAdded(const int artistID, bool favourite);
    void artistUpdated(const int artistID);
    void artistAddedToEvent(const int artistID, const int eventID);

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
