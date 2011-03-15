#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <MApplicationPage>
#include <QtCore/QModelIndex>
#include <QtSql/QSqlQuery>

#include "dbmanager.h"
#include "event.h"

class EventPage : public MApplicationPage
{
  Q_OBJECT

  public:
    enum PageMode {STARRED, ARTIST_BY_COUNTRY};

    // This constructor is used to show all the starred events
    EventPage(const DBManager::Storage& storage, QGraphicsItem *parent = 0);
    EventPage(const int& artistID,
              const DBManager::Storage& storage,
              const QString& country,
              QGraphicsItem *parent = 0);
    ~EventPage();

  protected:
    virtual void createContent();

  private slots:
    void slotEventClicked(const QModelIndex& index);

  private:
    QSqlQuery getQuery();

    PageMode m_pageMode;
    int m_artistID;
    DBManager::Storage m_dbStorage;
    QString m_country;
};

#endif // EVENTPAGE_H
