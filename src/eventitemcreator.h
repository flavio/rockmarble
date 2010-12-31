#ifndef EVENTITEMCREATOR_H
#define EVENTITEMCREATOR_H

#include <MList>
#include <MContentItem>

class EventModel;

class EventItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;

    EventModel* model;
};

#endif // EVENTITEMCREATOR_H
