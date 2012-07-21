/*
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

class TaskEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TaskEditor(QWidget * parent = 0);

    void disableStartDate();
    void disableDueDate();
    void setCollections(const QList<Akonadi::Collection> & collections);
    void setAllDay(const bool & checked);
    void setStartDate(const KDateTime & dateTime);
    void setDueDate(const KDateTime & dateTime);
    void setName(const QString & name);
    void setDescription(const QString & description);

    void updateTodo(const KCalCore::Todo::Ptr & todo);

    Akonadi::Item::Id selectedCollection();

public slots:
    void setAllDayEnabled();
    void setTimeDisabled(const bool & disabled);
    void setDateTimeStart(const bool & enabled);
    void setDateTimeDue(const bool & enabled);

private:
    Ui::taskedit * m_taskEditor;
};

#endif // TASKEDITOR_H
