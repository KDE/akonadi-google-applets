/*
    Akonadi google contact plasmoid - contactwidget.cpp
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


#include "contactwidget.h"

ContactWidget::ContactWidget(QGraphicsWidget* parent)
        : QGraphicsWidget(parent),
        m_findData(true),
        m_showEmptyContacts(true)

{
    m_layout = new ContactsLayout(Qt::Vertical, this);
    m_layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setLayout(m_layout);

}

void ContactWidget::addItem(ContactWidgetItem* item)
{

    if (!m_showEmptyContacts && item->isEmpty()) {
	
	item->hide();
	m_listFilterEmpty.push_back(item);
	
	return;
    }
    
    ContactWidgetItem * tmpItem;
    
    for (int i = 0; i < m_layout->count(); i++) {

        tmpItem = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

        if (item->name().toLower() < tmpItem->name().toLower()) {

            m_layout->insertItem(i,item);

            return;

        }

    }

    m_layout->addItem(item);

}

void ContactWidget::setFilterData(bool filter)
{

    m_findData = filter;

}

void ContactWidget::setShowEmptyContacts(bool show)
{

    if (m_showEmptyContacts != show) {
	
        m_showEmptyContacts = show;

        updateContacts();
	
    }
    
}

void ContactWidget::updateContacts()
{
            
    while (!m_listFilterEmpty.isEmpty()) {

        addItem(static_cast<ContactWidgetItem*>(m_listFilterEmpty.first()));

        static_cast<ContactWidgetItem*>(m_listFilterEmpty.first())->show();

        m_listFilterEmpty.pop_front();

    }

    ContactWidgetItem * item;

    if (!m_showEmptyContacts) {

        for (int i = 0; i < m_layout->count(); i++) {

            item = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

            if (item->isEmpty()) {

                item->hide();

                m_listFilterEmpty.push_back(item);

                m_layout->removeItem(item);;

                i--;
            }

        }

    }
    
}

void ContactWidget::clear()
{

    ContactWidgetItem * item;

    while (m_layout->count() > 0) {

        item = static_cast<ContactWidgetItem*>(m_layout->itemAt(0));

        m_layout->removeItem(item);

        item->deleteLater();

    }

}

void ContactWidget::showContactsContains(const QString& text)
{
    while (!m_listFilterText.isEmpty()) {

        addItem(static_cast<ContactWidgetItem*>(m_listFilterText.first()));

        static_cast<ContactWidgetItem*>(m_listFilterText.first())->show();

        m_listFilterText.pop_front();

    }

    ContactWidgetItem * item;

    for (int i = 0; i < m_layout->count(); i++) {

        item = static_cast<ContactWidgetItem*>(m_layout->itemAt(i));

        item->show();

        if ( ((!item->containsString(text)) && m_findData && (!item->containsStringInData(text))) ||
                ((!item->containsString(text)) && !m_findData)) {

            item->hide();

            m_listFilterText.push_back(item);

            m_layout->removeItem(item);;

            i--;

        }

    }


}
