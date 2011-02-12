#ifndef EVENTDETAILSPAGE_H
#define EVENTDETAILSPAGE_H

#include <MApplicationPage>

class MAction;

class EventDetailsPage : public MApplicationPage
{
  Q_OBJECT

  public:
    EventDetailsPage(const int& event_id, QGraphicsItem *parent=0);

  protected:
    virtual void createContent();

  private slots:
    void slotChangeStar();

  private:
    MAction* m_actionStar;
    int  m_eventID;
    bool m_starred;
};

#endif // EVENTDETAILSPAGE_H
