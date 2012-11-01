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


#ifndef CALENDARCONFIG_H
#define CALENDARCONFIG_H

#include <QWidget>
#include "ui_calendarconfig.h"
#include "calendarwidget.h"

class CalendarConfig : public QWidget
{
    Q_OBJECT

public:

    explicit CalendarConfig(QWidget * parent = 0);

    QString selectedDayColor() const {
        return m_calendarConfig->selectedDay->color().name();
    }

    QString currentMonthColor() const {
        return m_calendarConfig->currentMonth->color().name();
    }

    QString outdatedMonthColor() const {
        return m_calendarConfig->outdatedMonth->color().name();
    }

    QString currentEventColor() const {
        return m_calendarConfig->currentEvent->color().name();
    }

    QString outdatedEventColor() const {
        return m_calendarConfig->outdatedEvent->color().name();
    }

    CalendarWidget::AgendaPosition agendaPosition() const {
	return (CalendarWidget::AgendaPosition) m_calendarConfig->agendaPosition->currentIndex();
    }

    void setSelectedDayColor(const QColor & color);
    void setCurrentMonthColor(const QColor & color);
    void setOutdatedMonthColor(const QColor & color);
    void setCurrentEventColor(const QColor & color);
    void setOutdatedEventColor(const QColor & color);
    void setAgendaPosition(const CalendarWidget::AgendaPosition position);

signals:
    void changed();

private:
    Ui::calendarConfig * m_calendarConfig;

};

#endif // CALENDARCONFIG_H
