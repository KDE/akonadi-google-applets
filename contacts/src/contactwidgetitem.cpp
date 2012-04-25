/*
    Akonadi google contact plasmoid - contactwidgetitem.cpp
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software: you can redistribute it and/or modif y
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


#include "contactwidgetitem.h"

#include <KIcon>
#include <KToolInvocation>
#include <QLabel>
#include <Akonadi/Contact/ContactEditorDialog>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

ContactWidgetItem::ContactWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent)
    : Plasma::Frame(parent),
      m_contactInfo(0),
      m_edit(0),
      m_show(false),
      m_info(false)
{
    m_item = item;

    KABC::Addressee addressee = m_item.payload<KABC::Addressee>();

    m_addressee = new KABC::Addressee(addressee);

    m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_mainLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_edit = new Plasma::PushButton(this);
    m_edit->setMinimumHeight(25);
    m_edit->setMaximumHeight(25);
    m_edit->setText(i18n("Edit"));
    m_edit->hide();

    m_icon = new Plasma::IconWidget(this);
    m_icon->setOrientation(Qt::Horizontal);
    m_icon->setDrawBackground(true);
    m_icon->setMinimumSize(50, 50);
    m_icon->setMaximumHeight(50);

    setContactIcon();

    m_mainLayout->addItem(m_icon);

    setFrameShadow(Plasma::Frame::Raised);

    setLayout(m_mainLayout);

    connect(m_icon, SIGNAL(clicked()), SLOT(showContactInfo()));
    connect(m_edit, SIGNAL(clicked()), SLOT(editContact()));
}

void ContactWidgetItem::setContactIcon()
{
    if (m_addressee->photo().isEmpty()) {

        m_icon->setIcon(KIcon("user-identity"));

    } else {

        QPixmap pixmap;

        pixmap.convertFromImage(m_addressee->photo().data());

        m_icon->setIcon(KIcon(QIcon(pixmap)));

    }

    if (m_addressee->formattedName().isEmpty() && m_addressee->name().isEmpty()) {

        QStringList emails = m_addressee->emails();

        if (!emails.empty()) {

            m_icon->setText(emails.first());

        } else {

            m_icon->setText(i18n("Contact without name"));

        }

    } else {

        if (!m_addressee->formattedName().isEmpty()) {

            m_icon->setText(m_addressee->formattedName());

        }

        else {

            m_icon->setText(m_addressee->name());

        }

    }

}

void ContactWidgetItem::setContactInfo()
{
    m_contactInfo = new ContactWidgetItemInfo(this);

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {

        m_contactInfo->setHomeNumber(m_addressee->phoneNumber(KABC::PhoneNumber::Home).number());
    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {

        m_contactInfo->setWorkNumber(m_addressee->phoneNumber(KABC::PhoneNumber::Work).number());

    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty()) {

        m_contactInfo->setCellPhone(m_addressee->phoneNumber(KABC::PhoneNumber::Cell).number());

    }

    if (!m_addressee->emails().isEmpty()) {

        m_contactInfo->setEmails(m_addressee->emails());

    }

}

void ContactWidgetItem::showContactInfo()
{
    if (!m_info) {

        setContactInfo();
        m_info = true;

    }

    if (m_show) {

        setFrameShadow(Plasma::Frame::Raised);

        if (!isEmpty()) {

            m_mainLayout->removeItem(m_contactInfo);
            m_contactInfo->hide();

        }

        m_mainLayout->removeItem(m_edit);
        m_edit->hide();

        m_show = false;

    } else {

        setFrameShadow(Plasma::Frame::Sunken);

        if (!isEmpty()) {

            m_mainLayout->addItem(m_contactInfo);
            m_contactInfo->show();

        }

        m_mainLayout->addItem(m_edit);
        m_edit->show();

        m_show = true;
	
    }

}

bool ContactWidgetItem::hasStringInName(const QString & string)
{
    if (m_addressee->name().toLower().contains(string.toLower())) {
	
	return true;
	
    } 
    
    if (m_addressee->formattedName().toLower().contains(string.toLower())) {
	
	return true;
	
    } else {
    
	return m_icon->text().toLower().contains(string.toLower());
	
    }
    
}

bool ContactWidgetItem::hasStringInData(const QString & string)
{
    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {

        if (m_addressee->phoneNumber(KABC::PhoneNumber::Home).number().contains(string))

            return true;

    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {

        if (m_addressee->phoneNumber(KABC::PhoneNumber::Work).number().contains(string))

            return true;

    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty()) {

        if (m_addressee->phoneNumber(KABC::PhoneNumber::Cell).number().contains(string))

            return true;

    }

    if (!m_addressee->emails().isEmpty()) {

        if (m_addressee->emails().first().contains(string))

            return true;

    }

    return false;

}

bool ContactWidgetItem::isEmpty()
{
    if (m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty() &&
            m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty() &&
            m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty() &&
            m_addressee->emails().isEmpty()) {

        return true;

    }

    return false;
}

bool ContactWidgetItem::operator<(const ContactWidgetItem * item)
{
    return (this->m_icon->text().toLower() < item->m_icon->text().toLower());
}

bool ContactWidgetItem::operator= (const Akonadi::Item & item)
{
    return (this->m_item.id() == item.id());
}

void ContactWidgetItem::editContact()
{
    Akonadi::ContactEditorDialog * dialog = new Akonadi::ContactEditorDialog(Akonadi::ContactEditorDialog::EditMode);

    dialog->setContact(m_item);

    connect(dialog, SIGNAL(closeClicked()), dialog, SLOT(delayedDestruct()));
    connect(dialog, SIGNAL(okClicked()), dialog, SLOT(delayedDestruct()));
    connect(dialog, SIGNAL(cancelClicked()), dialog, SLOT(delayedDestruct()));

    dialog->show();
}

void ContactWidgetItem::openEmail(const QString & string)
{
    KToolInvocation::invokeMailer(string);
}

void ContactWidgetItem::updateContact(const Akonadi::Item & item)
{
    if (m_show) {

        if (!isEmpty())

            showContactInfo();

        m_show = false;

    }

    m_item = item;

    KABC::Addressee addressee = m_item.payload<KABC::Addressee>();

    delete m_addressee;

    m_addressee = new KABC::Addressee(addressee);

    if (!m_contactInfo) {

        delete m_contactInfo;
        m_contactInfo = 0;

    }

    setContactIcon();

    m_info = false;
}

ContactWidgetItem::~ContactWidgetItem()
{
    delete m_addressee;
}


