#ifndef COUNTRYPAGE_H
#define COUNTRYPAGE_H

#include "event.h"

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QtCore/QMultiMap>

class MList;

class CountryPage : public MApplicationPage
{
  Q_OBJECT

  public:
    CountryPage(const QString& artist, const EventList& events, QGraphicsItem *parent = 0);
    ~CountryPage();

  protected:
    virtual void createContent();

  private slots:
    void slotCountryClicked(const QModelIndex& index);

  private:
    QMultiMap<QString, Event*> m_events;
};

#endif // COUNTRYPAGE_H
