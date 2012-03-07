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

#include <QGraphicsLinearLayout>

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/PushButton>
#include <Plasma/Frame>

#include <KABC/Addressee>
#include <Akonadi/Item>

#include <QPropertyAnimation>

class ContactWidgetItem : public Plasma::Frame
{
    Q_OBJECT

public:

    ContactWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent = 0);
    virtual ~ContactWidgetItem();

    bool isEmpty();
    bool hasStringInName(const QString & string);
    bool hasStringInData(const QString & string);

    void updateContact(const Akonadi::Item & item);

    bool operator<(const ContactWidgetItem * item);
    bool operator=(const Akonadi::Item & item);

    void hide();
    void show();

public slots:

    void editContact();
    void showContactInfo();
    void openEmail(const QString & string);
    
private slots:
    void doHide();

private:

    void setContactInfo();
    void setContactIcon();

    QGraphicsLinearLayout * m_mainLayout;

    Plasma::IconWidget * m_icon;
    Plasma::Label * m_infoText;
    Plasma::PushButton *m_edit;

    Akonadi::Item m_item;
    KABC::Addressee * m_addressee;

    QPropertyAnimation * m_animation;

    bool m_show;
    bool m_info;

};

#endif // CONTACTWIDGETITEM_H
