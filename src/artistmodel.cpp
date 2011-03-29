#include "artistmodel.h"

#include <QtSql/QSqlRecord>

ArtistModel::ArtistModel(const DBManager::Storage& storage, QSqlQuery query,
                         QObject *parent)
  : QAbstractListModel(parent), m_dbStorage(storage)
{
  setQuery(query);

  connect(DBManager::instance(m_dbStorage), SIGNAL(artistAdded(const int, bool)),
          this, SLOT(slotArtistAdded(const int, bool)));
  connect(DBManager::instance(m_dbStorage), SIGNAL(artistAddedToEvent(int,int)),
          this, SLOT(slotArtistAddedToEvent(int,int)));
  connect(DBManager::instance(m_dbStorage), SIGNAL(artistUpdated(int)),
          this, SLOT(slotArtistUpdated(int)));
}

int ArtistModel::rowCount() const
{
  return m_artistIDs.size();
}

int ArtistModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return rowCount();
}

void ArtistModel::slotArtistAdded(const int artistID, bool favourite)
{
  if ((m_dbStorage == DBManager::DISK) && !favourite)
    return;

  QList<unsigned int> newIDs = getArtistIDs();
  int pos = newIDs.indexOf(artistID);
  beginInsertRows(createIndex(0,0), pos, pos+1);
  m_artistIDs.insert(pos, artistID);
  endInsertRows();
}

void ArtistModel::slotArtistAddedToEvent(int artistID, int eventID)
{
  Q_UNUSED(eventID);
  if (m_dbStorage == DBManager::MEMORY ||
      DBManager::instance(m_dbStorage)->isArtistFavourite(artistID)) {
    QModelIndex index = createIndex(m_artistIDs.indexOf(artistID), 0);
    emit dataChanged(index, index);
  }
}

void ArtistModel::slotArtistUpdated(int artistID)
{
  QModelIndex index = createIndex(m_artistIDs.indexOf(artistID), 0);
  emit dataChanged(index, index);
}

QVariant ArtistModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_artistIDs.size())
    return QVariant();

  if (role == Qt::DisplayRole || role == Qt::EditRole)
    return m_artistIDs.at(index.row());
  else
    return QVariant();
}

void ArtistModel::setQuery(QSqlQuery query)
{
  m_query = query;
  beginResetModel();
  m_artistIDs = getArtistIDs();
  endResetModel();
}

QList<unsigned int> ArtistModel::getArtistIDs()
{
  QList<unsigned int> artistIDs;
  if (m_query.exec()) {
    while (m_query.next()) {
      artistIDs << m_query.value(m_query.record().indexOf("id")).toInt();
    }
  }
  return artistIDs;
}
