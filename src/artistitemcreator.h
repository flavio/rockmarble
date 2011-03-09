#ifndef ARTISTITEMCREATOR_H
#define ARTISTITEMCREATOR_H

#include <MAbstractCellCreator>
#include <MContentItem>
#include <MWidgetRecycler>

#include "artistpage.h"

class ArtistItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    ArtistItemCreator(const ArtistPage::PageMode& pageMode,
                      const QString& country = "");
    void updateCell(const QModelIndex &index, MWidget *cell) const;
    MWidget* createCell(const QModelIndex & 	index,
                             MWidgetRecycler & 	recycler) const;

  private:
    ArtistPage::PageMode m_pageMode;
    QString m_country;
};

#endif // ARTISTITEMCREATOR_H
