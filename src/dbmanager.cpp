#include "dbmanager.h"

#include "location.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtGui/QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>


DBManager::DBManager(QObject *parent)
  : QObject(parent)
{
  initDB();
}

DBManager* DBManager::instance() {
  static DBManager * dbManagerInstance = 0;
  if (!dbManagerInstance) {
    dbManagerInstance = new DBManager(qApp);
  }

  return dbManagerInstance;
}

bool DBManager::executeQuery(const QString& query)
{
  QSqlQuery q (query);
  return executeQuery(&q);
}

bool DBManager::executeQuery(QSqlQuery* q)
{
  if (!q->exec()) {
    qWarning() << "error while executing" << q->lastQuery();
    qWarning() << q->lastError();
    return false;
  } else
    return true;
}

void DBManager::initDB() {
  const QString dbPath (QDir::homePath() + "/.rockmarble/");
  const QString dbFile ("rockmarble.sqlite");
  QDir dir;

  if (!dir.exists(dbPath))
    dir.mkpath(dbPath);

  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setHostName("localhost");
  db.setDatabaseName(dbPath + dbFile);
  bool ok = db.open();

  if (!ok) {
    qFatal("unable to open/create the db");
  }

  QStringList tables = db.tables();
  qWarning() << "db tables" << tables;

  executeQuery("PRAGMA foreign_keys = ON");

  if (!tables.contains("artists")) {
    executeQuery("create table artists ( "
               "id integer primary key autoincrement not null, "
               "name varchar(30) NOT NULL UNIQUE, "
               "all_data_fetched boolean NOT NULL DEFAULT 0, "
               "has_image boolean NOT NULL DEFAULT 0, "
               "favourite boolean NOT NULL DEFAULT 0)");
  }
  if (!tables.contains("locations")) {
    executeQuery("create table locations ( "
               "id integer primary key autoincrement not null, "
               "name varchar(30), "
               "city varchar(30) NOT NULL, "
               "country varchar(30) NOT NULL, "
               "street varchar(30) NOT NULL, "
               "longitude real NOT NULL, "
               "latitude real NOT NULL) ");
  }
  if (!tables.contains("events")) {
    executeQuery("create table events ( "
               "id integer primary key autoincrement not null, "
               "location_id integer NOT NULL, "
               "title varchar(30), "
               "description varchar(30), "
               "starred boolean NOT NULL DEFAULT 0, "
               "start_date datetime NOT NULL, "
               "FOREIGN KEY(location_id) REFERENCES locations(id)) ");
  }
  if (!tables.contains("artists_events")) {
    executeQuery("create table artists_events ( "
               "artist_id integer NOT NULL, "
               "event_id integer NOT NULL, "
               "FOREIGN KEY(artist_id) REFERENCES artists(id), "
               "FOREIGN KEY(event_id) REFERENCES events(id)) ");
  }
}

int DBManager::artistIDFromName(const QString& artistName)
{
  QSqlQuery q;
  q.prepare("SELECT id FROM artists WHERE name = ?");
  q.addBindValue(artistName);
  if (executeQuery(&q)) {
    if (q.next()) {
      //qWarning() << artistName << "already exists";
      return q.value(q.record().indexOf("id")).toInt();
    } else
      return -1;
  } else
    return -1;
}

bool DBManager::isArtistFavourite(const int& artistID)
{
  QSqlQuery q;
  q.prepare("SELECT id FROM artists WHERE id = ? AND favourite = ?");
  q.addBindValue(artistID);
  q.addBindValue((int)true);
  if (executeQuery(&q)) {
    if (!q.next())
      return false;
    else
      return true;
  } else
    return false;
}

int DBManager::addArtist(const QString &name, bool favourite)
{
  int artistID = artistIDFromName(name);

  if (artistID != -1) {
    return artistID;
  } else {
    QSqlQuery q;
    q.prepare("INSERT INTO artists (name, favourite) VALUES (?,?)");
    q.addBindValue(name);
    q.addBindValue((int) favourite);
    if (executeQuery(&q)) {
      // TODO handle false
      //qWarning() << "created a new artist entry:" << name << favourite;
      artistID = addArtist(name, favourite);
      emit artistAdded(artistID, favourite);
      return artistID;
    } else
      return -1;
  }
}

