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
qWarning() << "tables" << tables;
  if (!tables.contains("artists")) {
    executeQuery("create table artists ( "
               "id integer primary key autoincrement not null, "
               "name varchar(30) NOT NULL UNIQUE, "
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

int DBManager::addArtist(const QString &name, bool favourite)
{
  QSqlQuery q;
  q.prepare("SELECT id FROM artists WHERE name = ?");
  q.addBindValue(name);
  if (q.exec()) {
    if (q.next()) {
      //qWarning() << name << "already exists";
      return q.value(q.record().indexOf("id")).toInt();
    } else {
      q.prepare("INSERT INTO artists (name, favourite) VALUES (?,?)");
      q.addBindValue(name);
      q.addBindValue((int) favourite);
      q.exec();
      // TODO handle false
      //qWarning() << "created a new artist entry:" << name << favourite;
      int id = addArtist(name, favourite);
      emit artistAdded(id);
      return id;
    }
  } else {
    qWarning() << q.lastError();
    return -1;
  }
}

void DBManager::addEvent(const Event& event)
{
  int location_id = addLocation(event.location());

  QSqlQuery q;
  q.prepare("SELECT id FROM events WHERE id = ?");
  q.addBindValue(event.id());
  q.exec();
  if (!q.next()) {
    q.prepare("INSERT INTO events (id, title, description, start_date, "
              " location_id) VALUES (?,?,?,?,?)");
    q.addBindValue(event.id());
    q.addBindValue(event.title());
    q.addBindValue(event.description());
    q.addBindValue(event.dateTime());
    q.addBindValue(location_id);
    executeQuery(&q);
  }

  addArtistToEvent(event.headliner(), event.id());
  foreach (QString artist, event.artists()) {
    addArtistToEvent(artist, event.id());
  }
}

void DBManager::addArtistToEvent(const QString& artist, const int& event_id)
{
  int artist_id = addArtist(artist, false);

  QSqlQuery q;
  q.prepare("SELECT * FROM artists_events WHERE artist_id = ? AND "
            "event_id = ?");
  q.addBindValue(artist_id);
  q.addBindValue(event_id);
  if (executeQuery(&q)) {
    if (!q.next()) {
      q.prepare("INSERT INTO artists_events (artist_id, event_id) VALUES (?,?)");
      q.addBindValue(artist_id);
      q.addBindValue(event_id);
      executeQuery(&q);
    }
  }
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
  q.exec();
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
    q.exec();
    //qWarning() << "created a new location";
    return addLocation(location);
  }
}

int DBManager::eventsWithArtistNum(const QString &artist)
{
  QSqlQuery query ("select count(events.id) as event_num from events "
                   "join artists_events on events.id = artists_events.event_id "
                   "join artists on artists.id = artists_events.artist_id "
                   "where artists.name = ?");
  query.addBindValue(artist);
  executeQuery(&query);
  if (query.next()) {
    return query.value(query.record().indexOf("event_num")).toInt();
  } else {
    return 0;
  }
}

QStringList DBManager::artistsFromEvent(const int& event_id)
{
  QStringList artists;
  QSqlQuery query ("select artist_id from artists_events where event_id = ?");
  query.addBindValue(event_id);
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

Event* DBManager::eventFromID(const int &event_id)
{
  QSqlQuery query ("select * from events where id = ?");
  query.addBindValue(event_id);
  if (!executeQuery(&query))
    return 0;

  if (query.next()) {
    QStringList artists = artistsFromEvent(event_id);
    query.value(query.record().indexOf("event_num")).toInt();
    QString title = query.value(query.record().indexOf("title")).toString();
    QString description = query.value(query.record().indexOf("description")).toString();
    QDateTime dateTime = query.value(query.record().indexOf("start_date")).toDateTime();

    Location* location = locationFromID(query.value(query.record().indexOf("location_id")).toInt());
    return new Event (artists, title, description, dateTime, location);
  } else {
    return 0;
  }
}

Location* DBManager::locationFromID(const int &location_id)
{
  QSqlQuery query ("select * from locations where id = ? ");
  query.addBindValue(location_id);
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
