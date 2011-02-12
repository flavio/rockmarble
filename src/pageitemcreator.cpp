#include "pageitemcreator.h"

#include <QtCore/QObject>

void PageItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
  MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
  QVariant data = index.data(Qt::DisplayRole);

  contentItem->setTitle(pages.value((MainPage::PageType) data.toInt()));
  contentItem->boundingRect();
  switch ((MainPage::PageType) data.toInt()) {
    case MainPage::ByArtist:
      contentItem->setImageID("icon-m-content-artist");
      contentItem->setSubtitle(QObject::tr("Search the events by artist and then by country."));
      break;
    case MainPage::ByLocation:
      contentItem->setImageID("icon-m-clock-city-create");
      contentItem->setSubtitle(QObject::tr("Search the events by country and then by artist."));
      break;
    case MainPage::ByCurrentLocation:
      contentItem->setImageID("icon-m-common-location");
      contentItem->setSubtitle(QObject::tr("Show all the events taking place near you."));
      break;
    case MainPage::ByStar:
      contentItem->setImageID("icon-m-common-favorite-mark");
      contentItem->setSubtitle(QObject::tr("Show all the events you have starred."));
      break;
    }
}
