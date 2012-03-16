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

#include <Plasma/IconWidget>

PlasmaCalendar::PlasmaCalendar(QObject * parent, const QVariantList & args)
    : Plasma::PopupApplet(parent, args),
      m_widget(0),
      m_agenda(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setPopupIcon(icon());
}

QGraphicsWidget * PlasmaCalendar::graphicsWidget()
{
     if (!m_widget) {
	 
        m_widget = new QGraphicsWidget(this);
        m_widget->setMinimumSize(300, 400);

	m_agenda = new AgendaWidget(m_widget);
	
        m_layout = new QGraphicsLinearLayout(Qt::Vertical,m_widget); 
	
	m_tab = new Plasma::TabBar(m_widget);
	m_tab->addTab("Tab",m_agenda);
	m_tab->addTab("Tab2",new Plasma::IconWidget(m_widget));
	
        m_layout->addItem(m_tab);
	
	m_widget->setLayout(m_layout);
    }

    return m_widget;
    
}

 
#include "plasmacalendar.moc"

