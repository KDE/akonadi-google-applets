/*
    Copyright (C) 2012  Jan Grulich <jgrulich@redhat.com>

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

#include <Plasma/Containment>
#include <Plasma/PushButton>
#include <Plasma/IconWidget>

PlasmaCalendar::PlasmaCalendar(QObject * parent, const QVariantList & args):
    Plasma::PopupApplet(parent, args),
    agendaConfigDialog(0),
    calendarConfigDialog(0),
    m_widget(0),
    m_layout(0),
    m_agenda(0),
    m_calendar(0),
    m_clock(0),
    m_scroll(0),
    m_tab(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setPopupIcon(QIcon());
}

void PlasmaCalendar::init()
{
    m_engine = dataEngine("time");
    m_engine->connectSource("Local", this, 60000, Plasma::AlignToMinute);

    m_widget = new QGraphicsWidget(this);

    m_agenda = new AgendaWidget(m_widget);
    m_calendar = new CalendarWidget(m_widget);

    Plasma::PushButton * m_button = new Plasma::PushButton(m_widget);
    m_button->setMaximumHeight(25);
    m_button->setText(i18n("Add event"));

    connect(m_button, SIGNAL(clicked()), SLOT(createEvent()));

    m_layout = new QGraphicsLinearLayout(Qt::Vertical);

    m_scroll = new Plasma::ScrollWidget(m_widget);
    m_scroll->setWidget(m_agenda);

    m_tab = new Plasma::TabBar(m_widget);
    m_tab->addTab(i18n("Agenda"), m_scroll);
    m_tab->addTab(i18n("Calendar"), m_calendar);
    m_tab->setTabBarShown(true);

    connect(m_tab, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

    m_layout->addItem(m_tab);
    m_layout->addItem(m_button);

    Plasma::DataEngine::Data data = m_engine->query("Local");
    m_lastTime = data["Time"].toTime();
    m_lastDate = data["Date"].toDate();

    if (containment()->containmentType() == Plasma::Containment::DesktopContainment) {
	setLayout(m_layout);
	setMinimumSize(300,500);
    } else {
	m_widget->setPreferredSize(300, 500);
	m_widget->setMinimumSize(200,300);
	m_widget->setLayout(m_layout);

	setGraphicsWidget(m_widget);

	m_clock = new ClockWidget(this);
	
	m_clock->updateClock(m_lastTime, m_lastDate);

	QGraphicsLinearLayout * layout = new QGraphicsLinearLayout(this);
	layout->addItem(m_clock);
	layout->setContentsMargins(0, 0, 0, 2);
	layout->setSpacing(0);
	setLayout(layout);
    }

    configChanged();
}

void PlasmaCalendar::configChanged()
{
    KConfigGroup conf = config();

    QMap<Akonadi::Entity::Id, QString> map;

    QList<Akonadi::Item::Id> list = conf.readEntry("collections", QList<Akonadi::Item::Id>());

    m_agenda->setDateColor(conf.readEntry("dateColor", "#343E88"));
    m_agenda->setUpcomingDateColor(conf.readEntry("upcomingDateColor", "#C00000"));
    m_agenda->setEventBackgroundColor(conf.readEntry("eventBackgroundColor", "#303030"));
    m_agenda->setWeeks(conf.readEntry("weeks", 1));
    m_agenda->setUpcomingDays(conf.readEntry("upcomingDays", 3));

    if (list.isEmpty())
        setConfigurationRequired(true);
    else
        setConfigurationRequired(false);

    m_agenda->setCollections(list);

    foreach(Akonadi::Entity::Id id, m_agenda->collections()) {
        map.insert(id, conf.readEntry(QString::number(id), "#00C000"));
    }

    m_agenda->setCalendarsColors(map);

    m_calendar->setCollections(list);
    m_calendar->setDateColor(conf.readEntry("dateColor", "#343E88"));
    m_calendar->setActualDayColor(conf.readEntry("actualDayColor", "#16ae16"));
    m_calendar->setEventBackgroundColor(conf.readEntry("eventBackgroundColor", "#303030"));
    m_calendar->setSelectedDayColor(conf.readEntry("selectedDayColor", "#306fb5"));
    m_calendar->setCurrentMonthColor(conf.readEntry("currentMonthColor", "#45484b"));
    m_calendar->setOutdatedMonthColor(conf.readEntry("outdatedMonthColor", "#0a0b0d"));
    m_calendar->setCurrentEventColor(conf.readEntry("currentEventColor", "#831215"));
    m_calendar->setOutdatedEventColor(conf.readEntry("outdatedEventColor", "#e64600"));
    m_calendar->setCalendarsColors(map);
    m_calendar->setAgendaPosition((CalendarWidget::AgendaPosition)conf.readEntry("agendaPosition", 2));

    if (((CalendarWidget::AgendaPosition)conf.readEntry("agendaPosition", 2)) == 1) {
	if (m_clock) {
	    if (m_widget->size().width() < 400) {
		m_widget->setPreferredSize(600, 500);
	    }
	    m_widget->setMinimumSize(400, 300);
	} else {
	    if (size().width() < 400) {
		resize(600,500);
	    }
	    setMinimumSize(400,300);
	}
    } else {
	if (m_clock) {
	    if (m_widget->size().width() < 200) {
		m_widget->setPreferredSize(300, 500);
	    }
	    m_widget->setMinimumSize(200, 400);
	} else {
	    if (size().width() < 200) {
		resize(300,500);
	    }
	    setMinimumSize(200,400);
	}
    }

    m_tab->setCurrentIndex(conf.readEntry("defaultView", 0));

    if (m_clock) {
	m_clock->setDateFormat(conf.readEntry("clockDateFormat", 0));
	m_clock->updateSize(size().toSize(), formFactor());
	m_clock->setFontColor(conf.readEntry("clockFontColor", "none"));
    }
}

void PlasmaCalendar::createConfigurationInterface(KConfigDialog * parent)
{
    QWidget * widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    fetchCollections();

    configDialog.loadCollections->setIcon(KIcon("view-refresh"));
    configDialog.view->setCurrentIndex(conf.readEntry("defaultView", 0));

    connect(configDialog.collectionsList, SIGNAL(clicked(QModelIndex)), parent, SLOT(settingsModified()));
    connect(configDialog.loadCollections, SIGNAL(clicked(bool)), SLOT(fetchCollections()));
    connect(configDialog.view, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));

    parent->addPage(widget, i18n("General"), icon());

    agendaConfigDialog = new AgendaConfig(0);

    agendaConfigDialog->setCalendarsColors(m_agenda->calendarsColors());
    agendaConfigDialog->setDateColor(QColor(m_agenda->dateColor()));
    agendaConfigDialog->setUpcomingColor(QColor(m_agenda->upcomingDateColor()));
    agendaConfigDialog->setEventBackgroundColor(QColor(m_agenda->eventBackgroundColor()));
    agendaConfigDialog->setWeeks(m_agenda->weeks() - 1);
    agendaConfigDialog->setUpcomingDays(m_agenda->upcomingDays());

    parent->addPage(agendaConfigDialog, i18n("Agenda"), "view-calendar-agenda");

    connect(agendaConfigDialog, SIGNAL(updateCalendars()), SLOT(updateCalendars()));

    calendarConfigDialog = new CalendarConfig(0);

    calendarConfigDialog->setActualDayColor(m_calendar->actualDayColor());
    calendarConfigDialog->setSelectedDayColor(m_calendar->selectedDayColor());
    calendarConfigDialog->setCurrentMonthColor(m_calendar->currentMonthColor());
    calendarConfigDialog->setOutdatedMonthColor(m_calendar->outdatedMonthColor());
    calendarConfigDialog->setCurrentEventColor(m_calendar->currentEventColor());
    calendarConfigDialog->setOutdatedEventColor(m_calendar->outdatedEventColor());
    calendarConfigDialog->setAgendaPosition(m_calendar->agendaPosition());

    parent->addPage(calendarConfigDialog, i18n("Calendar"), "view-calendar-month");

    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(agendaConfigDialog, SIGNAL(changed()), parent, SLOT(settingsModified()));
    connect(calendarConfigDialog, SIGNAL(changed()), parent, SLOT(settingsModified()));

    if (m_clock) {
	QWidget * clockWidget = new QWidget(0);

	clockConfigDialog.setupUi(clockWidget);
	parent->addPage(clockWidget, i18n("Clock"), "preferences-system-time");

	clockConfigDialog.dateFormat->setCurrentIndex(m_clock->dateFormat());
	if (m_clock->fontColor() == "none") {
	    clockConfigDialog.customFontColor->setChecked(false);
	} else {
	    clockConfigDialog.customFontColor->setChecked(true);
	    clockConfigDialog.fontColor->setColor(QColor(m_clock->fontColor()));
	}
	
	connect(clockConfigDialog.dateFormat, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));
	connect(clockConfigDialog.fontColor, SIGNAL(changed(QColor)), parent, SLOT(settingsModified()));
	connect(clockConfigDialog.customFontColor, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
    }
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

    if (configDialog.view->currentIndex() != m_tab->currentIndex())
	conf.writeEntry("defaultView", configDialog.view->currentIndex());

    if (agendaConfigDialog->dateColor() != m_agenda->dateColor())
        conf.writeEntry("dateColor", agendaConfigDialog->dateColor());


    if (agendaConfigDialog->upcomingDateColor() != m_agenda->upcomingDateColor())
        conf.writeEntry("upcomingDateColor", agendaConfigDialog->upcomingDateColor());


    if (agendaConfigDialog->eventBackgroundColor() != m_agenda->eventBackgroundColor())
        conf.writeEntry("eventBackgroundColor", agendaConfigDialog->eventBackgroundColor());


    if (agendaConfigDialog->weeks() != m_agenda->weeks())
        conf.writeEntry("weeks", agendaConfigDialog->weeks());


    if (agendaConfigDialog->upcomingDays() != m_agenda->upcomingDays())
        conf.writeEntry("upcomingDays", agendaConfigDialog->upcomingDays());



    conf.writeEntry("collections", list);

    foreach(Akonadi::Entity::Id id, agendaConfigDialog->calendarsColors().keys()) {
        conf.writeEntry(QString::number(id), agendaConfigDialog->calendarsColors()[id]);
    }

    if (calendarConfigDialog->actualDayColor() != m_calendar->actualDayColor())
        conf.writeEntry("actualDayColor", calendarConfigDialog->actualDayColor());

    if (calendarConfigDialog->selectedDayColor() != m_calendar->selectedDayColor())
        conf.writeEntry("selectedDayColor", calendarConfigDialog->selectedDayColor());


    if (calendarConfigDialog->currentMonthColor() != m_calendar->currentMonthColor())
        conf.writeEntry("currentMonthColor", calendarConfigDialog->currentMonthColor());


    if (calendarConfigDialog->outdatedMonthColor() != m_calendar->outdatedMonthColor())
        conf.writeEntry("outdatedMonthColor", calendarConfigDialog->outdatedMonthColor());


    if (calendarConfigDialog->currentEventColor() != m_calendar->currentEventColor())
        conf.writeEntry("currentEventColor", calendarConfigDialog->currentEventColor());


    if (calendarConfigDialog->outdatedEventColor() != m_calendar->outdatedEventColor())
        conf.writeEntry("outdatedEventColor", calendarConfigDialog->outdatedEventColor());

    if (calendarConfigDialog->agendaPosition() != m_calendar->agendaPosition())
	conf.writeEntry("agendaPosition", (int)calendarConfigDialog->agendaPosition());

    if (m_clock) {
	if (clockConfigDialog.dateFormat->currentIndex() != m_clock->dateFormat())
	    conf.writeEntry("clockDateFormat", clockConfigDialog.dateFormat->currentIndex());
	if (clockConfigDialog.customFontColor->isChecked()) {
	    conf.writeEntry("clockFontColor", clockConfigDialog.fontColor->color().name());
	} else {
	    conf.writeEntry("clockFontColor", "none");
	}
    }

    emit configNeedsSaving();
}

void PlasmaCalendar::constraintsEvent(Plasma::Constraints constraints)
{
    if (m_widget) {

        if (constraints & Plasma::FormFactorConstraint) {

            if (formFactor() == Plasma::Horizontal) {
                connect(m_widget, SIGNAL(geometryChanged()), SLOT(widgetGeometryChanged()));
            } else {
                m_widget->disconnect(SIGNAL(geometryChanged()));
            }
        }
    }

    if (constraints & Plasma::SizeConstraint || constraints & Plasma::FormFactorConstraint) {
	if (m_clock)
	    m_calendar->updateSize(m_widget->size());
	else
	    m_calendar->updateSize(size());
	
	if (m_clock) {
	    m_clock->updateSize(size().toSize(), formFactor());
	}
    }
}

void PlasmaCalendar::widgetGeometryChanged()
{
    if (m_clock)
	m_calendar->updateSize(m_widget->size());
    else
	m_calendar->updateSize(size());
}

void PlasmaCalendar::fetchCollections()
{
    while (configDialog.collectionsList->count() != 0) {
        delete configDialog.collectionsList->item(0);
    }

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(),
            Akonadi::CollectionFetchJob::Recursive, this);

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

    foreach(const Akonadi::Collection & collection, collections) {

        /* Ignore "Last Search" collections */
        if (collection.resource() == QLatin1String("akonadi_search_resource")) {
            continue;
        }

        if (collection.contentMimeTypes().contains(KCalCore::Event::eventMimeType())) {
            Akonadi::EntityDisplayAttribute * attribute = collection.attribute< Akonadi::EntityDisplayAttribute > ();

            QListWidgetItem * item = new QListWidgetItem();

            QString name;

            if (collections.contains(collection.parentCollection())) {
                Akonadi::Collection col = collections.at(collections.indexOf(collection.parentCollection()));
                Akonadi::EntityDisplayAttribute * attr = col.attribute< Akonadi::EntityDisplayAttribute > ();

		if (!attribute || attribute->displayName().isEmpty()) {
                    name = col.name();
                } else {
                    name = attr->displayName();
                }

                name += " / ";
            }

            if (!attribute || attribute->displayName().isEmpty()) {
                name += collection.name();
            } else {
                name += attribute->displayName();
            }

            item->setText(name);

            item->setData(Qt::UserRole, collection.id());
            item->setCheckState(Qt::Unchecked);

            configDialog.collectionsList->insertItem(configDialog.collectionsList->count(), item);
        }
    }

    if (!m_agenda->collections().isEmpty()) {

        for (int i = 0; i < m_agenda->collections().count(); i++) {

            for (int j = 0; j < configDialog.collectionsList->count(); j++) {

                if (m_agenda->collections().at(i) == configDialog.collectionsList->item(j)->data(Qt::UserRole).toInt()) {
                    configDialog.collectionsList->item(j)->setCheckState(Qt::Checked);
                }
            }
        }
    }
    updateCalendars();
}

void PlasmaCalendar::createEvent()
{
    KRun::runCommand("kincidenceeditor --new-event", 0);
}

void PlasmaCalendar::dataUpdated(const QString& name, const Plasma::DataEngine::Data& data)
{
    Q_UNUSED(name);

    m_lastTime = data["Time"].toTime();

    if (m_lastDate != data["Date"].toDate()) {
	m_lastDate = data["Date"].toDate();
	if (m_clock) {
	    m_clock->updateClock(m_lastTime, m_lastDate);
	}
	if (m_calendar) {
	    m_calendar->setDate(m_lastDate);
	}
	if (m_agenda) {
	    m_agenda->setDate(m_lastDate);
	}
    } else {
	if (m_clock) {
	    m_clock->updateClock(m_lastTime);
	}
    }
}

#include "plasmacalendar.moc"
