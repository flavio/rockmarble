#include "dbmanager.h"

#include "location.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QMutex>
#include <QtCore/QVariant>
#include <QtGui/QApplication>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>


DBManager* DBManager::m_mainDatabase = 0;
DBManager* DBManager::m_currentLocationDatabase = 0;

DBManager::DBManager(const Storage& storage, QObject *parent)
  : QObject(parent), m_storage(storage)
{
  initDB(storage);
}

DBManager* DBManager::instance(const Storage& storage) {
  static QMutex mainDBMutex;
  static QMutex locationDBMutex;
  switch (storage) {
    case DISK: {
      if (!m_mainDatabase) {
        mainDBMutex.lock();
        if (!m_mainDatabase)
          m_mainDatabase = new DBManager(storage, qApp);
        mainDBMutex.unlock();
      }
      return m_mainDatabase;
    } case MEMORY: {
      if (!m_currentLocationDatabase) {
        locationDBMutex.lock();
        if (!m_currentLocationDatabase)
          m_currentLocationDatabase = new DBManager(storage, qApp);
        locationDBMutex.unlock();
      }
      return m_currentLocationDatabase;
    }
  }
  qWarning() << "DBManager::instance is goint to return 0 - this shouldn't happen";
  return 0;
}

QSqlDatabase DBManager::database()
{
  QString dbID = (m_storage == DISK) ? "DISK" : "MEMORY";
  return QSqlDatabase::database(dbID);
}

bool DBManager::executeQuery(const QString& query)
{
  QSqlDatabase db = database();
  return executeQuery(query, db);
}

bool DBManager::executeQuery(const QString& query, QSqlDatabase& db)
{
  QSqlQuery q (query, db);
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

const QString DBManager::databaseFile(const Storage &storage)
{
  const QString dbPath (QDir::homePath() + "/.rockmarble/");
  QString dbFile;

  switch (storage) {
  case DISK:
    dbFile = "rockmarble.sqlite";
    break;
  case MEMORY:
    dbFile = "rockmarble_current_location.sqlite";
    break;
  }
  return dbPath + dbFile;
}

bool DBManager::isDatabaseEmpty()
{
  QSqlQuery q("SELECT COUNT (id) AS count_all from artists",
              database());
  if (executeQuery(&q)) {
    if (q.next()) {
      return q.value(q.record().indexOf("count_all")).toInt() == 0;
    } else
      return true;
  } else
    return true;
}

bool DBManager::removeDatabase(const Storage &storage)
{
  QDir dir;
  const QString dbFile = databaseFile(storage);
  if (!dir.exists(dbFile))
    return true;
  else
    return dir.remove(dbFile);
}

void DBManager::cleanTables()
{
  if (!QFile::exists(databaseFile(m_storage)))
    return;
  executeQuery("DELETE FROM artists_events");
  executeQuery("DELETE FROM events");
  executeQuery("DELETE FROM locations");
  executeQuery("DELETE FROM artists");
}

void DBManager::initDB(const Storage& storage) {
  QString dbID = (storage == DISK) ? "DISK" : "MEMORY";
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", dbID);
  QDir dir;

  const QString dbPath (QDir::homePath() + "/.rockmarble/");
  if (!dir.exists(dbPath))
    dir.mkpath(dbPath);

  db.setHostName("localhost");
  db.setDatabaseName(databaseFile(storage));
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
                 "favourite boolean NOT NULL DEFAULT 0)", db);
  }
  qWarning() << "post artists" << db.tables();
  if (!tables.contains("locations")) {
    executeQuery("create table locations ( "
                 "id integer primary key autoincrement not null, "
                 "name varchar(30), "
                 "city varchar(30) NOT NULL, "
                 "country varchar(30) NOT NULL, "
                 "street varchar(30) NOT NULL, "
                 "longitude real NOT NULL, "
                 "latitude real NOT NULL) ", db);
  }
  qWarning() << "post locations" << db.tables();
  if (!tables.contains("events")) {
    executeQuery("create table events ( "
                 "id integer primary key autoincrement not null, "
                 "location_id integer NOT NULL, "
                 "title varchar(30), "
                 "description varchar(30), "
                 "starred boolean NOT NULL DEFAULT 0, "
                 "start_date datetime NOT NULL, "
                 "FOREIGN KEY(location_id) REFERENCES locations(id)) ", db);
  }
  qWarning() << "post events" << db.tables();
  if (!tables.contains("artists_events")) {
    executeQuery("create table artists_events ( "
                 "artist_id integer NOT NULL, "
                 "event_id integer NOT NULL, "
                 "FOREIGN KEY(artist_id) REFERENCES artists(id), "
                 "FOREIGN KEY(event_id) REFERENCES events(id)) ", db);
  }
  qWarning() << "post artists_events" << db.tables();
}

int DBManager::artistIDFromName(const QString& artistName)
{
  QSqlQuery q(database());
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
  QSqlQuery q(database());
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
    QSqlQuery q(database());
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
  QSqlQuery q(database());
  q.prepare("UPDATE artists SET has_image = ? WHERE id = ?");
  q.addBindValue((int) hasImage);
  q.addBindValue(artistID);
  executeQuery(&q);
  emit artistUpdated(artistID);
}

