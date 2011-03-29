#include "countrymodel.h"
#include "dbmanager.h"
#include "location.h"

#include <QtCore/QtAlgorithms>
#include <QtSql/QSqlRecord>

CountryModel::CountryModel(const DBManager::Storage& storage, QObject *parent)
  : QAbstractListModel(parent), m_dbStorage(storage)
{
  m_mode = ALL_COUNTRIES;
  setup();
}

CountryModel::CountryModel(const DBManager::Storage &storage,
                           const int &artistID, QObject *parent)
  : QAbstractListModel(parent), m_artistID(artistID), m_dbStorage(storage)
{
  m_mode = BY_ARTIST;
  setup();
}

void CountryModel::setup()
{
  DBManager* db = DBManager::instance(m_dbStorage);
  connect(db, SIGNAL(locationCreated(int)),
          this, SLOT(slotLocationCreated(int)));

  m_countries = getCountries();
}

int CountryModel::rowCount() const
{
  return m_countries.size();
}

int CountryModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return rowCount();
}

void CountryModel::slotLocationCreated(int locationID)
{
  DBManager* db = DBManager::instance(m_dbStorage);

  if (m_mode == BY_ARTIST) {
    bool relevant = db->artistsPlaysInLocation(m_artistID, locationID);
    if (!relevant)
      return;
  }

  Location* location = db->locationFromID(locationID);
  QString country = location->country();
  delete location;

  if (!m_countries.contains(country, Qt::CaseInsensitive)) {
    QStringList countries = m_countries;
    countries << country;
    qSort(countries);
    int pos = countries.indexOf(country);
    beginInsertRows(createIndex(0,0), pos, pos+1);
    m_countries = countries;
    endInsertRows();
  }
}

QVariant CountryModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_countries.size())
    return QVariant();

  if (role == Qt::DisplayRole || role == Qt::EditRole)
    return m_countries.at(index.row());
  else
    return QVariant();
}

QStringList CountryModel::getCountries()
{
  QStringList countries;
  ISqlQuery query = getQuery();
  if (query.exec()) {
    while (query.next()) {
      countries << query.value(query.record().indexOf("country")).toString();
    }
  }
  return countries;
}

ISqlQuery CountryModel::getQuery()
{
  QString q;
  ISqlQuery query(DBManager::instance(m_dbStorage)->database());
  if (m_mode == BY_ARTIST) {
    q = "SELECT locations.country as country FROM events "
        "JOIN artists_events ON artists_events.event_id = events.id "
        "JOIN locations ON locations.id = events.location_id "
        "WHERE artists_events.artist_id = ? "
        "GROUP BY locations.country "
        "ORDER BY locations.country ASC";
    query.prepare(q);
    query.addBindValue(m_artistID);
  } else if (m_mode == ALL_COUNTRIES) {
    q = "SELECT locations.country as country FROM events "
        "JOIN artists_events ON artists_events.event_id = events.id "
        "JOIN artists ON artists.id = artists_events.artist_id "
        "JOIN locations ON locations.id = events.location_id "
        "WHERE artists.favourite = ? "
        "GROUP BY locations.country "
        "ORDER BY locations.country ASC";
    query.prepare(q);
    query.addBindValue(1);
  }
  query.exec();
  return query;
}
