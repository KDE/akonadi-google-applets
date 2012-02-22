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
#include <Plasma/PushButton>
#include <Plasma/GroupBox>

#include <KABC/Addressee>
#include <Akonadi/Item>

class ContactWidgetItem : public QGraphicsWidget
{
    Q_OBJECT
    
public:
    
    ContactWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent = 0);
    virtual ~ContactWidgetItem();
    
    const KABC::Addressee * addressee() { return m_addressee; }
    const QString name() { return m_icon->text(); }
    
    bool containsString(const QString & string);
    bool containsStringInData(const QString & string);
    bool isEmpty();
    
public slots:
    
    void showInfo();
    void editContact();
    void openEmail(const QString & string);
    
private:
    
    void setInfo();
    
    Akonadi::Item m_item;
    
    KABC::Addressee * m_addressee;
    
    QGraphicsLinearLayout * m_mainLayout;
 
    Plasma::IconWidget * m_icon;
    
    // TODO: add more info?    
    Plasma::Label * m_homeNumber;
    Plasma::Label * m_officeNumber;
    Plasma::Label * m_cellPhone;
    Plasma::Label * m_mail;
    Plasma::Label * m_mail2;
    
    Plasma::PushButton *m_edit;
    
    bool m_show;
    
};

#endif // CONTACTWIDGETITEM_H
