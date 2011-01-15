#include "artistitemcreator.h"
#include "dbmanager.h"

void ArtistItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);
  QString artist = data.toString();

  contentItem->setTitle(artist);
  contentItem->boundingRect();

  int eventsNum = DBManager::instance()->eventsWithArtistNum(artist);
  if (eventsNum > 1)
    contentItem->setSubtitle(QString("%1 concerts").arg(eventsNum));
  else if (eventsNum == 1)
    contentItem->setSubtitle("1 event");
  else if (eventsNum == 0)
    contentItem->setSubtitle("No events yet.");
  else
    contentItem->setSubtitle("Error while retrieving data.");

  if (index.row() == 0)
      contentItem->setItemMode(MContentItem::SingleColumnTop);
  else if (index.sibling(index.row() + 1, 0).isValid())
      contentItem->setItemMode(MContentItem::SingleColumnCenter);
  else
      contentItem->setItemMode(MContentItem::SingleColumnBottom);
}
