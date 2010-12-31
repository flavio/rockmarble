#ifndef COUNTRYITEMCREATOR_H
#define COUNTRYITEMCREATOR_H

#include <MList>
#include <MContentItem>

class CountryItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    void updateCell(const QModelIndex &index, MWidget *cell) const;
};


#endif // COUNTRYITEMCREATOR_H
