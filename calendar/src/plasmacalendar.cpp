/*
    Akonadi google calendar plasmoid - plasmacalendar.cpp
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "plasmacalendar.h"

#include <QListWidgetItem>

#include <KRun>
#include <KConfigDialog>

#include <Akonadi/EntityDisplayAttribute>

#include <Plasma/PushButton>
#include <Plasma/IconWidget>

PlasmaCalendar::PlasmaCalendar(QObject * parent, const QVariantList & args)
    : Plasma::PopupApplet(parent, args),
      m_widget(0),
      m_agenda(0)
{
    setConfigurationRequired(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setPopupIcon(icon());
}

void PlasmaCalendar::init()
{
    configChanged();
}

QGraphicsWidget * PlasmaCalendar::graphicsWidget()
{
     if (!m_widget) {
	 
	m_agenda = new AgendaWidget(this);
	m_calendar = new CalendarWidget(this);
        
        Plasma::PushButton * m_button = new Plasma::PushButton(this);
        m_button->setMaximumHeight(25);
        m_button->setText(i18n("Add event"));
        
        connect(m_button,SIGNAL(clicked()),SLOT(createEvent()));
        
        m_layout = new QGraphicsLinearLayout(Qt::Vertical); 

	m_scroll = new Plasma::ScrollWidget(this);
	m_scroll->setWidget(m_agenda);
	
	m_tab = new Plasma::TabBar(this);
	m_tab->addTab(i18n("Agenda"),m_scroll);
	m_tab->addTab(i18n("Calendar"),m_calendar);
        m_tab->setTabBarShown(true);
	
        m_layout->addItem(m_tab);
        m_layout->addItem(m_button);
	
        m_widget = new QGraphicsWidget(this);
	m_widget->setMinimumSize(200,300);
        m_widget->setPreferredSize(300, 500);
	m_widget->setLayout(m_layout);
	
	//connect(m_widget,SIGNAL(geometryChanged()),SLOT(widgetGeometryChanged()));
	
    }

    return m_widget;
    
}

void PlasmaCalendar::configChanged()
{
    KConfigGroup conf = config();
    
    QMap<Akonadi::Entity::Id,QString> map;
    
    QList<Akonadi::Item::Id> list = conf.readEntry("collections", QList<Akonadi::Item::Id>());

    m_agenda->setDateColor(conf.readEntry("dateColor","#343E88"));
    m_agenda->setUpcomingDateColor(conf.readEntry("upcomingDateColor","#C00000"));
    m_agenda->setEventBackgroundColor(conf.readEntry("eventBackgroundColor","#303030"));
    m_agenda->setWeeks(conf.readEntry("weeks",1));
    m_agenda->setUpcomingDays(conf.readEntry("upcomingDays",3));
    
    if (list.isEmpty()) {

        setConfigurationRequired(true);

    } else {

        setConfigurationRequired(false);

    }

    m_agenda->setCollections(list);
    
    foreach (Akonadi::Entity::Id id, m_agenda->collectionsList()) {
        
        map.insert(id,conf.readEntry(QString::number(id),"#00C000"));
        
    }
    
    m_agenda->setCalendarsColors(map);
    
    
    m_calendar->setDateColor(conf.readEntry("dateColor","#343E88"));
    m_calendar->setEventBackgroundColor(conf.readEntry("eventBackgroundColor","#303030"));
    m_calendar->setSelectedDayColor(conf.readEntry("selectedDayColor","#306fb5"));
    m_calendar->setCurrentMonthColor(conf.readEntry("currentMonthColor","#45484b"));
    m_calendar->setOutdatedMonthColor(conf.readEntry("outdatedMonthColor","#0a0b0d"));
    m_calendar->setCurrentEventColor(conf.readEntry("currentEventColor","#831215"));
    m_calendar->setOutdatedEventColor(conf.readEntry("outdatedEventColor","#e64600"));
    m_calendar->setCalendarsColors(map);
    m_calendar->setCollections(list);
    
}

void PlasmaCalendar::createConfigurationInterface(KConfigDialog * parent)
{ 
    QWidget * widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    fetchCollections();
    
    configDialog.loadCollections->setIcon(KIcon("view-refresh"));

    connect(configDialog.collectionsList,SIGNAL(clicked(QModelIndex)),parent,SLOT(settingsModified()));
    connect(configDialog.loadCollections, SIGNAL(clicked(bool)), SLOT(fetchCollections()));

    parent->addPage(widget, i18n("General"), icon());
    
    agendaConfigDialog = new AgendaConfig(0);
    
    agendaConfigDialog->setCalendarsColors(m_agenda->calendarsColors());
    agendaConfigDialog->setDateColor(QColor(m_agenda->dateColor()));
    agendaConfigDialog->setUpcomingColor(QColor(m_agenda->upcomingDateColor()));
    agendaConfigDialog->setEventBackgroundColor(QColor(m_agenda->eventBackgroundColor()));
    agendaConfigDialog->setWeeks(m_agenda->weeks()-1);
    agendaConfigDialog->setUpcomingDays(m_agenda->upcomingDays());
    
    parent->addPage(agendaConfigDialog, i18n("Agenda"), "view-calendar-agenda");

    connect(agendaConfigDialog, SIGNAL(updateCalendars()), SLOT(updateCalendars()));
    
    calendarConfigDialog = new CalendarConfig(0);
        
    calendarConfigDialog->setSelectedDayColor(m_calendar->selectedDayColor());
    calendarConfigDialog->setCurrentMonthColor(m_calendar->currentMonthColor());
    calendarConfigDialog->setOutdatedMonthColor(m_calendar->outdatedMonthColor());
    calendarConfigDialog->setCurrentEventColor(m_calendar->currentEventColor());
    calendarConfigDialog->setOutdatedEventColor(m_calendar->outdatedEventColor());
    
    parent->addPage(calendarConfigDialog,i18n("Calendar"),"view-calendar-month");
    
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));  
    connect(agendaConfigDialog,SIGNAL(changed()),parent,SLOT(settingsModified()));
    connect(calendarConfigDialog,SIGNAL(changed()),parent,SLOT(settingsModified()));
    
}

void PlasmaCalendar::configAccepted()
{
    KConfigGroup conf = config();

    QList<Akonadi::Item::Id> list;

    for (int i = 0; i < configDialog.collectionsList->count(); i++) {

        if (configDialog.collectionsList->item(i)->checkState()) {

            list.push_back(configDialog.collectionsList->item(i)->data(Qt::UserRole).toInt());

        }

    }

    if (agendaConfigDialog->dateColor() != m_agenda->dateColor()) {

        conf.writeEntry("dateColor", agendaConfigDialog->dateColor());
    }
    
    if (agendaConfigDialog->upcomingDateColor() != m_agenda->upcomingDateColor()) {
        
        conf.writeEntry("upcomingDateColor", agendaConfigDialog->upcomingDateColor());
    }
    
    if (agendaConfigDialog->eventBackgroundColor() != m_agenda->eventBackgroundColor()) {
        
        conf.writeEntry("eventBackgroundColor", agendaConfigDialog->eventBackgroundColor());
    }
    
    if (agendaConfigDialog->weeks() != m_agenda->weeks()) {

        conf.writeEntry("weeks", agendaConfigDialog->weeks());
    }
    
    if (agendaConfigDialog->upcomingDays() != m_agenda->upcomingDays()) {
     
        conf.writeEntry("upcomingDays", agendaConfigDialog->upcomingDays());
        
    }
    
    conf.writeEntry("collections", list);
    
    foreach (Akonadi::Entity::Id id, agendaConfigDialog->calendarsColors().keys()) {
        
        conf.writeEntry(QString::number(id),agendaConfigDialog->calendarsColors()[id]);
        
    }
    
    if (calendarConfigDialog->selectedDayColor() != m_calendar->selectedDayColor()) {
        
        conf.writeEntry("selectedDayColor",calendarConfigDialog->selectedDayColor());
        
    }
    
    if (calendarConfigDialog->currentMonthColor() != m_calendar->currentMonthColor()) {
        
        conf.writeEntry("currentMonthColor",calendarConfigDialog->currentMonthColor());
        
    }
    
    if (calendarConfigDialog->outdatedMonthColor() != m_calendar->outdatedMonthColor()) {
        
        conf.writeEntry("outdatedMonthColor",calendarConfigDialog->outdatedMonthColor());
        
    }
    
    if (calendarConfigDialog->currentEventColor() != m_calendar->currentEventColor()) {
        
        conf.writeEntry("currentEventColor",calendarConfigDialog->currentEventColor());
        
    }
    
    if (calendarConfigDialog->outdatedEventColor() != m_calendar->outdatedEventColor()) {
        
        conf.writeEntry("outdatedEventColor",calendarConfigDialog->outdatedEventColor());
        
    }
    
    emit configNeedsSaving();
}

void PlasmaCalendar::constraintsEvent(Plasma::Constraints constraints)
{    
    if (m_widget) {
	
	if (constraints & Plasma::FormFactorConstraint) {
	    
	    if (formFactor() == 2) {
		
		connect(m_widget,SIGNAL(geometryChanged()),SLOT(widgetGeometryChanged()));

	    } else {
		
		m_widget->disconnect(SIGNAL(geometryChanged()));
		
	    }
	    
	}
	
    }
        
    if (constraints & Plasma::SizeConstraint) {
        
        m_calendar->updateSize(size());        
        
    }
    
}

void PlasmaCalendar::widgetGeometryChanged()
{
    m_calendar->updateSize(m_widget->size()); 
}


void PlasmaCalendar::fetchCollections()
{
    while (configDialog.collectionsList->count() != 0) {

        delete configDialog.collectionsList->item(0);

    }

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);

    job->fetchScope();

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchCollectionsFinished(KJob *)));
}

void PlasmaCalendar::updateCalendars()
{
    agendaConfigDialog->clear();
    
    for (int i = 0; i < configDialog.collectionsList->count(); i++) {
        
        if (configDialog.collectionsList->item(i)->checkState() == Qt::Checked) {
            
            agendaConfigDialog->addItem(configDialog.collectionsList->item(i)->text(), 
                                        configDialog.collectionsList->item(i)->data(Qt::UserRole).toInt());
            
        }
        
    }
    
}

void PlasmaCalendar::fetchCollectionsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob * fetchJob = qobject_cast<Akonadi::CollectionFetchJob *> (job);
    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach (const Akonadi::Collection & collection, collections) {

#ifndef ALL_COLLECTIONS
        if (collection.resource().contains("akonadi_googlecalendar_resource")) {
#endif
            if (collection.contentMimeTypes().contains(KCalCore::Event::eventMimeType())) {

                Akonadi::EntityDisplayAttribute * attribute = collection.attribute< Akonadi::EntityDisplayAttribute > ();
	    
                QListWidgetItem * item = new QListWidgetItem();
	        
	        QString name;

	        if (collections.contains(collection.parentCollection())) {
		 
		    Akonadi::Collection col = collections.at(collections.indexOf(collection.parentCollection()));
		    Akonadi::EntityDisplayAttribute * attr = col.attribute< Akonadi::EntityDisplayAttribute > ();
		    
		    if (!attribute) {
			
			name = col.name();
			
		    } else {
			
			name = attr->displayName();
			
		    }
		    
		    name += " / ";
		}
	     
                if (!attribute) {

		    name += collection.name();
		    
                } else {

                   name += attribute->displayName();

		}
                
                item->setText(name);

                item->setData(Qt::UserRole, collection.id());
                item->setCheckState(Qt::Unchecked);

                configDialog.collectionsList->insertItem(configDialog.collectionsList->count(), item);

            }
#ifndef ALL_COLLECTIONS
        }
#endif
    }

    if (!m_agenda->collectionsList().isEmpty()) {

        for (int i = 0; i < m_agenda->collectionsList().count(); i++) {

            for (int j = 0; j < configDialog.collectionsList->count(); j++) {

                if (m_agenda->collectionsList().at(i) == configDialog.collectionsList->item(j)->data(Qt::UserRole).toInt()) {

                    configDialog.collectionsList->item(j)->setCheckState(Qt::Checked);

                }

            }

        }

    }
    
    updateCalendars();
}
 
void PlasmaCalendar::createEvent()
{
    KRun::runCommand("kincidenceeditor --new-event",0);
} 
 
#include "plasmacalendar.moc"

