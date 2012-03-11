/*
    Akonadi google tasks plasmoid - taskeditor.cpp
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

#include <QListWidgetItem>

#include <Akonadi/Entity>
#include <Akonadi/EntityDisplayAttribute>

#include "taskeditor.h"

TaskEditor::TaskEditor(QWidget * parent): QWidget(parent), m_taskEditor(new Ui::taskedit)
{

    m_taskEditor->setupUi(this);

    m_taskEditor->dateEditDue->setDate(QDate::currentDate());

    connect(m_taskEditor->dateTimeDue,SIGNAL(clicked(bool)),SLOT(setAllDayEnabled()));
    connect(m_taskEditor->dateTimeDue,SIGNAL(clicked(bool)),SLOT(setDateTimeDue(bool)));
    connect(m_taskEditor->dateTimeStart,SIGNAL(clicked(bool)),SLOT(setAllDayEnabled()));
    connect(m_taskEditor->dateTimeStart,SIGNAL(clicked(bool)),SLOT(setDateTimeStart(bool)));
    connect(m_taskEditor->allDay,SIGNAL(clicked(bool)), SLOT(setTimeDisabled(bool)));

}

void TaskEditor::disableStartDate()
{

    m_taskEditor->dateEditStart->setDate(QDate::currentDate());
    m_taskEditor->dateTimeStart->setChecked(false);
    m_taskEditor->dateEditStart->setDisabled(true);
    m_taskEditor->timeEditStart->setDisabled(true);
    
}

void TaskEditor::disableDueDate()
{

    m_taskEditor->dateTimeDue->setChecked(false);
    m_taskEditor->allDay->setDisabled(true);
    m_taskEditor->dateEditDue->setDisabled(true);
    m_taskEditor->timeEditDue->setDisabled(true);
    
}

void TaskEditor::setCollections(QList< Akonadi::Collection > collections)
{

    m_taskEditor->comboBox->setEnabled(true);
    
    for (int i = 0; i < collections.count(); i++) {
	
	Akonadi::EntityDisplayAttribute * attribute = collections.at(i).attribute< Akonadi::EntityDisplayAttribute > ();
	
	if (!attribute) {

	    qDebug() << "add";
            m_taskEditor->comboBox->addItem(collections.at(i).name(),collections.at(i).id());

         } else {

	    qDebug() << "add";
            m_taskEditor->comboBox->addItem(attribute->displayName(),collections.at(i).id());

        }
	
    }
    
}

void TaskEditor::setAllDay(bool checked)
{

    m_taskEditor->allDay->setChecked(checked);

}

void TaskEditor::setStartDate(KDateTime dateTime)
{

    m_taskEditor->dateTimeStart->setChecked(true);
    m_taskEditor->dateEditStart->setEnabled(true);
    
    m_taskEditor->dateEditStart->setDate(dateTime.date());

    if (m_taskEditor->allDay->isChecked()) {

        m_taskEditor->timeEditStart->setEnabled(false);

    } else {

        m_taskEditor->timeEditStart->setTime(dateTime.time());

    }
    
}

void TaskEditor::setDueDate(KDateTime dateTime)
{

    m_taskEditor->dateEditDue->setDate(dateTime.date());

    if (m_taskEditor->allDay->isChecked()) {

        m_taskEditor->timeEditDue->setEnabled(false);

    } else {

        m_taskEditor->timeEditDue->setTime(dateTime.time());

    }

}

void TaskEditor::setName(QString name)
{

    m_taskEditor->nameEdit->setText(name);

}

void TaskEditor::setDescription(QString description)
{

    m_taskEditor->descriptionEdit->setText(description);

}

void TaskEditor::updateTodo(KCalCore::Todo::Ptr todo)
{
    
    if (m_taskEditor->dateTimeStart->isChecked()) {
	
	KDateTime time;
	
	if (m_taskEditor->allDay->isChecked()) {
	    
	    time = KDateTime(m_taskEditor->dateEditStart->date());
	    
	} else {
	 
	    time = KDateTime(m_taskEditor->dateEditStart->date(),m_taskEditor->timeEditStart->time());
	    
	}
	
	todo->setAllDay(m_taskEditor->allDay->isChecked());
	todo->setDtStart(time);
	
    } else {
	
	todo->setHasStartDate(false);
	
    }
    
    if (m_taskEditor->dateTimeDue->isChecked()) {
	
	KDateTime time;
	
	if (m_taskEditor->allDay->isChecked()) {
	    
	    time = KDateTime(m_taskEditor->dateEditDue->date());
	    
	} else {
	 
	    time = KDateTime(m_taskEditor->dateEditDue->date(),m_taskEditor->timeEditDue->time());
	    
	}
	
	todo->setAllDay(m_taskEditor->allDay->isChecked());
	todo->setDtDue(time);
	
    } else {
	
	todo->setHasDueDate(false);
	
    }
    
    todo->setSummary(m_taskEditor->nameEdit->text());
    todo->setDescription(m_taskEditor->descriptionEdit->toPlainText());
    
}

Akonadi::Entity::Id TaskEditor::selectedCollection()
{

    return m_taskEditor->comboBox->itemData(m_taskEditor->comboBox->currentIndex()).toInt();
    
}


void TaskEditor::setAllDayEnabled()
{

    if (!m_taskEditor->dateTimeDue->isChecked() && !m_taskEditor->dateTimeStart->isChecked()) {

        m_taskEditor->allDay->setDisabled(true);

    } else {

        m_taskEditor->allDay->setEnabled(true);

    }

}

void TaskEditor::setTimeDisabled(bool disabled)
{

    if (m_taskEditor->dateTimeStart->isChecked()) {

        m_taskEditor->timeEditStart->setDisabled(disabled);

    }

    if (m_taskEditor->dateTimeDue->isChecked()) {

        m_taskEditor->timeEditDue->setDisabled(disabled);

    }

}

void TaskEditor::setDateTimeStart(bool enabled)
{

    m_taskEditor->dateEditStart->setEnabled(enabled);

    if (!m_taskEditor->allDay->isChecked()) {

        m_taskEditor->timeEditStart->setEnabled(enabled);

    }

}

void TaskEditor::setDateTimeDue(bool enabled)
{

    m_taskEditor->dateEditDue->setEnabled(enabled);

    if (!m_taskEditor->allDay->isChecked()) {

        m_taskEditor->timeEditDue->setEnabled(enabled);

    }

}
