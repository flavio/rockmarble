#include "artistpage.h"
#include "countrypage.h"
#include "countryitemcreator.h"
#include "event.h"
#include "eventpage.h"
#include "location.h"

#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QtGui/QStringListModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

#include <QDebug>

CountryPage::CountryPage(DBManager::Storage storage, QGraphicsItem *parent)
  : MApplicationPage(parent), m_dbStorage(storage)
{
  setTitle(tr("Countries"));
  m_pageMode = CountryModel::ALL_COUNTRIES;
}

CountryPage::CountryPage(const int& artistID, DBManager::Storage storage,
                         QGraphicsItem *parent)
  : MApplicationPage(parent), m_artistID(artistID), m_dbStorage(storage)
{
  m_pageMode = CountryModel::BY_ARTIST;
  m_dbStorage = DBManager::DISK;
  setTitle(DBManager::instance(m_dbStorage)->artistNameFromID(artistID));
}

CountryPage::~CountryPage()
{
}

void CountryPage::createContent()
{
  CountryModel* countryModel;
  if (m_pageMode == CountryModel::BY_ARTIST)
    countryModel = new CountryModel(m_dbStorage, m_artistID);
  else
    countryModel = new CountryModel(m_dbStorage);

  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  MList *countryList = new MList();
  countryList->setSelectionMode(MList::SingleSelection);

  CountryItemCreator *cellCreator = new CountryItemCreator();
  countryList->setCellCreator(cellCreator);
  countryList->setItemModel(countryModel);
  policy->addItem(countryList);

  connect (countryList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotCountryClicked(QModelIndex)));
}

void CountryPage::slotCountryClicked(const QModelIndex& index)
{
  QString country= index.data(Qt::DisplayRole).toString();
  MApplicationPage* page;
  if (m_pageMode == CountryModel::BY_ARTIST)
    page = new EventPage(m_artistID, m_dbStorage, country);
  else if (m_pageMode == CountryModel::ALL_COUNTRIES)
    page = new ArtistPage(m_dbStorage, country);

  page->appear(MSceneWindow::DestroyWhenDismissed);
}
