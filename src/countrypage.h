#ifndef COUNTRYPAGE_H
#define COUNTRYPAGE_H

#include "countrymodel.h"
#include "dbmanager.h"
#include "event.h"

#include <MApplicationPage>

class MLabel;
class MLinearLayoutPolicy;
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
    void slotLocationCreated(int locationID);

  private:
    ISqlQuery getQuery();

    int m_artistID;
    CountryModel::Mode m_pageMode;
    CountryModel* m_countryModel;
    DBManager::Storage m_dbStorage;
    MLabel* m_noCountryLabel;
    MLinearLayoutPolicy* m_policy;
    bool m_noCountryLabelVisible;
};

#endif // COUNTRYPAGE_H
