/*
    Akonadi google tasks plasmoid - taskeditor.h
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


#ifndef TASKEDITOR_H
#define TASKEDITOR_H

#include <QList>
#include <QGraphicsWidget>

#include <Akonadi/Item>
#include <Akonadi/Collection>
#include <KCalCore/Todo>

#include <KDateTime>

#include "ui_taskedit.h"

class TaskEditor : public QWidget {
    
    Q_OBJECT
    
    public:
    
	explicit TaskEditor(QWidget *parent = 0);

	void disableStartDate();
	void disableDueDate();
	void setCollections(QList<Akonadi::Collection> collections);
	void setAllDay(bool checked);
	void setStartDate(KDateTime dateTime);
	void setDueDate(KDateTime dateTime);
	void setName(QString name);
	void setDescription(QString description);
	
	void updateTodo(KCalCore::Todo::Ptr todo);
	
	Akonadi::Item::Id selectedCollection();
	
    public slots:

	void setAllDayEnabled();
	void setTimeDisabled(bool disabled);
	void setDateTimeStart(bool enabled);
	void setDateTimeDue(bool enabled);
	
    private:
    
	Ui::taskedit * m_taskEditor;

    
};

#endif // TASKEDITOR_H
