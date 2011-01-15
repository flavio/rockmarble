#ifndef EVENTDETAILSPAGE_H
#define EVENTDETAILSPAGE_H

#include <MApplicationPage>


class EventDetailsPage : public MApplicationPage
{
  Q_OBJECT

  public:
    EventDetailsPage(const int& event_id, QGraphicsItem *parent=0);

  protected:
  virtual void createContent();

  private:
    int m_event_id;
};

#endif // EVENTDETAILSPAGE_H
