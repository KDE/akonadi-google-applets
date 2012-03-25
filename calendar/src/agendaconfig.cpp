/*
    Akonadi google calendar plasmoid - agendaconfig.cpp
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


#include "agendaconfig.h"

#include <KIcon>

AgendaConfig::AgendaConfig(QWidget * parent): QWidget(parent), m_agendaConfig(new Ui::agendaConfig)
{
    
    m_agendaConfig->setupUi(this);

    m_agendaConfig->loadCalendars->setIcon(KIcon("view-refresh"));
    
    connect(m_agendaConfig->loadCalendars, SIGNAL(clicked(bool)), SLOT(loadCalendarsClicked()));
    connect(m_agendaConfig->calendarsList, SIGNAL(currentIndexChanged(int)), SLOT(calendarChanged(int)));
    connect(m_agendaConfig->calendarColor, SIGNAL(changed(QColor)), SLOT(colorChanged(QColor)));

}

void AgendaConfig::loadCalendarsClicked()
{

    emit updateCalendars();
    
}

void AgendaConfig::calendarChanged(int index)
{

    Akonadi::Entity::Id id = m_agendaConfig->calendarsList->itemData(index).toInt();
    
    if (m_calendarsColors.contains(id)) {
        
        m_agendaConfig->calendarColor->setColor(QColor(m_calendarsColors[id]));
        
    } else {
        
        m_agendaConfig->calendarColor->setColor(QColor("#00C000"));
        
    }
    
}

void AgendaConfig::colorChanged(QColor color)
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

void AgendaConfig::addItem(QString text, int id)
{

    m_agendaConfig->calendarsList->addItem(text,id);
    
}

void AgendaConfig::setDateColor(QColor color)
{

    m_agendaConfig->dateColor->setColor(color);
    
}

void AgendaConfig::setWeeks(int weeks)
{
    
    m_agendaConfig->weeks->setCurrentIndex(weeks);

}

void AgendaConfig::setCalendarsColors(QMap< Akonadi::Entity::Id, QString > colors)
{

    m_calendarsColors = colors;
    
}

