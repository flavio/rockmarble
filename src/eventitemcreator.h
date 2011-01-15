#ifndef EVENTITEMCREATOR_H
#define EVENTITEMCREATOR_H

#include <MList>
#include <MContentItem>

class EventItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;
};

#endif // EVENTITEMCREATOR_H
