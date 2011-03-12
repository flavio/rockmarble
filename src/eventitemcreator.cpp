#include "eventitemcreator.h"
#include "dbmanager.h"
#include "event.h"
#include "location.h"

void EventItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);
  int eventID = data.toInt();
  Event* event = DBManager::instance()->eventFromID(eventID);

  QDateTime startDate = event->dateTime();

  Location* location = event->location();
  QString city = location->city();
  QString location_name = location->name();

  contentItem->setTitle(QString("%1 - %2").arg(city).arg(location_name));
  contentItem->setSubtitle(startDate.toString(Qt::TextDate));
  contentItem->boundingRect();

  if (index.row() == 0)
      contentItem->setItemMode(MContentItem::SingleColumnTop);
  else if (index.sibling(index.row() + 1, 0).isValid())
      contentItem->setItemMode(MContentItem::SingleColumnCenter);
  else
      contentItem->setItemMode(MContentItem::SingleColumnBottom);

//  if (event->starred()) {
//    contentItem->setOptionalImageID("icon-m-toolbar-favorite-mark");
//  } else {
//    contentItem->setOptionalImageID("icon-m-toolbar-favorite-unmark");
//  }

  delete event;
}
