/*
    Akonadi google tasks plasmoid - taskwidgetitem.h
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

#ifndef TASKWIDGETITEM_H
#define TASKWIDGETITEM_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Plasma/Frame>
#include <Plasma/CheckBox>
#include <Plasma/Label>

#include <KCalCore/Todo>
#include <Akonadi/Item>

class TaskWidgetItem : public Plasma::Frame
{
    Q_OBJECT

public:

    TaskWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent = 0);

    void setRelated();
    void setUnrelated();
    
    bool isRelated() { return m_related; }
    bool operator<(const TaskWidgetItem * item);
    bool operator=(const TaskWidgetItem * item);

    QString relatedTo() { return m_todo->relatedTo(KCalCore::Incidence::RelTypeParent); }
    
public slots:
    
    void setCompleted(bool completed);
    
private:

    void setItemInfo();
    
    QGraphicsLinearLayout * m_mainLayout;
    QGraphicsLinearLayout * m_infoLayout;
    
    Plasma::CheckBox * m_completed;
    Plasma::Label * m_date;
    Plasma::Label * m_name;
    
    Akonadi::Item m_item;
    KCalCore::Todo::Ptr m_todo;
    
    bool m_related;
    
    
};

#endif // TASKWIDGETITEM_H