void DBManager::setArtistHasImage(const int &artistID, bool hasImage)
{
  QSqlQuery q;
  q.prepare("UPDATE artists SET has_image = ? WHERE id = ?");
  q.addBindValue((int) hasImage);
  q.addBindValue(artistID);
  executeQuery(&q);
  emit artistUpdated(artistID);
}

void DBManager::setArtistAllDataFetched(const int& artistID, bool done)
{
  QSqlQuery q;
  q.prepare("UPDATE artists SET all_data_fetched = ? WHERE id = ?");
  q.addBindValue((int) done);
  q.addBindValue(artistID);
  executeQuery(&q);
  emit artistUpdated(artistID);
}


void DBManager::setEventStarred(const int &eventID, const bool &starred)
{
  QSqlQuery q;
  q.prepare("UPDATE events SET starred = ? WHERE id = ?");
  q.addBindValue((int) starred);
  q.addBindValue(eventID);
  executeQuery(&q);
}

void DBManager::addEvent(const Event& event)
{
  int location_id = addLocation(event.location());

  QSqlQuery q;
  q.prepare("SELECT id FROM events WHERE id = ?");
  q.addBindValue(event.id());
  if (executeQuery(&q)) {
    if (!q.next()) {
      q.prepare("INSERT INTO events (id, title, description, start_date, "
                " starred, location_id) VALUES (?,?,?,?,?,?)");
      q.addBindValue(event.id());
      q.addBindValue(event.title());
      q.addBindValue(event.description());
      q.addBindValue(event.dateTime());
      q.addBindValue((int) event.starred());
      q.addBindValue(location_id);
      executeQuery(&q);
    }

    addArtistToEvent(event.headliner(), event.id());
    foreach (QString artist, event.artists()) {
      addArtistToEvent(artist, event.id());
    }
  }
}

void DBManager::addArtistToEvent(const QString& artist, const int& eventID)
{
  int artistID = addArtist(artist, false);

  QSqlQuery q;
  q.prepare("SELECT * FROM artists_events WHERE artist_id = ? AND "
            "event_id = ?");
  q.addBindValue(artistID);
  q.addBindValue(eventID);
  if (executeQuery(&q)) {
    if (!q.next()) {
      q.prepare("INSERT INTO artists_events (artist_id, event_id) VALUES (?,?)");
      q.addBindValue(artistID);
      q.addBindValue(eventID);
      executeQuery(&q);
    }
  }
  if (isArtistFavourite(artistID))
    emit artistAddedToEvent(artistID, eventID);
}

int DBManager::addLocation(const Location *location)
{
  QSqlQuery q;
  q.prepare("SELECT id FROM locations WHERE name = ? AND "
            "city = ? AND "
            "country = ? AND "
            "latitude = ? AND "
            "longitude = ? AND "
            "street = ?");
  q.addBindValue(location->name());
  q.addBindValue(location->city());
  q.addBindValue(location->country());
  q.addBindValue(location->latitude());
  q.addBindValue(location->longitude());
  q.addBindValue(location->street());
  if (executeQuery(&q)) {
    if (q.next()) {
      return q.value(q.record().indexOf("id")).toInt();
    } else {
      q.prepare("INSERT INTO locations (name, city, country, "
                "latitude, longitude, street) VALUES (?,?,?,?,?,?)");
      q.addBindValue(location->name());
      q.addBindValue(location->city());
      q.addBindValue(location->country());
      q.addBindValue(location->latitude());
      q.addBindValue(location->longitude());
      q.addBindValue(location->street());
      if (executeQuery(&q)) {
        //qWarning() << "created a new location";
        return addLocation(location);
      } else
        return -1;
    }
  } else
    return -1;
}

