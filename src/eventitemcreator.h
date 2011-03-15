#ifndef EVENTITEMCREATOR_H
#define EVENTITEMCREATOR_H

#include <MAbstractCellCreator>
#include <MContentItem>

#include "dbmanager.h"

class EventItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    EventItemCreator(const DBManager::Storage& storage);
    void updateCell(const QModelIndex &index, MWidget *cell) const;

  private:
    DBManager::Storage m_dbStorage;
};

#endif // EVENTITEMCREATOR_H
