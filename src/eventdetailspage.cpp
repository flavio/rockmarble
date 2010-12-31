#include "eventdetailspage.h"

#include "event.h"
#include "location.h"

#include <MLayout>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QtGui/QGraphicsLinearLayout>

EventDetailsPage::EventDetailsPage(Event* event, QGraphicsItem *parent)
  : MApplicationPage(parent), m_event (event)
{
}

void EventDetailsPage::createContent()
{
  MLayout *layout = new MLayout;
  centralWidget()->setLayout(layout);

  Location* location = m_event->location();

  // Build a vertical layout that will hold all the informations.
  QGraphicsLinearLayout*infoLayout = new QGraphicsLinearLayout(Qt::Vertical);
  infoLayout->addItem(new MLabel("<strong>Headliner</strong>"));
  infoLayout->addItem(new MLabel(m_event->headliner()));

  infoLayout->addItem(new MLabel("<strong>Where</strong>"));
  QString locationString ("<ul>");
  locationString += QString("<li><em>Name: </em>%1</li>").arg(location->name());
  locationString += QString("<li><em>Street: </em>%1</li>").arg(location->street());
  locationString += QString("<li><em>City: </em>%1</li>").arg(location->city());
  locationString += QString("<li><em>Country: </em>%1</li>").arg(location->country());
  locationString += "</ul>";
  infoLayout->addItem(new MLabel(locationString));

  infoLayout->addItem(new MLabel("<strong>When</strong>"));
  infoLayout->addItem(new MLabel(m_event->dateTime().toString(Qt::TextDate)));

  infoLayout->addItem(new MLabel("<strong>Artists</strong>"));
  QString artistsString ("<ul>");
  foreach(QString artist, m_event->artists()){
    artistsString += QString("<li>%1</li>").arg(artist);
  }
  artistsString += "</ul>";
  infoLayout->addItem(new MLabel(artistsString));

  // Landscape orientation
  MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
  landscapePolicy->addItem(infoLayout);
  layout->setLandscapePolicy(landscapePolicy);

  // Portrait orientation
  MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
  portraitPolicy->addItem(infoLayout);
  layout->setPortraitPolicy(portraitPolicy);
}
