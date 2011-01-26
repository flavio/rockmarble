#include "dbmanager.h"
#include "eventpage.h"
#include "eventdetailspage.h"
#include "eventitemcreator.h"

#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

EventPage::EventPage(const int& artistID, const QString& country, QGraphicsItem *parent)
  : MApplicationPage(parent), m_artistID(artistID), m_country(country)
{
  setTitle(DBManager::instance()->artistFromID(artistID) + " - " + country);
}

EventPage::~EventPage()
{
}

void EventPage::createContent()
{
  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  // MList with fast view
  MList *eventList = new MList();
  eventList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  QSqlQueryModel* eventModel = new QSqlQueryModel();
  QSqlQuery query;
  query.prepare("SELECT events.id FROM events "
                "JOIN artists_events ON artists_events.event_id = events.id "
                "JOIN locations ON locations.id = events.location_id "
                "WHERE artists_events.artist_id = ? AND locations.country = ? "
                "ORDER BY events.start_date ASC");
  query.addBindValue(m_artistID);
  query.addBindValue(m_country);
  query.exec();
  eventModel->setQuery(query);

  EventItemCreator *cellCreator = new EventItemCreator();
  eventList->setCellCreator(cellCreator);
  eventList->setItemModel(eventModel);
  policy->addItem(eventList);

  connect (eventList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotEventClicked(QModelIndex)));
}

void EventPage::slotEventClicked(const QModelIndex& index)
{
  int event_id = index.data(Qt::DisplayRole).toInt();
  EventDetailsPage *eventDetailsPage = new EventDetailsPage(event_id);
  eventDetailsPage->appear(MSceneWindow::DestroyWhenDismissed);
}
