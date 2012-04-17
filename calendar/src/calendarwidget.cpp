/*
    Akonadi google calendar plasmoid - calendarwidget.cpp
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


#include "calendarwidget.h"

#include <Akonadi/Entity>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>

#include <KCalCore/Event>

#include <KDateTime>
#include <KIntSpinBox>
#include <KIcon>

CalendarWidget::CalendarWidget(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags),
      m_mainLayout(0),
      m_changeLayout(0),
      m_daysLayout(0),
      m_dateColor("#343E88"),
      m_eventBackgroundColor("#303030"),
      m_selectedDayColor("#306fb5"),
      m_currentMonthColor("#45484b"),
      m_outdatedMonthColor("#0a0b0d"),
      m_currentEventColor("#831215"),
      m_outdatedEventColor("#e64600"),
      m_spin(0),
      m_combo(0),
      m_agenda(0),
      m_firstDay(1)
{
    m_firstDay = KGlobal::locale()->weekStartDay();

    m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_daysLayout = new QGraphicsGridLayout(m_mainLayout);
    m_changeLayout = new QGraphicsLinearLayout(m_mainLayout);

    Plasma::IconWidget * previousMonth = new Plasma::IconWidget(this);
    previousMonth->setMinimumSize(20,20);
    previousMonth->setMaximumSize(20,20);
    previousMonth->setOrientation(Qt::Horizontal);
    previousMonth->setIcon(KIcon("arrow-left"));

    connect(previousMonth,SIGNAL(clicked()),SLOT(previousMonth()));

    Plasma::IconWidget * nextMonth = new Plasma::IconWidget(this);
    nextMonth->setMinimumSize(20,20);
    nextMonth->setMaximumSize(20,20);
    nextMonth->setOrientation(Qt::Horizontal);
    nextMonth->setIcon(KIcon("arrow-right"));

    connect(nextMonth,SIGNAL(clicked()),SLOT(nextMonth()));

    m_spin = new Plasma::SpinBox(this);
    m_spin->nativeWidget()->setMaximum(2099);

    connect(m_spin,SIGNAL(valueChanged(int)),SLOT(yearChanged(int)));

    m_combo = new Plasma::ComboBox(this);
    m_combo->addItem(i18n("January"));
    m_combo->addItem(i18n("February"));
    m_combo->addItem(i18n("March"));
    m_combo->addItem(i18n("April"));
    m_combo->addItem(i18n("May"));
    m_combo->addItem(i18n("June"));
    m_combo->addItem(i18n("July"));
    m_combo->addItem(i18n("August"));
    m_combo->addItem(i18n("September"));
    m_combo->addItem(i18n("October"));
    m_combo->addItem(i18n("November"));
    m_combo->addItem(i18n("December"));

    connect(m_combo,SIGNAL(currentIndexChanged(int)),SLOT(monthChanged(int)));

    m_changeLayout->addItem(previousMonth);
    m_changeLayout->addItem(m_combo);
    m_changeLayout->addItem(m_spin);
    m_changeLayout->addItem(nextMonth);

    createCalendar();

    m_agenda = new AgendaWidgetDateItem(QDate::currentDate(),this);
    m_agenda->setDateColor(m_dateColor);
    m_agenda->setBackgroundColor(m_eventBackgroundColor);

    m_scroll = new Plasma::ScrollWidget(this);
    m_scroll->setWidget(m_agenda);

    m_mainLayout->addItem(m_changeLayout);
    m_mainLayout->addItem(m_daysLayout);
    m_mainLayout->addItem(m_scroll);

    setLayout(m_mainLayout);

    setDate(QDate::currentDate());

    m_monitor = new Akonadi::Monitor();
    m_monitor->itemFetchScope().fetchFullPayload(true);

    connect(m_monitor, SIGNAL(itemAdded(Akonadi::Item, Akonadi::Collection)),
            SLOT(itemAdded(Akonadi::Item, Akonadi::Collection)));
    connect(m_monitor, SIGNAL(itemChanged(Akonadi::Item, QSet<QByteArray>)),
            SLOT(itemChanged(Akonadi::Item, QSet<QByteArray>)));
    connect(m_monitor, SIGNAL(itemRemoved(Akonadi::Item)),
            SLOT(itemRemoved(Akonadi::Item)));

}

void CalendarWidget::createCalendar()
{
    QFont fnt = font();
    fnt.setPointSize(fnt.pointSize()-3.5);

    Plasma::IconWidget * today = new Plasma::IconWidget(this);

    today->setIcon(KIcon("view-pim-calendar"));
    today->setDrawBackground(true);
    today->setOrientation(Qt::Vertical);
    today->setMinimumSize(10,10);
    today->setFont(fnt);

    connect(today,SIGNAL(clicked()),SLOT(setToday()));

    m_daysLayout->addItem(today,0,0);

    Plasma::IconWidget * weekNumber;

    for (int i = 1; i < 7; i++) {

        weekNumber = new Plasma::IconWidget(this);
        weekNumber->setAutoFillBackground(true);
        weekNumber->setOrientation(Qt::Vertical);
        weekNumber->setMinimumSize(10,10);
        weekNumber->setFont(fnt);

        m_daysLayout->addItem(weekNumber,i,0);

    }

    Plasma::IconWidget * weekDay;

    for (int i = 1; i < 8; i++) {

        weekDay = new Plasma::IconWidget(this);
        weekDay->setAutoFillBackground(true);
        weekDay->setMinimumSize(10,10);
        weekDay->setFont(fnt);

        m_daysLayout->addItem(weekDay,0,i);

    }

    CalendarWidgetDayItem * m_dayItem;

    for (int i = 1; i < 8; i++) {

        for (int j = 1; j < 7; j++) {

            m_dayItem = new CalendarWidgetDayItem(this);

            m_daysLayout->addItem(m_dayItem,j,i);
            connect(m_dayItem,SIGNAL(clicked(QDate)),SLOT(dayChanged(QDate)));

        }

    }

    QStringList days;
    days << i18n("Mon") << i18n("Tue") << i18n("Wed") << i18n("Thu");
    days << i18n("Fri") << i18n("Sat") << i18n("Sun");

    for (int i = m_firstDay; i < 8; i++) {

        weekDay = static_cast<Plasma::IconWidget*>(m_daysLayout->itemAt(0,i-m_firstDay+1));
        weekDay->setText(days.at(i-1));

    }

    for (int i = 8-m_firstDay+1; i < 8; i++) {

        weekDay = static_cast<Plasma::IconWidget*>(m_daysLayout->itemAt(0,i));
        weekDay->setText(days.at(i+m_firstDay-9));
    }

}

void CalendarWidget::setCollections(const QList< Akonadi::Entity::Id > & ids)
{
    m_agenda->setDate(m_date);

    clearEvents();

    m_idList = ids;

    if (!m_idList.isEmpty()) {

        fetchCollections();

    }

}

void CalendarWidget::setCalendarsColors(const QMap< Akonadi::Entity::Id, QString > & colors)
{
    m_calendarsColors = colors;
}

void CalendarWidget::setDateColor(const QString & color)
{
    m_dateColor = color;
    m_agenda->setDateColor(m_dateColor);
}

void CalendarWidget::setEventBackgroundColor(const QString & color)
{
    m_eventBackgroundColor = color;
    m_agenda->setBackgroundColor(m_eventBackgroundColor);
}

void CalendarWidget::setSelectedDayColor(const QString & color)
{
    m_selectedDayColor = color;
}

void CalendarWidget::setCurrentMonthColor(const QString & color)
{
    m_currentMonthColor = color;
}

void CalendarWidget::setOutdatedMonthColor(const QString & color)
{
    m_outdatedMonthColor = color;
}

void CalendarWidget::setCurrentEventColor(const QString & color)
{
    m_currentEventColor = color;
}

void CalendarWidget::setOutdatedEventColor(const QString & color)
{
    m_outdatedEventColor = color;
}

void CalendarWidget::fetchCollections()
{
    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);

    job->fetchScope();

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchCollectionsFinished(KJob *)));
}

void CalendarWidget::fetchCollectionsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob * fetchJob = qobject_cast<Akonadi::CollectionFetchJob *>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach (const Akonadi::Collection & collection, collections) {

        if (m_idList.contains(collection.id())) {

            m_monitor->setCollectionMonitored(collection);

            fetchItems(collection);

        }

    }

}

void CalendarWidget::fetchItems(const Akonadi::Collection & collection)
{
    Akonadi::ItemFetchJob * job = new Akonadi::ItemFetchJob(collection);

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchItemsFinished(KJob *)));

    job->fetchScope().fetchFullPayload(true);

}

void CalendarWidget::fetchItemsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchItems failed";

        return;
    }

    Akonadi::ItemFetchJob * fetchJob = qobject_cast<Akonadi::ItemFetchJob *>(job);

    const Akonadi::Item::List items = fetchJob->items();

    foreach (const Akonadi::Item & item, items) {

        if (item.hasPayload<KCalCore::Event::Ptr>()) {

            addItem(item);

        }

    }

}

void CalendarWidget::addItem(const Akonadi::Item & item)
{
    QDate min = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(1,1))->date();
    QDate max = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(6,7))->date();

    KCalCore::Event::Ptr event = item.payload<KCalCore::Event::Ptr>();

    QDate dateStart = event->dtStart().date();
    QDate dateEnd = event->dtEnd().date();
    QDate date = dateStart;

    int daysTo = dateStart.daysTo(dateEnd );

    if (dateStart > max) {

        return;

    } else if (dateStart < min && dateEnd < min && !event->recurs()) {

        return;

    } else if (dateStart < min && event->recurs()) {

        date = event->recurrence()->getPreviousDateTime(KDateTime(min)).date();

        if (date.addDays(daysTo) < min) {

            date = event->recurrence()->getNextDateTime(KDateTime(date)).date();

        }

        if (date.addDays(daysTo) < min || date > max) {

            return;

        }

    }

    while (date < max) {

        AgendaWidgetEventItem * eventItem;

        for (int i = 1; i < daysTo; i++) {

            if (date.addDays(i) >= min  && date.addDays(i) <= max ) {

                if (m_date == date.addDays(i)) {

                    eventItem = new AgendaWidgetEventItem(item.id(),m_agenda);
                    eventItem->setEventName(event->summary());
                    eventItem->setColor(m_calendarsColors[item.storageCollectionId()]);
                    m_agenda->addEvent(eventItem);

                }

                setColored(date.addDays(i));

            }
        }

        if (dateStart == dateEnd ) {

            if (m_date == date) {

                eventItem = new AgendaWidgetEventItem(item.id(),m_agenda);
                eventItem->setEventName(event->summary());
                eventItem->setColor(m_calendarsColors[item.storageCollectionId()]);

                if (!event->allDay()) {

                    eventItem->setEventTime(event->dtStart().time(),event->dtEnd().time());

                }

                m_agenda->addEvent(eventItem);

            }

            setColored(date);

        } else {

            if (date >= min ) {

                if (m_date == date) {

                    eventItem = new AgendaWidgetEventItem(item.id(),m_agenda);
                    eventItem->setEventName(event->summary());
                    eventItem->setColor(m_calendarsColors[item.storageCollectionId()]);

                    if (!event->allDay()) {

                        eventItem->setEventStartTime(event->dtStart().time());

                    }

                    m_agenda->addEvent(eventItem);

                }

                setColored(date);

            }

            if (dateEnd <= max ) {

                if (m_date == date.addDays(daysTo)) {

                    eventItem = new AgendaWidgetEventItem(item.id(),m_agenda);
                    eventItem->setEventName(event->summary());
                    eventItem->setColor(m_calendarsColors[item.storageCollectionId()]);

                    if (!event->allDay()) {

                        eventItem->setEventEndTime(event->dtEnd().time());

                    }

                    m_agenda->addEvent(eventItem);

                }

                setColored(date.addDays(daysTo));

            }

        }
        
        if (event->recurs()) {
	    
	    date = event->recurrence()->getNextDateTime(KDateTime(date)).date();
	    
	    if (date.isNull()) {
		
		return;
		
	    } 
	    
	} else {
		    
	    return;
	    
	}

    }

}

void CalendarWidget::setColored(const QDate & date)
{
    CalendarWidgetDayItem * dayItem;

    for (int i = 1; i < 8; i++) {

        for (int j = 1; j < 7; j++) {

            dayItem = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(j,i));

            if (dayItem->date() == date && date != m_date) {

                if (m_date.month() == date.month()) {

                    dayItem->setColor(m_currentEventColor);

                } else {

                    dayItem->setColor(m_outdatedEventColor);

                }

                dayItem->setDayWithEvent(true);

            }

        }

    }

}

void CalendarWidget::clearEvents()
{
    CalendarWidgetDayItem * dayItem;

    for (int i = 1; i < 8; i++) {

        for (int j = 1; j < 7; j++) {

            dayItem = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(j,i));

            if (dayItem->date() == m_date) {

                dayItem->setColor(m_selectedDayColor);

            } else if (dayItem->date().month() == m_date.month()) {

                dayItem->setColor(m_currentMonthColor);

            } else {

                dayItem->setColor(m_outdatedMonthColor);

            }

            dayItem->setDayWithEvent(false);

        }

    }

    m_agenda->clear();
}

void CalendarWidget::setDate(const QDate & date)
{
    if (!date.isValid()) {

        return;

    }

    m_date = date;

    m_spin->setValue(m_date.year());
    m_combo->setCurrentIndex(m_date.month()-1);

    QDate firstDate(m_date.year(),m_date.month(),1);
    int weekDay = firstDate.dayOfWeek();

    if (m_firstDay >= weekDay) {

        firstDate = firstDate.addDays(-7);

    }

    firstDate = firstDate.addDays(-weekDay+m_firstDay);

    CalendarWidgetDayItem * dayItem;
    Plasma::IconWidget * weekNumber;

    for (int week = 1; week < 7; week++) {

        weekNumber = static_cast<Plasma::IconWidget*>(m_daysLayout->itemAt(week,0));

        weekNumber->setText(QString::number(firstDate.weekNumber()));
        weekNumber->update();

        for (int day = 1; day < 8; day++) {

            dayItem = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(week,day));
            dayItem->setDay(firstDate);

            if (firstDate == m_date) {

                dayItem->setColor(m_selectedDayColor);

            } else {

                if (firstDate.month() == m_date.month()) {

                    dayItem->setColor(m_currentMonthColor);

                } else {

                    dayItem->setColor(m_outdatedMonthColor);

                }

            }

            firstDate = firstDate.addDays(1);

        }

    }

    setCollections(m_idList);
}

void CalendarWidget::setToday()
{
    setDate(QDate::currentDate());
}

void CalendarWidget::updateSize(QSizeF size)
{
    if (size.height() > 700) {

        m_scroll->setMinimumHeight(size.height()/2);

    } else if (size.height() > 500) {

        m_scroll->setMinimumHeight(size.height()/2.5);

    } else if (size.height() > 300) {

        m_scroll->setMinimumHeight(size.height()/3);

    } 
    
    QFont fontWeeks = font();

    fontWeeks.setPointSize(10);

    if (size.width() < 300) {

        fontWeeks.setPointSize(fontWeeks.pointSize()-5);

    } else if (size.width() < 400) {

        fontWeeks.setPointSize(fontWeeks.pointSize()-4);

    } else {

        fontWeeks.setPointSize(fontWeeks.pointSize()-3);

    }

    updateFontWeeks(fontWeeks);

    QFont fontDays = font();

    fontDays.setPointSize(10);

    if (size.width() < 300) {

        fontDays.setPointSize(fontDays.pointSize()-2);

    } else {

        fontDays.setPointSize(fontDays.pointSize());

    }

    updateFontDays(fontDays);
}

void CalendarWidget::updateFontWeeks(QFont font)
{
    Plasma::IconWidget * weekNumber;

    for (int i = 1; i < 7; i++) {

        weekNumber = static_cast<Plasma::IconWidget*>(m_daysLayout->itemAt(i,0));
        weekNumber->setMinimumSize(10,10);
        weekNumber->setFont(font);

    }

    Plasma::IconWidget * weekDay;

    for (int i = 1; i < 8; i++) {

        weekDay = static_cast<Plasma::IconWidget*>(m_daysLayout->itemAt(0,i));
        weekDay->setFont(font);

    }

}

void CalendarWidget::updateFontDays(QFont font)
{
    CalendarWidgetDayItem * m_dayItem;

    for (int i = 1; i < 8; i++) {

        for (int j = 1; j < 7; j++) {

            m_dayItem = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(j,i));
            m_dayItem->setFont(font);

        }

    }

}

void CalendarWidget::previousMonth()
{
    setDate(m_date.addMonths(-1));
}

void CalendarWidget::nextMonth()
{
    setDate(m_date.addMonths(1));
}

void CalendarWidget::dayChanged(const QDate & date)
{
    if (date.month() != m_date.month()) {

        setDate(date);

    } else {

        CalendarWidgetDayItem * m_dayItem;

        for (int i = 1; i < 8; i++) {

            for (int j = 1; j < 7; j++) {

                m_dayItem = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(j,i));

                if (m_dayItem->date() == m_date) {

                    if (m_dayItem->hasEvent()) {

                        m_dayItem->setColor(m_currentEventColor);

                    } else {

                        m_dayItem->setColor(m_currentMonthColor);

                    }

                } else if (m_dayItem->date() == date) {

                    m_dayItem->setColor(m_selectedDayColor);

                }

            }

        }

        m_date = date;
        m_agenda->setDate(m_date);
        m_agenda->clear();

        fetchCollections();

    }

}

void CalendarWidget::yearChanged(const int & year)
{
    if (year == m_date.year()) {

        return;

    }

    QDate dt(year,m_date.month(),m_date.day());

    if (dt.isValid()) {

        setDate(dt);

    } else {

        dt.setDate(year,m_date.month(),1);
        setDate(dt);

    }

}

void CalendarWidget::monthChanged(const int & month)
{
    if (month+1 == m_date.month()) {

        return;

    }

    QDate dt(m_date.year(),month+1,m_date.day());

    if (dt.isValid()) {

        setDate(dt);

    } else {

        dt.setDate(m_date.year(),month+1,1);
        setDate(dt);

    }

}

void CalendarWidget::itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection)
{
    if (!item.hasPayload<KCalCore::Event::Ptr>()) {

        return;

    }

    if (m_idList.contains(collection.id())) {

        addItem(item);

    }

}

void CalendarWidget::itemChanged(const Akonadi::Item & item, QSet< QByteArray > array)
{
    Q_UNUSED(array);
    Q_UNUSED(item);

    if (!item.hasPayload<KCalCore::Event::Ptr>()) {

        return;

    }

    setCollections(m_idList);

}

void CalendarWidget::itemRemoved(const Akonadi::Item & item)
{
    Q_UNUSED(item);

    if (!item.hasPayload<KCalCore::Event::Ptr>()) {

        return;

    }

    if (m_idList.contains(item.parentCollection().id())) {

        setCollections(m_idList);

    }

}
