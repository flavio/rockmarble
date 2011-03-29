#include "artistpage.h"
#include "countrypage.h"
#include "countryitemcreator.h"
#include "event.h"
#include "eventpage.h"
#include "isqlquery.h"
#include "location.h"

#include <MLabel>
#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QtGui/QStringListModel>
#include <QtSql/QSqlQueryModel>

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
  if (m_pageMode == CountryModel::BY_ARTIST)
    m_countryModel = new CountryModel(m_dbStorage, m_artistID);
  else
    m_countryModel = new CountryModel(m_dbStorage);

  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  m_policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  // No country label
  m_noCountryLabel = new MLabel(tr("No events to show.<br/>"
                                   "Possible reasons:<br/>"
                                   "<ul>"
                                   "<li>None of your artist is on tour.</li>"
                                   "<li>You don't have added an artist yet.</li>"
                                   "</ul>"));
  m_noCountryLabel->setAlignment(Qt::AlignJustify);
  if (m_countryModel->rowCount() == 0) {
    m_policy->addItem(m_noCountryLabel);
    m_noCountryLabelVisible = true;
  } else
    m_noCountryLabelVisible = false;

  MList *countryList = new MList();
  countryList->setSelectionMode(MList::SingleSelection);

  CountryItemCreator *cellCreator = new CountryItemCreator();
  countryList->setCellCreator(cellCreator);
  countryList->setItemModel(m_countryModel);
  m_policy->addItem(countryList);

  connect (countryList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotCountryClicked(QModelIndex)));
  connect (DBManager::instance(m_dbStorage), SIGNAL(locationCreated(int)),
           this, SLOT(slotLocationCreated(int)));
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

void CountryPage::slotLocationCreated(int locationID)
{
  Q_UNUSED(locationID)

  if (!m_noCountryLabelVisible)
    return;

  if ((m_pageMode == CountryModel::ALL_COUNTRIES) ||
      (m_countryModel->rowCount() > 0)) {
    m_noCountryLabelVisible = false;
    m_policy->removeItem(m_noCountryLabel);
  }
}
