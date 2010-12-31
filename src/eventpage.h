#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>

#include "event.h"

class MList;

class EventPage : public MApplicationPage
{
  Q_OBJECT

  public:
    EventPage(const QString& country, const EventList& eventList, QGraphicsItem *parent = 0);
    ~EventPage();

  protected:
    virtual void createContent();

  private slots:
    void slotEventClicked(const QModelIndex& index);

  private:
    EventList m_events;
    MList* m_eventList;
};

#endif // EVENTPAGE_H
