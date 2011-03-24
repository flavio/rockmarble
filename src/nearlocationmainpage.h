#ifndef NEARLOCATIONMAINPAGE_H
#define NEARLOCATIONMAINPAGE_H

#include <MApplicationPage>
#include <QtCore/QList>
#include <QtCore/QModelIndex>

#include "page.h"

class NearLocationMainPage : public MApplicationPage
{
  Q_OBJECT
  public:
    NearLocationMainPage(QGraphicsItem *parent = 0);

    enum PageType {
      NewSearch,
      PreviousSearch
    };

  protected:
    void createContent();

  private slots:
    void slotItemClicked(QModelIndex);

  private:
    QList<Page> m_pages;
};

#endif // NEARLOCATIONMAINPAGE_H
