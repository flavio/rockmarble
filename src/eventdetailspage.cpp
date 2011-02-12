#include "eventdetailspage.h"

#include <MAction>
#include <MLayout>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QtGui/QGraphicsLinearLayout>

#include "dbmanager.h"
#include "event.h"
#include "location.h"

EventDetailsPage::EventDetailsPage(const int& event_id, QGraphicsItem *parent)
  : MApplicationPage(parent), m_eventID (event_id)
{
}

void EventDetailsPage::createContent()
{
  MLayout *layout = new MLayout;
  centralWidget()->setLayout(layout);

  Event* event = DBManager::instance()->eventFromID(m_eventID);
  m_starred = event->starred();
  Location* location = event->location();

  // Build a vertical layout that will hold all the informations.
  QGraphicsLinearLayout*infoLayout = new QGraphicsLinearLayout(Qt::Vertical);
//  infoLayout->addItem(new MLabel("<strong>Headliner</strong>"));
//  infoLayout->addItem(new MLabel(event.headliner()));

  infoLayout->addItem(new MLabel("<strong>Where</strong>"));
  QString locationString ("<ul>");
  locationString += QString("<li><em>Name: </em>%1</li>").arg(location->name());
  locationString += QString("<li><em>Street: </em>%1</li>").arg(location->street());
  locationString += QString("<li><em>City: </em>%1</li>").arg(location->city());
  locationString += QString("<li><em>Country: </em>%1</li>").arg(location->country());
  locationString += "</ul>";
  infoLayout->addItem(new MLabel(locationString));

  infoLayout->addItem(new MLabel("<strong>When</strong>"));
  infoLayout->addItem(new MLabel(event->dateTime().toString(Qt::TextDate)));

  infoLayout->addItem(new MLabel("<strong>Artists</strong>"));
  QString artistsString ("<ul>");
  foreach(QString artist, event->artists()){
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

  // Toolbar Actions
  m_actionStar = new MAction(this);
  m_actionStar->setLocation(MAction::ToolBarLocation);
  addAction(m_actionStar);
  connect(m_actionStar, SIGNAL(triggered()), this, SLOT(slotChangeStar()));
  updateStarredAction();

  delete event;
}

void EventDetailsPage::slotChangeStar()
{
  m_starred = !m_starred;
  DBManager::instance()->setEventStarred(m_eventID, m_starred);
  updateStarredAction();
}

void EventDetailsPage::updateStarredAction()
{
  if (m_starred) {
    m_actionStar->setIconID("icon-m-toolbar-favorite-mark");
  } else {
    m_actionStar->setIconID("icon-m-toolbar-favorite-unmark");
  }
}
