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

#include "calendarconfig.h"

CalendarConfig::CalendarConfig(QWidget * parent):
    QWidget(parent),
    m_calendarConfig(new Ui::calendarConfig)
{
    m_calendarConfig->setupUi(this);

    connect(m_calendarConfig->actualDay, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_calendarConfig->selectedDay, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_calendarConfig->currentMonth, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_calendarConfig->outdatedMonth, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_calendarConfig->currentEvent, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_calendarConfig->outdatedEvent, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_calendarConfig->agendaPosition, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
}

void CalendarConfig::setActualDayColor(const QColor& color)
{
    m_calendarConfig->actualDay->setColor(color);
}

void CalendarConfig::setSelectedDayColor(const QColor & color)
{
    m_calendarConfig->selectedDay->setColor(color);
}

void CalendarConfig::setCurrentMonthColor(const QColor & color)
{
    m_calendarConfig->currentMonth->setColor(color);
}

void CalendarConfig::setOutdatedMonthColor(const QColor & color)
{
    m_calendarConfig->outdatedMonth->setColor(color);
}

void CalendarConfig::setCurrentEventColor(const QColor & color)
{
    m_calendarConfig->currentEvent->setColor(color);
}

void CalendarConfig::setOutdatedEventColor(const QColor & color)
{
    m_calendarConfig->outdatedEvent->setColor(color);
}

void CalendarConfig::setAgendaPosition (const CalendarWidget::AgendaPosition position)
{
    m_calendarConfig->agendaPosition->setCurrentIndex(position);
}
