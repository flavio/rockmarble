#include "event.h"
#include "location.h"

Event::Event(const QVariant& data)
{
  QVariantMap event = data.toMap();

  m_id = event["id"].toInt();

  QVariantMap artists = event["artists"].toMap();
  m_artist = artists["artist"].toString();
  m_headliner = artists["headliner"].toString();

  m_title = event["title"].toString();
  m_date = QDate::fromString(event["startDate"].toString(), "ddd, dd MMM yyyy");
  m_time = QTime::fromString(event["startTime"].toString(), "hh:mm");

//  qDebug() << "start date" << event["startDate"] << "-->" << m_date;
//  qDebug() << "start time" << event["startTime"] << "-->" << m_time;

  m_location = new Location (event["venue"]);
}

Event::~Event() {
  delete m_location;
  m_location = 0;
}

