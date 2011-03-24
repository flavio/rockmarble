#ifndef PAGEITEMCREATOR_H
#define PAGEITEMCREATOR_H

#include <MList>
#include <MContentItem>
#include <QtCore/QList>

#include "page.h"

class PageItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;

    QList<Page> pages;
};

#endif // PAGEITEMCREATOR_H
