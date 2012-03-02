/*
    Akonadi google tasks plasmoid - taskwidgetitem.cpp
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


#include "taskwidgetitem.h"

#include <KDateTime>

TaskWidgetItem::TaskWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent)
    : Plasma::Frame(parent),
      m_completed(0),
      m_date(0),
      m_name(0),
      m_related(false)
{
    m_item = item;
    
    m_todo = m_item.payload<KCalCore::Todo::Ptr>();
        
    m_mainLayout = new QGraphicsLinearLayout(Qt::Horizontal,this);
    
    m_infoLayout = new QGraphicsLinearLayout(Qt::Vertical,m_mainLayout);
    
    setLayout(m_mainLayout);
    
    setFrameShadow(Plasma::Frame::Raised);
    
    setItemInfo();
}

void TaskWidgetItem::setItemInfo()
{

    m_completed = new Plasma::CheckBox(this);
    m_completed->setChecked(m_todo->isCompleted());
    
    connect(m_completed,SIGNAL(toggled(bool)), SLOT(setCompleted(bool)));
    
    m_mainLayout->addItem(m_completed);
    
    m_mainLayout->setAlignment(m_completed,Qt::AlignVCenter);
    
    if (m_todo->hasDueDate()) {
    
	m_date = new Plasma::Label(this);
	m_date->setText(m_todo->dtDue().toString(KDateTime::LocalDate));
	m_date->setMaximumHeight(20);
	
	int days = KDateTime::currentLocalDateTime().daysTo(m_todo->dtDue());
	
	if (days < 0) {
	    
	    m_date->setStyleSheet("color : red;");
	    
	} else if (days < 8) {
	    
	    m_date->setStyleSheet("color : orange;");
	
	} else {
	    
	    m_date->setStyleSheet("color : yellow");
	        

	}

	m_infoLayout->addItem(m_date);
    }
    
    m_name = new Plasma::Label(this);
    m_name->setText(m_todo->summary());
    
    if (m_completed->isChecked()) {
	
	m_name->setStyleSheet("text-decoration : line-through");

    }
    
    m_infoLayout->addItem(m_name);
    
    m_mainLayout->addItem(m_infoLayout);
    
}

void TaskWidgetItem::setCompleted(bool completed)
{

    if (completed) {
	
	m_name->setStyleSheet("text-decoration : line-through");
	
    } else {
	
	m_name->setStyleSheet("text-decoration : none");
	
    }
    
}

void TaskWidgetItem::setRelated()
{
     m_mainLayout->setContentsMargins(30,0,0,0);
    
     m_related = true;
}

void TaskWidgetItem::setUnrelated()
{
     m_mainLayout->setContentsMargins(0,0,0,0);
 
     m_related = false;
}

bool TaskWidgetItem::operator<(const TaskWidgetItem * item)
{
    
    if (this->m_todo->hasDueDate() && item->m_todo->hasDueDate()) {
	
	if (this->m_todo->dtDue() == item->m_todo->dtDue()) {
	    
	    return (this->m_todo->summary() < item->m_todo->summary());
	    
	}
	
	return (this->m_todo->dtDue() < item->m_todo->dtDue());
	
    } else if (this->m_todo->hasDueDate()) {
	
	return true;
	
    } else if (item->m_todo->hasDueDate()) {
	
	return false;
	
    }
    
    return (this->m_todo->summary() < item->m_todo->summary());

}

bool TaskWidgetItem::operator=(const TaskWidgetItem * item)
{
    
    return (this->m_todo->relatedTo(KCalCore::Incidence::RelTypeParent) == item->m_todo->uid());
    
}

