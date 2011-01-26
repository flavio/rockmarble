#ifndef COUNTRYPAGE_H
#define COUNTRYPAGE_H

#include "event.h"

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QtCore/QMultiMap>
#include <QtSql/QSqlQuery>

class MList;

class CountryPage : public MApplicationPage
{
  Q_OBJECT

  public:
    CountryPage(QGraphicsItem *parent = 0);
    CountryPage(const int& artistID, QGraphicsItem *parent = 0);
    ~CountryPage();

  protected:
    virtual void createContent();

  private slots:
    void slotCountryClicked(const QModelIndex& index);

  private:
    QSqlQuery getQuery();

    int m_artistID;
};

#endif // COUNTRYPAGE_H
