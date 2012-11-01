/*
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


#ifndef PLASMA_CALENDAR_HEADER
#define PLASMA_CALENDAR_HEADER

#include <Plasma/PopupApplet>
#include <Plasma/ScrollWidget>
#include <Plasma/TabBar>
#include <Plasma/DataEngine>

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>

#include "agendawidget.h"
#include "calendarwidget.h"
#include "clockwidget.h"
#include "agendaconfig.h"
#include "calendarconfig.h"
#include "ui_config.h"
#include "ui_clockconfig.h"

class PlasmaCalendar : public Plasma::PopupApplet
{
    Q_OBJECT

public:
    PlasmaCalendar(QObject *parent, const QVariantList &args);
    ~PlasmaCalendar() {};

    void init();
    void createConfigurationInterface(KConfigDialog * parent);

public slots:
    void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
    void configAccepted();
    void createEvent();
    void fetchCollections();
    void fetchCollectionsFinished(KJob * job);
    void updateCalendars();
    void widgetGeometryChanged();

protected:
    void constraintsEvent(Plasma::Constraints constraints);

private:
    void configChanged();

    Ui::config configDialog;
    Ui::clockconfig clockConfigDialog;

    AgendaConfig * agendaConfigDialog;
    CalendarConfig * calendarConfigDialog;

    QGraphicsWidget * m_widget;
    QGraphicsLinearLayout * m_layout;

    AgendaWidget * m_agenda;
    CalendarWidget * m_calendar;
    ClockWidget * m_clock;

    Plasma::ScrollWidget * m_scroll;
    Plasma::TabBar * m_tab;

    Plasma::DataEngine * m_engine;

    QTime m_lastTime;
    QDate m_lastDate;
};

K_EXPORT_PLASMA_APPLET(akonadi_calendars, PlasmaCalendar)

#endif