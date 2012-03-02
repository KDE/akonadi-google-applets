/*
    Akonadi google tasks plasmoid - taskwidget.h
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

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>
#include <Akonadi/Monitor>

#include <Plasma/IconWidget>
#include <Plasma/Label>

#include "taskwidgetitem.h"

class TaskWidget : public QGraphicsWidget
{
    Q_OBJECT

public:

    TaskWidget(QGraphicsWidget * parent = 0);
    
    void setCollection(Akonadi::Collection::Id id);
    
public slots:  
    
    void fetchCollectionsFinished(KJob *job);
    void fetchItemsFinished(KJob * job);
    
private:
    
    void addItem(TaskWidgetItem * item);
    
    void clear();
    
    void fetchCollections();
    void fetchItems(const Akonadi::Collection & collections);

    void updateRelationship();
    
    QGraphicsLinearLayout * m_layout;
    
    Akonadi::Collection::Id m_id;
};

#endif // TASKWIDGET_H
