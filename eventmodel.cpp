#include "eventmodel.h"
#include "artist.h"
#include "event.h"
#include "location.h"

EventModel::EventModel(Artist* artist, QObject* parent)
  : QAbstractTableModel(parent),
    m_artist(artist)
{
}

EventModel::~EventModel()
{
  delete m_artist;
}

int EventModel::rowCount (const QModelIndex & parent) const
{
  if (parent.isValid())
    return 0;
  else
    return m_artist->events().size();
}

int EventModel::columnCount ( const QModelIndex & parent) const
{
  if (parent.isValid())
    return 0;
  else {
    // country
    // city
    // location
    // date
    return 4;
  }
}

QModelIndex EventModel::index ( int row, int column, const QModelIndex & parent) const
{
  if (parent.isValid())
    return QModelIndex();

  if (row > m_artist->events().size())
    return QModelIndex();

  if (column > this->columnCount())
    return QModelIndex();

  return createIndex(row, column, m_artist);
}

QVariant EventModel::data ( const QModelIndex & index, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (!index.isValid())
    return QVariant();

  Artist* artist = static_cast<Artist*>(index.internalPointer());

  if (index.row() > artist->events().size())
    return QVariant();

  Event* event = artist->events().at(index.row());
  switch (index.column())
  {
    case 0:
      return event->location()->country();
    case 1:
      return event->location()->city();
    case 2:
      return QString("%1 - %2").arg(event->location()->name()).arg(event->location()->street());
    case 3:
      return event->date().toString();
    default:
      return QVariant();
  }
}

QVariant EventModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section)
    {
      case 0:
        return tr("Country");
      case 1:
        return tr("City");
      case 2:
        return tr("Location");
      case 3:
        return tr("Date");
      default:
        return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags EventModel::flags ( const QModelIndex & index ) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool EventModel::getCoordinates(const QModelIndex& index, qreal* latitude, qreal* longitude)
{
  if (!index.isValid())
    return false;

  Artist* artist = static_cast<Artist*>(index.internalPointer());

  if (index.row() > artist->events().size())
    return false;

  Event* event = artist->events().at(index.row());

  *latitude = event->location()->latitude();
  *longitude = event->location()->longitude();

  return true;
}

