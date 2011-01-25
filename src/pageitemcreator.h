#ifndef PAGEITEMCREATOR_H
#define PAGEITEMCREATOR_H

#include <MList>
#include <MContentItem>
#include "mainpage.h"

class PageItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;

    QMap<MainPage::PageType, QString> pages;
};

#endif // PAGEITEMCREATOR_H
