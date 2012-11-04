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

#include "agendaconfig.h"

#include <KIcon>

AgendaConfig::AgendaConfig(QWidget * parent): QWidget(parent),
    m_agendaConfig(new Ui::agendaConfig)
{
    m_agendaConfig->setupUi(this);

    m_agendaConfig->loadCalendars->setIcon(KIcon("view-refresh"));

    connect(m_agendaConfig->loadCalendars, SIGNAL(clicked(bool)), SLOT(loadCalendarsClicked()));
    connect(m_agendaConfig->calendarsList, SIGNAL(currentIndexChanged(int)), SLOT(calendarChanged(int)));
    connect(m_agendaConfig->calendarColor, SIGNAL(changed(QColor)), SLOT(colorChanged(QColor)));
    connect(m_agendaConfig->weeks, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    connect(m_agendaConfig->upcomingDays, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    connect(m_agendaConfig->dateColor, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_agendaConfig->upcomingDateColor, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_agendaConfig->eventBackground, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_agendaConfig->calendarColor, SIGNAL(changed(QColor)), SIGNAL(changed()));
    connect(m_agendaConfig->eventBackgroundCheckbox, SIGNAL(toggled(bool)), SIGNAL(changed()));
}

void AgendaConfig::loadCalendarsClicked()
{
    emit updateCalendars();
}

void AgendaConfig::calendarChanged(const int & index)
{
    Akonadi::Entity::Id id = m_agendaConfig->calendarsList->itemData(index).toInt();

    if (m_calendarsColors.contains(id))
        m_agendaConfig->calendarColor->setColor(QColor(m_calendarsColors[id]));
    else
        m_agendaConfig->calendarColor->setColor(QColor("#00C000"));
}

void AgendaConfig::colorChanged(const QColor & color)
{
    Akonadi::Entity::Id id = m_agendaConfig->calendarsList->itemData(m_agendaConfig->calendarsList->currentIndex()).toInt();

    m_calendarsColors[id] = color.name();
}


void AgendaConfig::clear()
{
    while (m_agendaConfig->calendarsList->count() != 0) {
        m_agendaConfig->calendarsList->removeItem(0);
    }
}

void AgendaConfig::addItem(const QString & text, const int & id)
{
    m_agendaConfig->calendarsList->addItem(text, id);
}

void AgendaConfig::setDateColor(const QColor & color)
{
    m_agendaConfig->dateColor->setColor(color);
}

QString AgendaConfig::dateColor() const
{
    return m_agendaConfig->dateColor->color().name();
}

void AgendaConfig::setUpcomingColor(const QColor & color)
{
    m_agendaConfig->upcomingDateColor->setColor(color);
}

QString AgendaConfig::upcomingDateColor() const
{
    return m_agendaConfig->upcomingDateColor->color().name();
}

void AgendaConfig::setEventBackgroundColor(const QColor & color)
{
    if (color.isValid())
	m_agendaConfig->eventBackground->setColor(color);
    else
	m_agendaConfig->eventBackgroundCheckbox->setChecked(false);
}

QString AgendaConfig::eventBackgroundColor() const
{
    if (m_agendaConfig->eventBackgroundCheckbox->isChecked())
	return m_agendaConfig->eventBackground->color().name();
    else
	return QString("none");
}

void AgendaConfig::setWeeks(const int & weeks)
{
    m_agendaConfig->weeks->setCurrentIndex(weeks);
}

int AgendaConfig::weeks() const
{
    return m_agendaConfig->weeks->currentIndex() + 1;
}

void AgendaConfig::setUpcomingDays(const int & days)
{
    m_agendaConfig->upcomingDays->setCurrentIndex(days);
}

int AgendaConfig::upcomingDays() const
{
    return m_agendaConfig->upcomingDays->currentIndex();
}

void AgendaConfig::setCalendarsColors(const QMap< Akonadi::Entity::Id, QString > & colors)
{
    m_calendarsColors = colors;
}

QMap<Akonadi::Entity::Id, QString> AgendaConfig::calendarsColors() const
{
        return m_calendarsColors;
}
