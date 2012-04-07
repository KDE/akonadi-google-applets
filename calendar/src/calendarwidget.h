/*
    Akonadi google calendar plasmoid - calendarwidget.h.cpp
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


#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

#include <Plasma/IconWidget>
#include <Plasma/SpinBox>
#include <Plasma/ComboBox>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>
#include <Akonadi/Monitor>

#include "calendarwidgetdayitem.h"

class CalendarWidget : public QGraphicsWidget
{
    Q_OBJECT
    
    public:
        
        CalendarWidget(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
        virtual ~CalendarWidget(){};

        QList<Akonadi::Collection::Id> collectionsList() const {
            return m_idList;
        }
        
        int firstDay() const {
            return m_firstDay;
        }
        
        void setCollections(QList<Akonadi::Collection::Id> ids);
        
    public slots:
    
        void fetchCollectionsFinished(KJob * job);
        void fetchItemsFinished(KJob * job);
        
        void previousMonth();
        void nextMonth();
        void yearChanged(int year);
        void monthChanged(int month);
        void setDate(QDate date);
        
    private:
    
        void fetchCollections();
        void fetchItems(const Akonadi::Collection & collection);
        
        void addItem(const Akonadi::Item & item);
        void createCalendar();
        void clearEvents();
        void setColored(QDate date);
        
        QGraphicsLinearLayout * m_mainLayout;
        QGraphicsLinearLayout * m_changeLayout;
        QGraphicsGridLayout * m_daysLayout;
        
        QList<Akonadi::Item::Id> m_idList;
        
        Plasma::SpinBox * m_spin;
        Plasma::ComboBox * m_combo;
        
        int m_firstDay;
        
        QDate m_date;
        
};

#endif // CALENDARWIDGET_H
