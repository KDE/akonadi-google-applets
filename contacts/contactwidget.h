/*
    Akonadi google contact plasmoid - contactwidget.h
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


#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QObject>
#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsLayoutItem>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>

#include <Plasma/IconWidget>
#include <Plasma/Label>

#include "contactwidgetitem.h"
#include "contactslayout.h"

class ContactWidget : public QGraphicsWidget
{
    Q_OBJECT
    
public:
    
    ContactWidget(QGraphicsWidget * parent = 0);
   
    void setCollection(Akonadi::Collection::Id id);
    void setFilterData(bool filter = true);
    void setShowEmptyContacts(bool show = true);
    
    void showContactsContains(const QString & text);
    
public slots:
    
    void fetchCollectionsFinished(KJob *job);
    void fetchItemsFinished(KJob * job);

private:
    
    void addItem(ContactWidgetItem * item);
    void clear();
    void updateContacts();

    void fetchCollections();
    void fetchItems(const Akonadi::Collection & collections);
    
    ContactsLayout * m_layout;
    
    QList<QGraphicsLayoutItem*> m_listFilterText;
    QList<QGraphicsLayoutItem*> m_listFilterEmpty;
    
    Akonadi::Collection::Id m_id;
    
    bool m_findData;
    bool m_showEmptyContacts;
    
    
    
};

#endif // CONTACTWIDGET_H
