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

class TaskWidget : public QGraphicsWidget
{
    Q_OBJECT

    public:

        enum OrderBy {
            DNC = 0, NDC = 1, CDN = 2, CND = 3
        };

        TaskWidget(QGraphicsWidget * parent = 0);

	QString backgroundColor() const {
            return m_backgroundColor;
        }
	
        QString expiredColor() const {
            return m_expiredColor;
        }

        QString todayColor() const {
            return m_todayColor;
        }

        QString weekColor() const {
            return m_weekColor;
        }

        QString otherColor() const {
            return m_otherColor;
        }
        
        QString completedColor() const {
	    return m_completedColor;
        }

        QList<Akonadi::Collection::Id> idList() const {
            return m_idList;
        }

        OrderBy orderBy() const {
            return m_order;
        }

        bool autoHideCompleted() const {
            return m_autoHide;
        }

        bool autoDeleteCompleted() const {
            return m_autoDel;
        }

        void setBackgroundColor(const QString color);
        void setExpiredColor(const QString color);
        void setTodayColor(const QString color);
        void setWeekColor(const QString color);
        void setOtherColor(const QString color);
	void setCompletedColor(const QString color);
        void setOrderBy(const OrderBy order);

        void setAutoHideCompleted(bool hide);
        void setAutoDeleteCompleted(bool del);

        void setCollections(QList<Akonadi::Collection::Id> ids);

    private slots:

        void fetchCollectionsFinished(KJob * job);
        void fetchItemsFinished(KJob * job);

        void itemDeleted(KJob * job);

        void itemAdded(const Akonadi::Item & item, const Akonadi::Collection & collection);
        void itemChanged(const Akonadi::Item & item, QSet< QByteArray > array);
        void itemRemoved(const Akonadi::Item & item);

    private:

        void addItem(TaskWidgetItem * item);
        void clear();
        void updateCompletedTasks();

        void fetchCollections();
        void fetchItems(const Akonadi::Collection & collections);

        TaskLayout * m_layout;

	QString m_backgroundColor;
        QString m_expiredColor;
        QString m_todayColor;
        QString m_weekColor;
        QString m_otherColor;
	QString m_completedColor;

        bool m_autoHide;
        bool m_autoDel;

        QList<Akonadi::Collection::Id> m_idList;

        Akonadi::Monitor * m_monitor;

        OrderBy m_order;
};


#endif // TASKWIDGET_H
