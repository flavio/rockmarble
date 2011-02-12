#include "artistpage.h"
#include "dbmanager.h"
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

CountryPage::CountryPage(QGraphicsItem *parent)
  : MApplicationPage(parent)
{
  setTitle(tr("Countries"));
  m_pageMode = ALL_COUNTRIES;
}

CountryPage::CountryPage(const int& artistID, QGraphicsItem *parent)
  : MApplicationPage(parent), m_artistID(artistID)
{
  setTitle(DBManager::instance()->artistFromID(artistID));
  m_pageMode = BY_ARTIST;
}

CountryPage::~CountryPage()
{
}

QSqlQuery CountryPage::getQuery()
{
  QString q;
  QSqlQuery query;
  if (m_pageMode == BY_ARTIST) {
    q = "SELECT locations.country FROM events "
        "JOIN artists_events ON artists_events.event_id = events.id "
        "JOIN locations ON locations.id = events.location_id "
        "WHERE artists_events.artist_id = ? "
        "GROUP BY locations.country "
        "ORDER BY locations.country ASC";
    query.prepare(q);
    query.addBindValue(m_artistID);
  } else if (m_pageMode == ALL_COUNTRIES) {
    q = "SELECT locations.country FROM events "
        "JOIN artists_events ON artists_events.event_id = events.id "
        "JOIN artists ON artists.id = artists_events.artist_id "
        "JOIN locations ON locations.id = events.location_id "
        "WHERE artists.favourite = ? "
        "GROUP BY locations.country "
        "ORDER BY locations.country ASC";
    query.prepare(q);
    query.addBindValue(1);
  }
  query.exec();
  return query;
}

void CountryPage::createContent()
{
  QSqlQueryModel* countryModel = new QSqlQueryModel();
  countryModel->setQuery(getQuery());

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
  if (m_pageMode == BY_ARTIST)
    page = new EventPage(m_artistID, country);
  else if (m_pageMode == ALL_COUNTRIES)
    page = new ArtistPage(country);

  page->appear(MSceneWindow::DestroyWhenDismissed);
}
