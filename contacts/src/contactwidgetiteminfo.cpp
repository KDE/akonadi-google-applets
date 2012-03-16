/*
    Akonadi google contact plasmoid - contactwidgetiteminfo.cpp
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

#include "contactwidgetiteminfo.h"

#include <KIcon>
#include <KToolInvocation>

ContactWidgetItemInfo::ContactWidgetItemInfo(QGraphicsItem* parent, Qt::WindowFlags wFlags)
   : QGraphicsWidget(parent, wFlags),
     m_homeIcon(0),
     m_workIcon(0),
     m_cellIcon(0),
     m_emailsIcon(0),
     m_homeLabel(0),
     m_workLabel(0),
     m_cellLabel(0)
{
    m_layout = new QGraphicsGridLayout(this);
    
    this->setLayout(m_layout);
    
    QGraphicsWidget::hide();
}

void ContactWidgetItemInfo::show()
{

    int row = 0;
    
    if (m_homeIcon && m_homeLabel) {

	m_layout->addItem(m_homeIcon,row,0);
	m_layout->addItem(m_homeLabel,row,1);
		
	row++;
	
    }
    
    if (m_workIcon && m_workLabel) {

	m_layout->addItem(m_workIcon,row,0);
	m_layout->addItem(m_workLabel,row,1);
	
	row++;
	
    }
    
    if (m_cellIcon && m_cellLabel) {

	m_layout->addItem(m_cellIcon,row,0);
	m_layout->addItem(m_cellLabel,row,1);
	
	row++;
	
    }
    
    if (m_emailsIcon) {

	m_layout->addItem(m_emailsIcon,row,0);

	for (int i = 0; i < m_emailLabels.count(); i++) {
	    
	    m_layout->addItem(m_emailLabels.at(i),row,1);
	    
            row++;
	    
	}
	
	
    }
    
    QGraphicsWidget::show();
    
}

void ContactWidgetItemInfo::setHomeNumber(const QString number)
{

    m_homeIcon = new Plasma::IconWidget(this);
    m_homeIcon->setIcon(KIcon("phone"));
    m_homeIcon->setOrientation(Qt::Horizontal);
    m_homeIcon->setMinimumWidth(70);
    m_homeIcon->setMaximumHeight(15);
    m_homeIcon->setText(i18n("Home number"));
        
    m_homeLabel = new Plasma::Label(this);
    m_homeLabel->setTextSelectable(true);
    m_homeLabel->setMaximumHeight(15);
    m_homeLabel->setText(number);
}

void ContactWidgetItemInfo::setWorkNumber(const QString number)
{

    m_workIcon = new Plasma::IconWidget(this);
    m_workIcon->setIcon(KIcon("phone"));
    m_workIcon->setOrientation(Qt::Horizontal);
    m_workIcon->setMinimumWidth(70);
    m_workIcon->setMaximumHeight(15);
    m_workIcon->setText(i18n("Work number"));
        
    m_workLabel = new Plasma::Label(this);
    m_workLabel->setTextSelectable(true);
    m_workLabel->setMaximumHeight(15);
    m_workLabel->setText(number);
    
}

void ContactWidgetItemInfo::setCellPhone(const QString number)
{

    m_cellIcon = new Plasma::IconWidget(this);
    m_cellIcon->setIcon(KIcon("phone"));
    m_cellIcon->setOrientation(Qt::Horizontal);
    m_cellIcon->setMinimumWidth(70);
    m_cellIcon->setMaximumHeight(15);
    m_cellIcon->setText(i18n("Cell phone"));
        
    m_cellLabel = new Plasma::Label(this);
    m_cellLabel->setTextSelectable(true);
    m_cellLabel->setMaximumHeight(15);
    m_cellLabel->setText(number);
    
}

void ContactWidgetItemInfo::setEmails(const QStringList emails)
{

    m_emailsIcon = new Plasma::IconWidget(this);
    m_emailsIcon->setIcon(KIcon("mail-message"));
    m_emailsIcon->setOrientation(Qt::Horizontal);
    m_emailsIcon->setMinimumWidth(70);
    m_emailsIcon->setMaximumHeight(15);
    
    if (emails.count() > 1) {
	
	m_emailsIcon->setText(i18n("Emails"));
	
    } else {
	
	m_emailsIcon->setText(i18n("Email"));
	
    }
            
    for (int i = 0; i < emails.count(); i++) {
	
	Plasma::Label * email = new Plasma::Label(this);
	email->setScaledContents(true);
	email->setWordWrap(true);
	email->setTextSelectable(true);
	email->setMinimumWidth(70);
	email->setMaximumHeight(15);
	
	QString mailText = "<a href=\"" + emails.at(i) + "\">" + emails.at(i) + "</a><br>";
	
	email->setText(mailText);
        connect(email, SIGNAL(linkActivated(QString)), SLOT(openEmail(QString)));
	
	m_emailLabels.push_back(email);
	
	
    }
        
}

void ContactWidgetItemInfo::openEmail(const QString & string)
{
    KToolInvocation::invokeMailer(string);
}
