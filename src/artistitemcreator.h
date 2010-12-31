#ifndef ARTISTITEMCREATOR_H
#define ARTISTITEMCREATOR_H

#include <MList>
#include <MContentItem>

class ArtistItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;
};

#endif // ARTISTITEMCREATOR_H
