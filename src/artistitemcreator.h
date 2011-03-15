#ifndef ARTISTITEMCREATOR_H
#define ARTISTITEMCREATOR_H

#include <MAbstractCellCreator>
#include <MContentItem>
#include <MWidgetRecycler>

#include "artistpage.h"
#include "dbmanager.h"

class ArtistItemCreator : public MAbstractCellCreator<MContentItem>
{
  public:
    ArtistItemCreator(const ArtistPage::PageMode& pageMode,
                      const DBManager::Storage& storage,
                      const QString& country = "");
    void updateCell(const QModelIndex &index, MWidget *cell) const;
    MWidget* createCell(const QModelIndex & 	index,
                             MWidgetRecycler & 	recycler) const;

  private:
    ArtistPage::PageMode m_pageMode;
    DBManager::Storage m_dbStorage;
    QString m_country;
};

#endif // ARTISTITEMCREATOR_H
