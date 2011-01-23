#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>

#include "event.h"

class EventPage : public MApplicationPage
{
  Q_OBJECT

  public:
    EventPage(const int& artistID, const QString& country, QGraphicsItem *parent = 0);
    ~EventPage();

  protected:
    virtual void createContent();

  private slots:
    void slotEventClicked(const QModelIndex& index);

  private:
    int m_artistID;
    QString m_country;
};

#endif // EVENTPAGE_H
