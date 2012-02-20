/*
    Akonadi google contact plasmoid - contactwidgetitem.h
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


#ifndef CONTACTWIDGETITEM_H
#define CONTACTWIDGETITEM_H

#include <QObject>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Plasma/IconWidget>
#include <Plasma/Label>

#include <KABC/Addressee>

class ContactWidgetItem : public QGraphicsWidget
{
    Q_OBJECT
    
public:
    
    ContactWidgetItem(KABC::Addressee * addressee, QGraphicsWidget * parent = 0);
    virtual ~ContactWidgetItem();
    
    const KABC::Addressee * addressee() { return m_addressee; }
    
    bool containsString(const QString & string);
    bool containsStringInData(const QString & string);
    
public slots:
    
    void showInfo();
    
private:
    
    void setInfo();
    
    KABC::Addressee * m_addressee;
    
    QGraphicsLinearLayout * m_mainLayout;
 
    Plasma::IconWidget * m_icon;
    
    // TODO: add more info?
    Plasma::Label * m_name;
    
    Plasma::Label * m_homeNumber;
    Plasma::Label * m_officeNumber;
    Plasma::Label * m_cellPhone;
    Plasma::Label * m_mail;
    Plasma::Label * m_mail2;
    
    bool m_show;
    
};

#endif // CONTACTWIDGETITEM_H
