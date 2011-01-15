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

CountryPage::CountryPage(const QString& artist, QGraphicsItem *parent)
  : MApplicationPage(parent), m_artist(artist)
{
  setTitle(m_artist);
}

CountryPage::~CountryPage()
{
}

void CountryPage::createContent()
{
  QSqlQueryModel* countryModel = new QSqlQueryModel();
  QSqlQuery query;
  query.prepare("SELECT locations.country FROM events "
                 "JOIN artists_events ON artists_events.event_id = events.id "
                 "JOIN artists ON artists.id = artists_events.artist_id "
                 "JOIN locations ON locations.id = events.location_id "
                 "WHERE artists.name = ? "
                 "GROUP BY locations.country "
                 "ORDER BY locations.country ASC");
  query.addBindValue(m_artist);
  query.exec();
  countryModel->setQuery(query);

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
  EventPage *eventPage = new EventPage(m_artist, country);
  eventPage->appear(MSceneWindow::DestroyWhenDismissed);
}
