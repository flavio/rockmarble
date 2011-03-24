#ifndef NEARLOCATIONPAGE_H
#define NEARLOCATIONPAGE_H

#include <MApplicationPage>

#include <QGeoPositionInfo>

class MButton;
class MComboBox;

using namespace QtMobility;

class NearLocationSearchPage : public MApplicationPage
{
  Q_OBJECT
  public:
    NearLocationSearchPage(QGraphicsItem *parent = 0);
    ~NearLocationSearchPage();

  protected:
    void createContent();

  private slots:
    void slotSearchEvents();
    void updatePosition(const QGeoPositionInfo& pos);
    void updatePositionTimeout();

  private:
    void showMessage(const QString &message, bool error);

    QGeoPositionInfo m_position;
    MButton* m_btnSearchEvents;
    bool m_trackingLocation;
    MComboBox* m_kmBox;
};

#endif // NEARLOCATIONPAGE_H
