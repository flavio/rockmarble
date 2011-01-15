#include "eventitemcreator.h"

#include <QtCore/QDateTime>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

void EventItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  QSqlQuery q;
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);
  int event_id = data.toInt();

  // fetch start time
  q.prepare("SELECT start_date FROM events WHERE id = ?");
  q.addBindValue(event_id);
  q.exec();
  q.next();
  QDateTime startDate = q.value(q.record().indexOf("start_date")).toDateTime();

  // fetch city
  q.prepare("SELECT locations.city AS city, locations.name AS name "
            "FROM locations "
            "JOIN events on locations.id = events.location_id "
            "WHERE events.id = ?");
  q.addBindValue(event_id);
  q.exec();
  q.next();
  QString city = q.value(q.record().indexOf("city")).toString();
  QString location_name = q.value(q.record().indexOf("name")).toString();

  contentItem->setTitle(QString("%1 - %2").arg(city).arg(location_name));
  contentItem->setSubtitle(startDate.toString(Qt::TextDate));
  contentItem->boundingRect();

  if (index.row() == 0)
      contentItem->setItemMode(MContentItem::SingleColumnTop);
  else if (index.sibling(index.row() + 1, 0).isValid())
      contentItem->setItemMode(MContentItem::SingleColumnCenter);
  else
      contentItem->setItemMode(MContentItem::SingleColumnBottom);
}
