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

#include <Akonadi/ItemModifyJob>

#include <QFontMetrics>

#include <Plasma/ToolTipContent>
#include <Plasma/ToolTipManager>

TaskWidgetItem::TaskWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent)
    : Plasma::Frame(parent),
      m_completed(0),
      m_date(0),
      m_name(0),
      m_indent(0)
{
    m_item = item;

    m_todo = m_item.payload<KCalCore::Todo::Ptr>();

    m_layout = new QGraphicsGridLayout(this);
    
    setLayout(m_layout);
    
    setFrameShadow(Plasma::Frame::Raised);

    setItemInfo();
}

void TaskWidgetItem::setItemInfo()
{

    m_completed = new Plasma::CheckBox(this);
    m_completed->setChecked(m_todo->isCompleted());
    m_completed->setMaximumSize(25,25);
    
    m_layout->addItem(m_completed,0,0);    
    m_layout->setColumnAlignment(0,Qt::AlignVCenter);
    
    connect(m_completed,SIGNAL(toggled(bool)), SLOT(setCompleted(bool)));

    if (m_todo->hasDueDate()) {

        m_date = new Plasma::Label(this);

        m_date->setText(m_todo->dtDue().toString(KDateTime::LocalDate));
        m_date->setMaximumHeight(15);

        setColorForDate();

	m_layout->addItem(m_date,0,1);

    }

    m_name = new Plasma::Label(this);
    m_name->setText(m_todo->summary()); 
    
    /* TODO
    QFontMetrics * metrics = new QFontMetrics(m_name->font());
    m_name->setText(metrics->elidedText(m_todo->summary(),Qt::ElideRight,m_name->size().width()));
    */

    m_name->setMaximumHeight(15);

    if (m_completed->isChecked()) {

        m_name->setStyleSheet("text-decoration : line-through");

    }
    
    if (m_date) {
	
	m_layout->addItem(m_name,1,1);
	
    } else {
	
	m_layout->addItem(m_name,0,1);
	
    }
        
}

void TaskWidgetItem::setColorForDate()
{

    int days = KDateTime::currentLocalDateTime().daysTo(m_todo->dtDue());

    if (!m_completed->isChecked()) {

        if (days < 0) {

            m_date->setStyleSheet("color : #FF00FF");

        } else if (days == 0) {

            m_date->setStyleSheet("color : red");

        } else if (days < 8) {

            m_date->setStyleSheet("color : orange");

        } else {

            m_date->setStyleSheet("color : yellow");

        }

    }

}

void TaskWidgetItem::setCompleted(bool completed)
{

    if (completed) {

        m_name->setStyleSheet("text-decoration : line-through");
        m_todo->setCompleted(true);

    } else {

        m_name->setStyleSheet("text-decoration : none");
        m_todo->setCompleted(false);
    }

    m_item.setPayload<KCalCore::Todo::Ptr>(m_todo);

    Akonadi::ItemModifyJob * job = new Akonadi::ItemModifyJob(m_item);
    connect(job, SIGNAL(result(KJob*)), SLOT(modifyFinished(KJob*)));
}

void TaskWidgetItem::setRelated(TaskWidgetItem * item)
{

    m_indent = item->indent() + 1;

    m_layout->setContentsMargins((m_indent*25)+5,0,0,0);

}

void TaskWidgetItem::setUnrelated()
{
    m_layout->setContentsMargins(5,0,0,0);
    
    m_indent = 0;
}

void TaskWidgetItem::updateTask(const Akonadi::Item& item)
{

    m_item = item;

    m_todo = m_item.payload<KCalCore::Todo::Ptr>();

    if (m_indent != 0) {

        setUnrelated();

    }

    if (m_completed) {

        m_layout->removeItem(m_completed);
        
        delete m_completed;
        m_completed = 0;

    }

    if (m_date) {

        m_layout->removeItem(m_date); 
        
        delete m_date;
        m_date = 0;

    }

    if (m_name) {

        m_layout->removeItem(m_name);
        
        delete m_name;
        m_name = 0;

    }
    
    setItemInfo();

}


bool TaskWidgetItem::operator<(const TaskWidgetItem * item)
{

    if ((this->m_todo->isCompleted() && item->m_todo->isCompleted()) ||
            (!this->m_todo->isCompleted() && !item->m_todo->isCompleted())) {

        if (this->m_todo->hasDueDate() && item->m_todo->hasDueDate()) {

            if (this->m_todo->dtDue() == item->m_todo->dtDue()) {

                return (this->m_todo->summary().toLower() <= item->m_todo->summary().toLower());

            }

            return (this->m_todo->dtDue() <= item->m_todo->dtDue());

        } else if (this->m_todo->hasDueDate()) {

            return true;

        } else if (item->m_todo->hasDueDate()) {

            return false;

        }

    } else if (this->m_todo->isCompleted()) {

        return false;

    } else if (item->m_todo->isCompleted()) {

        return true;

    }

    return (this->m_todo->summary().toLower() <= item->m_todo->summary().toLower());

}

bool TaskWidgetItem::operator<<(const TaskWidgetItem * item)
{

    return (this->m_todo->relatedTo(KCalCore::Incidence::RelTypeParent) == item->m_todo->uid());

}

bool TaskWidgetItem::operator==(const Akonadi::Item & item)
{

    return (this->m_item.id() == item.id());

}


void TaskWidgetItem::modifyFinished(KJob* job)
{

    if ( job->error() )

        qDebug() << "Error occurred";

    else

        qDebug() << "Item modified successfully";

}
