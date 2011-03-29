#include "eventmodel.h"
#include "dbmanager.h"
#include "event.h"
#include "location.h"

#include <QtSql/QSqlRecord>

EventModel::EventModel(const DBManager::Storage &storage,
                       bool starred, QObject *parent)
  : QAbstractListModel(parent), m_dbStorage(storage), m_starred(starred)
{
  m_mode = STARRED;
  setup();
}

EventModel::EventModel(const DBManager::Storage& storage,
                       const int artistID, const QString& country,
                       QObject *parent)
  : QAbstractListModel(parent),
    m_dbStorage(storage),
    m_artistID(artistID),
    m_country(country)
{
  m_mode = ARTIST_BY_COUNTRY;
  setup();
}

EventModel::EventModel(const DBManager::Storage& storage,
                       const int artistID,
                       QObject *parent)
  : QAbstractListModel(parent),
    m_dbStorage(storage),
    m_artistID(artistID)
{
  m_mode = ARTIST_NEAR_LOCATION;
  setup();
}

void EventModel::setup()
{
  DBManager* db = DBManager::instance(m_dbStorage);
  connect(db, SIGNAL(eventCreated(int)),
          this, SLOT(slotEventCreated(int)));

  m_eventIDs = getEventIDs();
}

int EventModel::rowCount() const
{
  return m_eventIDs.size();
}

int EventModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return rowCount();
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_eventIDs.size())
    return QVariant();

  if (role == Qt::DisplayRole || role == Qt::EditRole)
    return m_eventIDs.at(index.row());
  else
    return QVariant();
}

QList<int> EventModel::getEventIDs()
{
  QList<int> eventIDs;
  ISqlQuery query = getQuery();
  if (query.exec()) {
    while (query.next()) {
      eventIDs << query.value(query.record().indexOf("event_id")).toInt();
    }
  }
  return eventIDs;
}

ISqlQuery EventModel::getQuery()
{
  QString q;
  ISqlQuery query(DBManager::instance(m_dbStorage)->database());
  switch(m_mode) {
    case ARTIST_BY_COUNTRY:
      query.prepare("SELECT events.id AS event_id FROM events "
                    "JOIN artists_events ON artists_events.event_id = events.id "
                    "JOIN locations ON locations.id = events.location_id "
                    "WHERE artists_events.artist_id = ? AND locations.country = ? "
                    "ORDER BY events.start_date ASC");
      query.addBindValue(m_artistID);
      query.addBindValue(m_country);
      break;
    case ARTIST_NEAR_LOCATION:
      query.prepare("SELECT events.id AS event_id FROM events "
                    "JOIN artists_events ON artists_events.event_id = events.id "
                    "JOIN locations ON locations.id = events.location_id "
                    "WHERE artists_events.artist_id = ? "
                    "ORDER BY events.start_date ASC");
      query.addBindValue(m_artistID);
      break;
    case STARRED:
      query.prepare("SELECT events.id AS event_id FROM events "
                    "WHERE starred = ? "
                    "ORDER BY events.start_date ASC");
      query.addBindValue((int) true);
      break;
  }
  query.exec();
  return query;
}

void EventModel::slotEventCreated(int eventID)
{
  DBManager* db = DBManager::instance(m_dbStorage);

  if ((m_mode == ARTIST_NEAR_LOCATION) ||  (m_mode == ARTIST_BY_COUNTRY)) {
    QString artist = db->artistNameFromID(m_artistID);
    Event* event = db->eventFromID(eventID);
    QStringList artists = event->artists();
    delete event;
    if (!artists.contains(artist))
      return;
  }

  if (m_mode == STARRED) {
    Event* event = db->eventFromID(eventID);
    QStringList artists = event->artists();
    delete event;
    foreach(QString artist, artists) {
      if (db->isArtistFavourite(artist) != m_starred)
        return;
    }
  }

  if (m_mode == ARTIST_BY_COUNTRY) {
    Event* event = db->eventFromID(eventID);
    QString country = event->location()->country();
    delete event;
    if (country.compare(m_country) != 0)
      return;
  }

  QList<int> newIDs = getEventIDs();
  int pos = newIDs.indexOf(eventID);
  beginInsertRows(createIndex(0,0), pos, pos+1);
  m_eventIDs.insert(pos, eventID);
  endInsertRows();
}
