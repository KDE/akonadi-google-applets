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

#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Akonadi/Monitor>
#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>

#include "tasklayout.h"
#include "taskwidgetitem.h"

class TaskWidget : public QGraphicsWidget {
        
    Q_OBJECT

    public:

        TaskWidget(QGraphicsWidget * parent = 0);

	QString expiredColor() { 
	        return m_expiredColor;
	}
	
	QString todayColor() {
	        return m_todayColor;
	}
	
	QString weekColor() {
	        return m_weekColor;
	}
	
	QString otherColor() {
	        return m_otherColor;   
	}
	
	QList<Akonadi::Collection::Id> idList() {
	        return m_idList;   
	}
	
	void setExpiredColor(QString color);
	void setTodayColor(QString color);
	void setWeekColor(QString color);
	void setOtherColor(QString color);
	
	void setCollections(QList<Akonadi::Collection::Id> ids);
	
    private slots:

        void fetchCollectionsFinished(KJob * job);
        void fetchItemsFinished(KJob * job);

        void itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection);
        void itemChanged(const Akonadi::Item & item, QSet< QByteArray > array);
        void itemRemoved(const Akonadi::Item & item);

    private:

        void addItem(TaskWidgetItem * item);
        void clear();

        void fetchCollections();
        void fetchItems(const Akonadi::Collection & collections);

        TaskLayout * m_layout;

	QString m_expiredColor;
	QString m_todayColor;
	QString m_weekColor;
	QString m_otherColor;
	
        QList<Akonadi::Collection::Id> m_idList;

        Akonadi::Monitor * m_monitor;


};


#endif // TASKWIDGET_H
