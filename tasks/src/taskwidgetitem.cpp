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

#include <QWidget>

#include <KIcon>
#include <KDateTime>

#include <Akonadi/ItemModifyJob>

#include <Plasma/ToolTipContent>
#include <Plasma/ToolTipManager>

#include "taskwidget.h"

TaskWidgetItem::TaskWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent)
    : QGraphicsWidget(parent),
      m_editor(0),
      m_completed(0),
      m_date(0),
      m_name(0),
      m_indent(0)
{
    m_item = item;

    m_todo = m_item.payload<KCalCore::Todo::Ptr>();

    setAutoFillBackground(true);
    
    QColor color = QColor(((TaskWidget *)parentWidget())->backgroundColor());
    color.setAlphaF(0.5);
    QPalette palette;
    palette = this->palette();
    palette.setColor(QPalette::Window,color);
    this->setPalette(palette);
    
    setItemInfo();
}

void TaskWidgetItem::setItemInfo()
{
    m_layout = new QGraphicsGridLayout(this);

    m_completed = new Plasma::IconWidget(this);
    
    if (m_todo->isCompleted()) {
	
	m_completed->setIcon(KIcon("task-complete"));
	
    } else {
	
	m_completed->setIcon(KIcon("task-reject"));
	
    }
    m_completed->setMinimumSize(20,20);
    m_completed->setMaximumSize(20,20);

    connect(m_completed, SIGNAL(clicked()), SLOT(setCompleted()));
    m_layout->addItem(m_completed, 0, 0);

    if (m_todo->hasDueDate()) {

        m_date = new Plasma::IconWidget(this);
	m_date->setOrientation(Qt::Horizontal);
	m_date->setMinimumWidth(50);
	m_date->setMinimumHeight(20);
	m_date->setMaximumHeight(20);
        m_date->setText(m_todo->dtDue().toString(KDateTime::LocalDate));

        setColorForDate();

        m_layout->addItem(m_date, 0, 1);

    }

    m_name = new Plasma::IconWidget(this);
    m_name->setMinimumWidth(50);
    m_name->setMinimumHeight(20);
    m_name->setMaximumHeight(20);
    m_name->setText(m_todo->summary());
    m_name->setOrientation(Qt::Horizontal);

    connect(m_name, SIGNAL(clicked()), SLOT(editTask()));

    if (m_todo->isCompleted()) {

        m_name->setIcon(KIcon("dialog-ok"));
    }

    if (m_date) {

        m_layout->addItem(m_name, 1, 1);

    } else {

        m_layout->addItem(m_name, 0, 1);

    }

    Plasma::ToolTipContent content(m_todo->summary(), m_todo->description());

    if (m_todo->isCompleted()) {

        content.setImage(KIcon("dialog-ok"));

    } else {

        content.setImage(KIcon("edit-delete"));

    }

    Plasma::ToolTipManager::self()->setContent(m_name, content);

    if (m_date) {

        Plasma::ToolTipManager::self()->setContent(m_date, content);

        connect(m_date, SIGNAL(clicked()), SLOT(editTask()));

    }

    setLayout(m_layout);
}

void TaskWidgetItem::TaskWidgetItem::editTask()
{
    m_editor = new TaskEditor();

    m_editor->setAllDay(m_todo->allDay());

    if (m_todo->hasStartDate()) {

        m_editor->setStartDate(m_todo->dtStart());

    } else {

        m_editor->disableStartDate();

        if (m_todo->hasDueDate()) {

            if (m_todo->dtDue().date() < QDate::currentDate()) {

                m_editor->setStartDate(m_todo->dtDue());

            }

        }

    }

    if (m_todo->hasDueDate()) {

        m_editor->setDueDate(m_todo->dtDue());

    } else {

        m_editor->disableDueDate();

    }

    m_editor->setName(m_todo->summary());
    m_editor->setDescription(m_todo->description());

    KDialog * dialog = new KDialog();
    dialog->setCaption(m_todo->summary());
    dialog->setButtons(KDialog::Ok | KDialog::Cancel);

    dialog->setMainWidget(m_editor);

    connect(dialog, SIGNAL(okClicked()), SLOT(saveTask()));

    connect(dialog, SIGNAL(okClicked()), dialog, SLOT(delayedDestruct()));
    connect(dialog, SIGNAL(cancelClicked()), dialog, SLOT(delayedDestruct()));

    dialog->show();
}

void TaskWidgetItem::saveTask()
{
    m_editor->updateTodo(m_todo);

    m_item.setPayload<KCalCore::Todo::Ptr>(m_todo);

    Akonadi::ItemModifyJob * job = new Akonadi::ItemModifyJob(m_item);
    connect(job, SIGNAL(result(KJob *)), SLOT(modifyFinished(KJob *)));
}


