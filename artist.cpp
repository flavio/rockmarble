#include "artist.h"
#include "event.h"

Artist::Artist(const QVariant& data)
{
  QVariantMap response = data.toMap();
  response = response["events"].toMap();
  m_name = response["artist"].toString();

  QVariantList events = response["event"].toList();
  foreach(QVariant event, events) {
    m_events.push_back(new Event (event));
  }
}

Artist::~Artist()
{
  foreach(Event* event, m_events) {
    delete event;
  }
  m_events.clear();
}

