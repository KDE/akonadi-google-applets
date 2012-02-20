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

ContactWidget::ContactWidget(QGraphicsWidget* parent): QGraphicsWidget(parent)
{    
    m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setLayout(m_layout);
    
}

void ContactWidget::addItem(ContactWidgetItem* item)
{

     for (int i = 0; i < m_layout->count(); i++) {

        if ((item->addressee()->name().toLower()) < (((ContactWidgetItem*)m_layout->itemAt(i))->addressee()->name().toLower())) {

            m_layout->insertItem(i,item);
            return;

        }

    }

    m_layout->addItem(item);
        
}

void ContactWidget::clear()
{

    QGraphicsLayoutItem * item;
    
    while (m_layout->count() > 0) {
	
	item = m_layout->itemAt(0);
	
	((ContactWidgetItem*)item)->hide();
	
	m_layout->removeItem(item);
	
        ((ContactWidgetItem*)item)->deleteLater();;
	
    }
    
}


void ContactWidget::setFilterData(bool filter)
{

    m_findData = filter;
    
}

void ContactWidget::showContactsContains(const QString& text)
{
    while (!m_list.isEmpty()) {

        addItem((ContactWidgetItem*)m_list.first());
	
        ((ContactWidgetItem*)m_list.first())->show();
	
        m_list.pop_front();

    }

    for (int i = 0; i < m_layout->count(); i++) {

        if ((( (!((ContactWidgetItem*)m_layout->itemAt(i))->containsString(text))) && m_findData && (!((ContactWidgetItem*)m_layout->itemAt(i))->containsStringInData(text)) ) ||
                (!m_findData && (!((ContactWidgetItem*)m_layout->itemAt(i))->containsString(text))))
	{

            ((ContactWidgetItem*)m_layout->itemAt(i))->hide();
	    
            m_list.push_back(m_layout->itemAt(i));
	    
            m_layout->removeAt(i);
	    
            i--;

        }

    }
   
}
