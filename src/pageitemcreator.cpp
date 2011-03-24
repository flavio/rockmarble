#include "pageitemcreator.h"

void PageItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);

  Page page = pages.at(data.toInt());
  contentItem->setTitle(page.title);
  contentItem->boundingRect();
  contentItem->setImageID(page.imageID);
  contentItem->setSubtitle(page.subtitle);

}
