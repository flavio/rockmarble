#include "eventpage.h"
#include "eventdetailspage.h"
#include "eventmodel.h"
#include "eventitemcreator.h"

#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>


EventPage::EventPage(const DBManager::Storage& storage, QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_dbStorage(storage)
{
  m_pageMode = STARRED;
  setTitle(tr("Starrred events"));
}

EventPage::EventPage(const int& artistID,
                     const DBManager::Storage& storage,
                     const QString& country,
                     QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_artistID(artistID),
    m_dbStorage(storage),
    m_country(country)
{
  m_pageMode = ARTIST_BY_COUNTRY;
  setTitle(DBManager::instance(m_dbStorage)->artistNameFromID(artistID) +
           " - " + country);
}

EventPage::EventPage(const int& artistID,
                     const DBManager::Storage& storage,
                     QGraphicsItem *parent)
  : MApplicationPage(parent),
    m_artistID(artistID),
    m_dbStorage(storage)
{
  m_pageMode = ARTIST_NEAR_LOCATION;
  setTitle(DBManager::instance(m_dbStorage)->artistNameFromID(artistID) +
           " - " + tr("near you"));
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
  EventModel* eventModel = 0;
  switch (m_pageMode) {
    case ARTIST_BY_COUNTRY:
      eventModel = new EventModel(m_dbStorage, m_artistID, m_country);
      break;
    case ARTIST_NEAR_LOCATION:
      eventModel = new EventModel(m_dbStorage, m_artistID);
      break;
    case STARRED:
      eventModel = new EventModel(m_dbStorage, true);
      break;
  }

  EventItemCreator *cellCreator = new EventItemCreator(m_dbStorage);
  eventList->setCellCreator(cellCreator);
  eventList->setItemModel(eventModel);
  policy->addItem(eventList);

  connect (eventList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotEventClicked(QModelIndex)));
}

void EventPage::slotEventClicked(const QModelIndex& index)
{
  int event_id = index.data(Qt::DisplayRole).toInt();
  EventDetailsPage *eventDetailsPage = new EventDetailsPage(m_dbStorage,
                                                            event_id);
  eventDetailsPage->appear(MSceneWindow::DestroyWhenDismissed);
}
