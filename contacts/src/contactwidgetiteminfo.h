/*
    Akonadi google contact plasmoid - contactwidgetiteminfo.h
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


#ifndef CONTACTWIDGETITEMINFO_H
#define CONTACTWIDGETITEMINFO_H

#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsGridLayout>

#include <Plasma/IconWidget>
#include <Plasma/Label>

class ContactWidgetItemInfo : public QGraphicsWidget
{
    Q_OBJECT
    
    public:
	
	ContactWidgetItemInfo(QGraphicsItem* parent = 0, Qt::WindowFlags wFlags = 0);
    
	void show();
	
	void setHomeNumber(const QString number);
	void setWorkNumber(const QString number);
	void setCellPhone(const QString number);
	void setEmails(const QStringList emails);
	
    public slots:
	
	void openEmail(const QString & mail);
	    
    private:
	
	QGraphicsGridLayout * m_layout;
	
	Plasma::IconWidget * m_homeIcon;
	Plasma::IconWidget * m_workIcon;
	Plasma::IconWidget * m_cellIcon;
	Plasma::IconWidget * m_emailsIcon;
   
	Plasma::Label * m_homeLabel;
	Plasma::Label * m_workLabel;
	Plasma::Label * m_cellLabel;
	QList<Plasma::Label*> m_emailLabels;
};

#endif // CONTACTWIDGETITEMINFO_H
