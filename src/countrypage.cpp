#include "countrypage.h"
#include "countryitemcreator.h"
#include "event.h"
#include "eventpage.h"
#include "location.h"

#include <MLayout>
#include <MList>
#include <MLinearLayoutPolicy>

#include <QtGui/QStringListModel>

CountryPage::CountryPage(const QString& artist, const EventList& events, QGraphicsItem *parent)
  : MApplicationPage(parent)
{
  setTitle(artist);
  foreach(Event* event,events) {
    QString country = event->location()->country();
    m_events.insertMulti(country, event);
  }
}

CountryPage::~CountryPage()
{
}

void CountryPage::createContent()
{
  QStringListModel* countryModel = new QStringListModel();

  QStringList countries = m_events.uniqueKeys();
  qSort(countries);
  countryModel->setStringList(countries);

  QGraphicsWidget *panel = centralWidget();
  MLayout *layout = new MLayout(panel);
  layout->setAnimation(NULL);
  panel->setLayout(layout);
  MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

  MList* countryList = new MList();
  countryList->setSelectionMode(MList::SingleSelection);

  CountryItemCreator *cellCreator = new CountryItemCreator();
  countryList->setCellCreator(cellCreator);
  countryList->setItemModel(countryModel);
  policy->addItem(countryList);

  connect (countryList, SIGNAL(itemClicked(QModelIndex)),
           this, SLOT(slotCountryClicked(QModelIndex)));
}

void CountryPage::slotCountryClicked(const QModelIndex& index)
{
  QString country= index.data(Qt::DisplayRole).toString();
  EventPage *eventPage = new EventPage(country, m_events.values(country));
  eventPage->appear(MSceneWindow::DestroyWhenDismissed);
}
