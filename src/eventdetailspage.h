#ifndef EVENTDETAILSPAGE_H
#define EVENTDETAILSPAGE_H

#include <MApplicationPage>

class Event;

class EventDetailsPage : public MApplicationPage
{
  Q_OBJECT

  public:
    EventDetailsPage(Event* event, QGraphicsItem *parent=0);

  protected:
  virtual void createContent();

  private:
    Event* m_event;
};

#endif // EVENTDETAILSPAGE_H
