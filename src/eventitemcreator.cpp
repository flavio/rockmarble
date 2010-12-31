#include "eventitemcreator.h"

#include "eventmodel.h"
#include "event.h"
#include "location.h"

void EventItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
    Event* event = model->getEvent(index);
    Location* location = event->location();
    contentItem->setTitle(location->city());
    contentItem->setSubtitle(event->dateTime().toString(Qt::TextDate));
    contentItem->boundingRect();

    if (index.row() == 0)
        contentItem->setItemMode(MContentItem::SingleColumnTop);
    else if (index.sibling(index.row() + 1, 0).isValid())
        contentItem->setItemMode(MContentItem::SingleColumnCenter);
    else
        contentItem->setItemMode(MContentItem::SingleColumnBottom);
}
