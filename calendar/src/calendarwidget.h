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


#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/PushButton>
#include <Plasma/SpinBox>
#include <Plasma/ComboBox>
#include <Plasma/ScrollWidget>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>
#include <Akonadi/Monitor>
#include <Akonadi/Entity>

#include "calendarwidgetdayitem.h"
#include "agendawidgetdateitem.h"

class CalendarWidget : public QGraphicsWidget
{
    Q_OBJECT

public:

    enum AgendaPosition {None, NextTo, Under};

    CalendarWidget(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~CalendarWidget() {};

    void setCollections(const QList<Akonadi::Collection::Id> & ids);
    QList<Akonadi::Collection::Id> collections() const;

    void setCalendarsColors(const QMap<Akonadi::Collection::Id, QString> & colors);
    QMap<Akonadi::Item::Id, QString> calendarsColors() const;

    void setDateColor(const QString & color);
    QString dateColor() const;

    void setEventBackgroundColor(const QString & color);
    QString eventBackgroundColor() const;

    void setSelectedDayColor(const QString & color);
    QString selectedDayColor() const;

    void setCurrentMonthColor(const QString & color);
    QString currentMonthColor() const;

    void setOutdatedMonthColor(const QString & color);
    QString outdatedMonthColor() const;

    void setCurrentEventColor(const QString & color);
    QString currentEventColor() const;

    void setOutdatedEventColor(const QString & color);
    QString outdatedEventColor() const;

    int firstDay() const;

    void setAgendaPosition(const AgendaPosition position);
    AgendaPosition agendaPosition() const;

public slots:
    void fetchCollectionsFinished(KJob * job);
    void fetchItemsFinished(KJob * job);

    void dayChanged(const QDate & date);
    void monthChanged(const int & month);
    void yearChanged(const int & year);

    void previousMonth();
    void nextMonth();

    void setDate(const QDate & date);
    void setToday();

    void updateSize(QSizeF size);
    void updateFontDays(QFont font);

    void itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection);
    void itemChanged(const Akonadi::Item & item, QSet< QByteArray > array);
    void itemRemoved(const Akonadi::Item & item);

private:
    void fetchCollections();
    void fetchItems(const Akonadi::Collection & collection);

    void addItem(const Akonadi::Item & item);
    void createCalendar();
    void clearEvents();
    void setColored(const QDate & date);

    QGraphicsLinearLayout * m_mainLayout;
    QGraphicsLinearLayout * m_dateLayout;
    QGraphicsLinearLayout * m_changeLayout;
    QGraphicsGridLayout * m_daysLayout;

    QList<Akonadi::Item::Id> m_idList;
    QMap<Akonadi::Item::Id, QString> m_calendarsColors;

    QString m_dateColor;
    QString m_eventBackgroundColor;
    QString m_selectedDayColor;
    QString m_currentMonthColor;
    QString m_outdatedMonthColor;
    QString m_currentEventColor;
    QString m_outdatedEventColor;

    Plasma::SpinBox * m_spin;
    Plasma::ComboBox * m_combo;
    Plasma::ScrollWidget * m_scroll;

    AgendaPosition m_agendaPosition;
    AgendaWidgetDateItem * m_agenda;

    int m_firstDay;

    QDate m_date;

    Akonadi::Monitor * m_monitor;
};

#endif // CALENDARWIDGET_H
