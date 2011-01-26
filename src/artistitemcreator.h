#ifndef ARTISTITEMCREATOR_H
#define ARTISTITEMCREATOR_H

#include <MList>
#include <MContentItem>

#include "artistpage.h"

class ArtistItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    ArtistItemCreator(const ArtistPage::PageMode& pageMode,
                      const QString& country = "");
    void updateCell(const QModelIndex &index, MWidget *cell) const;

  private:
    ArtistPage::PageMode m_pageMode;
    QString m_country;
};

#endif // ARTISTITEMCREATOR_H
