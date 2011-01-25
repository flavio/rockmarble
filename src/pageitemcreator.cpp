#include "pageitemcreator.h"

void PageItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);

  contentItem->setTitle(pages.value((MainPage::PageType) data.toInt()));
  contentItem->boundingRect();
}