void DBManager::setArtistAllDataFetched(const int& artistID, bool done)
{
  QSqlQuery q(database());
  q.prepare("UPDATE artists SET all_data_fetched = ? WHERE id = ?");
  q.addBindValue((int) done);
  q.addBindValue(artistID);
  executeQuery(&q);
  emit artistUpdated(artistID);
}


void DBManager::setEventStarred(const int &eventID, const bool &starred)
{
  QSqlQuery q(database());
  q.prepare("UPDATE events SET starred = ? WHERE id = ?");
  q.addBindValue((int) starred);
  q.addBindValue(eventID);
  executeQuery(&q);
}

void DBManager::addEvent(const Event& event)
{
  int location_id = addLocation(event.location());

  QSqlQuery q(database());
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

  QSqlQuery q(database());
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
  emit artistAddedToEvent(artistID, eventID);
}

int DBManager::addLocation(const Location *location)
{
  QSqlQuery q(database());
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
                   "where artists_events.artist_id = ?", database());
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
                   "and locations.country = ?", database());
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
  QSqlQuery q ("select name from artists where id = ?", database());
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
  QSqlQuery q ("select has_image from artists where id = ?", database());
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
  QSqlQuery q ("select all_data_fetched from artists where id = ?", database());
  q.addBindValue(artistID);
  if (executeQuery(&q)) {
    q.next();
    ret = q.value(q.record().indexOf("all_data_fetched")).toBool();
  }

  return ret;
}

void DBManager::setAllArtistHaveAllData()
{
  QSqlQuery q(database());
  int artistID;
  q.prepare("SELECT id FROM artists ");
  if (executeQuery(&q)) {
    while (q.next()) {
      artistID = q.value(q.record().indexOf("id")).toInt();
      setArtistAllDataFetched(artistID, true);
    }
  }
}

const QStringList DBManager::incompleteArtists()
{
  QStringList artists;
  QSqlQuery q(database());

  q.prepare("SELECT name FROM artists where "
            "favourite = ? AND all_data_fetched = ?");
  q.addBindValue((int) true);
  q.addBindValue((int) false);
  if (executeQuery(&q)) {
    while (q.next()) {
      artists << q.value(q.record().indexOf("name")).toString();
    }
  }
  return artists;
}

const QStringList DBManager::artistsWithoutImage()
{
  QStringList artists;
  QSqlQuery q(database());

  q.prepare("SELECT name FROM artists where "
            "favourite = ? AND has_image = ?");
  q.addBindValue((int) true);
  q.addBindValue((int) false);
  if (executeQuery(&q)) {
    while (q.next()) {
      artists << q.value(q.record().indexOf("name")).toString();
    }
  }
  return artists;
}

const QStringList DBManager::artists(bool favourite)
{
  QStringList artists;
  QSqlQuery q(database());

  q.prepare("SELECT name FROM artists where favourite = ?");
  q.addBindValue((int) favourite);
  if (executeQuery(&q)) {
    while (q.next()) {
      artists << q.value(q.record().indexOf("name")).toString();
    }
  }
  return artists;
}

QStringList DBManager::artistsFromEvent(const int& eventID)
{
  QStringList artists;
  QSqlQuery query ("select artist_id from artists_events where event_id = ?",
                   database());
  query.addBindValue(eventID);
  if (!executeQuery(&query))
    return artists;

  while (query.next()) {
    QSqlQuery q ("select name from artists where id = ?", database());
    q.addBindValue(query.value(query.record().indexOf("artist_id")).toInt());
    if (executeQuery(&q) && q.next())
      artists << q.value(q.record().indexOf("name")).toString();
  }
  return artists;
}

Event* DBManager::eventFromID(const int &eventID)
{
  QSqlQuery query ("select * from events where id = ?", database());
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
  QSqlQuery query ("select * from locations where id = ? ", database());
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

void DBManager::removeOldEvents(const QDate &upTo)
{
  QList<int> eventIDs;
  QSqlQuery query (database());
  query.prepare("select id from events where start_date < ? ");
  query.addBindValue(upTo);
  if (executeQuery(&query)) {
    while (query.next()) {
      eventIDs <<  query.value(query.record().indexOf("id")).toInt();
    }
  }

  foreach (int id, eventIDs) {
    QList<int> artistsIDs;

    // find all the favourite artists that are going to lose one event
    query.prepare("select artists.id from artists join artists_events on "
                  "artists.id = artists_events.artist_id where "
                  "artists.favourite = ? and artists_events.event_id = ?");
    query.addBindValue((int) true);
    query.addBindValue(id);
    if (executeQuery(&query)) {
      while (query.next())
        artistsIDs << query.value(query.record().indexOf("id")).toInt();
    }

    // delete all entries inside of artists_events
    query.prepare("delete from artists_events where event_id = ?");
    query.addBindValue(id);
    executeQuery(&query);

    foreach (int id, artistsIDs) {
      emit artistUpdated(id);
    }

    // delete all the non-favourite artists that have no event
    query.prepare("delete from artists where favourite = ? and "
                  "id not in (select artist_id from artists_events)");
    query.addBindValue((int) false);
    executeQuery(&query);

    query.prepare("delete from events where id = ?");
    query.addBindValue(id);
    executeQuery(&query);
  }

  executeQuery("delete from locations where id not in"
               "(select location_id from events)");
}