void TaskWidgetItem::setColorForDate()
{
    int days = KDateTime::currentLocalDateTime().daysTo(m_todo->dtDue());

    if (!m_todo->isCompleted()) {

        if (days < 0) {

            m_date->setTextBackgroundColor(QColor(((TaskWidget *)parentWidget())->expiredColor()));

        } else if (days == 0) {

            m_date->setTextBackgroundColor(QColor(((TaskWidget *)parentWidget())->todayColor()));

        } else if (days < 8) {

            m_date->setTextBackgroundColor(QColor(((TaskWidget *)parentWidget())->weekColor()));

        } else {

            m_date->setTextBackgroundColor(QColor(((TaskWidget *)parentWidget())->otherColor()));

        }

    }

}

void TaskWidgetItem::setCompleted()
{
    if (m_todo->isCompleted()) {

        m_todo->setCompleted(false);

    } else {

        m_todo->setCompleted(true);
    }

    m_item.setPayload<KCalCore::Todo::Ptr>(m_todo);

    Akonadi::ItemModifyJob * job = new Akonadi::ItemModifyJob(m_item);
    connect(job, SIGNAL(result(KJob *)), SLOT(modifyFinished(KJob *)));
}

void TaskWidgetItem::setRelated(TaskWidgetItem * item)
{

    m_indent = item->indent() + 1;

    m_layout->setContentsMargins((m_indent * 25) + 5, 2, 2, 2);

}

void TaskWidgetItem::setUnrelated()
{
    m_layout->setContentsMargins(5, 2, 2, 2);

    m_indent = 0;
}

void TaskWidgetItem::updateTask(const Akonadi::Item & item)
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

bool TaskWidgetItem::orderByName(const TaskWidgetItem * item, bool completedFirst)
{
    if (this->m_todo->isCompleted() && !item->m_todo->isCompleted()) {

        if (completedFirst)
            return true;

        return false;

    } else if (!this->m_todo->isCompleted() && item->m_todo->isCompleted()) {

        if (completedFirst)
            return false;

        return true;

    } else {

        if (this->m_todo->hasDueDate() && item->m_todo->hasDueDate()) {
 
            if (this->m_todo->dtDue() == item->m_todo->dtDue()) {

                return (this->m_todo->summary().toLower() <= item->m_todo->summary().toLower());

            }

            return (this->m_todo->dtDue() <= item->m_todo->dtDue());

        } else if (this->m_todo->hasDueDate()) {

            if (completedFirst)
                return false;

            return true;

        } else if (item->m_todo->hasDueDate()) {

            if (completedFirst)
                return true;

            return false;

        }

    }

    return (this->m_todo->summary().toLower() <= item->m_todo->summary().toLower());
}

bool TaskWidgetItem::orderByDate(const TaskWidgetItem * item, bool completedFirst)
{
    if (this->m_todo->isCompleted() && !item->m_todo->isCompleted()) {

        if (completedFirst)
            return true;

        return false;

    } else if (!this->m_todo->isCompleted() && item->m_todo->isCompleted()) {

        if (completedFirst)
            return false;

        return true;

    } else {

        if (this->m_todo->hasDueDate() && item->m_todo->hasDueDate()) {

            if (this->m_todo->dtDue() == item->m_todo->dtDue()) {

                return (this->m_todo->summary().toLower() <= item->m_todo->summary().toLower());

            }

            return (this->m_todo->dtDue() <= item->m_todo->dtDue());

        } else if (this->m_todo->hasDueDate()) {

            if (completedFirst)
                return true;

            return false;

        } else if (item->m_todo->hasDueDate()) {

            if (completedFirst)
                return false;

            return true;

        }

    }

    return (this->m_todo->summary().toLower() <= item->m_todo->summary().toLower());
}


bool TaskWidgetItem::operator<(const TaskWidgetItem * item)
{
    switch (((TaskWidget *)parentWidget())->orderBy()) {

    case(0):

        return orderByName(item);

    case(1):

        return orderByDate(item);

    case(2):

        return orderByDate(item, true);

    case(3):

        return orderByName(item, true);
    }

    return false;
}

bool TaskWidgetItem::operator<<(const TaskWidgetItem * item)
{
    return (this->m_todo->relatedTo(KCalCore::Incidence::RelTypeParent) == item->m_todo->uid());
}

bool TaskWidgetItem::operator==(const Akonadi::Item & item)
{
    return (this->m_item.id() == item.id());
}

void TaskWidgetItem::modifyFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "Error occurred";

    } else {

        qDebug() << "Item modified successfully";

    }

}