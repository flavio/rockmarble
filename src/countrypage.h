#ifndef COUNTRYPAGE_H
#define COUNTRYPAGE_H

#include "countrymodel.h"
#include "dbmanager.h"
#include "event.h"

#include <MApplicationPage>


class MList;

class CountryPage : public MApplicationPage
{
  Q_OBJECT

  public:
    CountryPage(DBManager::Storage storage, QGraphicsItem *parent = 0);
    CountryPage(const int& artistID, DBManager::Storage storage,
                QGraphicsItem *parent = 0);
    ~CountryPage();

  protected:
    virtual void createContent();

  private slots:
    void slotCountryClicked(const QModelIndex& index);

  private:
    QSqlQuery getQuery();

    int m_artistID;
    CountryModel::Mode m_pageMode;
    DBManager::Storage m_dbStorage;
};

#endif // COUNTRYPAGE_H
