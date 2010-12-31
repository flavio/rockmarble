#include "eventpage.h"

#include "eventdetailspage.h"
#include "eventitemcreator.h"
#include "eventmodel.h"

#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

EventPage::EventPage(const QString& country, const EventList& events, QGraphicsItem *parent)
  : MApplicationPage(parent), m_events(events)
{
  setTitle(country);
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
  m_eventList = new MList();
  m_eventList->setSelectionMode(MList::SingleSelection);

  // Content item creator and item model for the list
  EventModel* eventModel = new EventModel(m_events);
  EventItemCreator *cellCreator = new EventItemCreator();
  cellCreator->model = eventModel;
  m_eventList->setCellCreator(cellCreator);
  m_eventList->setItemModel(eventModel);
  policy->addItem(m_eventList);

  connect (m_eventList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotEventClicked(QModelIndex)));
}

void EventPage::slotEventClicked(const QModelIndex& index)
{
  Event* event = m_events.at(index.row());
  EventDetailsPage *eventDetailsPage = new EventDetailsPage(event);
  eventDetailsPage->appear(MSceneWindow::DestroyWhenDismissed);
}