int DBManager::eventsWithArtistNum(const int &artistID)
{
  QSqlQuery query ("select count(events.id) as event_num from events "
                   "join artists_events on events.id = artists_events.event_id "
                   "where artists_events.artist_id = ?");
  query.addBindValue(artistID);
  executeQuery(&query);
  if (query.next()) {
    return query.value(query.record().indexOf("event_num")).toInt();
  } else {
    return 0;
  }
}

int DBManager::eventsWithArtistInCountryNum(const int &artistID,
                                            const QString &country)
{
  QSqlQuery query ("select count(events.id) as event_num from events "
                   "join artists_events on events.id = artists_events.event_id "
                   "join locations on events.location_id = locations.id "
                   "where artists_events.artist_id = ?"
                   "and locations.country = ?");
  query.addBindValue(artistID);
  query.addBindValue(country);
  executeQuery(&query);
  if (query.next()) {
    return query.value(query.record().indexOf("event_num")).toInt();
  } else {
    return 0;
  }
}

QString DBManager::artistNameFromID(const int &artistID)
{
  QString artist;
  QSqlQuery q ("select name from artists where id = ?");
  q.addBindValue(artistID);
  if (executeQuery(&q)) {
    q.next();
    artist = q.value(q.record().indexOf("name")).toString();
  }

  return artist;
}

bool DBManager::artistHasImage(const int &artistID)
{
  bool ret = false;
  QSqlQuery q ("select has_image from artists where id = ?");
  q.addBindValue(artistID);
  if (executeQuery(&q)) {
    q.next();
    ret = q.value(q.record().indexOf("has_image")).toBool();
  }

  return ret;
}

bool DBManager::artistHasAllData(const int &artistID)
{
  bool ret = false;
  QSqlQuery q ("select all_data_fetched from artists where id = ?");
  q.addBindValue(artistID);
  if (executeQuery(&q)) {
    q.next();
    ret = q.value(q.record().indexOf("all_data_fetched")).toBool();
  }

  return ret;
}


QStringList DBManager::artistsFromEvent(const int& eventID)
{
  QStringList artists;
  QSqlQuery query ("select artist_id from artists_events where event_id = ?");
  query.addBindValue(eventID);
  if (!executeQuery(&query))
    return artists;

  while (query.next()) {
    QSqlQuery q ("select name from artists where id = ?");
    q.addBindValue(query.value(query.record().indexOf("artist_id")).toInt());
    if (executeQuery(&q) && q.next())
      artists << q.value(q.record().indexOf("name")).toString();
  }
  return artists;
}

Event* DBManager::eventFromID(const int &eventID)
{
  QSqlQuery query ("select * from events where id = ?");
  query.addBindValue(eventID);
  if (!executeQuery(&query))
    return 0;

  if (query.next()) {
    QStringList artists = artistsFromEvent(eventID);
    query.value(query.record().indexOf("event_num")).toInt();
    QString title = query.value(query.record().indexOf("title")).toString();
    QString description = query.value(query.record().indexOf("description")).toString();
    QDateTime dateTime = query.value(query.record().indexOf("start_date")).toDateTime();
    bool starred = query.value(query.record().indexOf("starred")).toBool();

    Location* location = locationFromID(query.value(query.record().indexOf("location_id")).toInt());
    return new Event (artists, title, description, dateTime, starred, location);
  } else {
    return 0;
  }
}

Location* DBManager::locationFromID(const int &locationID)
{
  QSqlQuery query ("select * from locations where id = ? ");
  query.addBindValue(locationID);
  if (!executeQuery(&query))
    return 0;

  if (query.next()) {
    QString name = query.value(query.record().indexOf("name")).toString();
    QString city = query.value(query.record().indexOf("city")).toString();
    QString country = query.value(query.record().indexOf("country")).toString();
    QString street = query.value(query.record().indexOf("street")).toString();
    qreal latitude = query.value(query.record().indexOf("latitude")).toReal();
    qreal longitude = query.value(query.record().indexOf("longitude")).toReal();
    return new Location (name, city, country, street, latitude, longitude);
  } else {
    return 0;
  }
}
