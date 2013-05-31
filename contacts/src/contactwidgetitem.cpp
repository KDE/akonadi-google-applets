/*
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

ContactWidgetItem::ContactWidgetItem(const Akonadi::Item & item, QGraphicsWidget * parent):
    Plasma::Frame(parent),
    m_mainLayout(new QGraphicsLinearLayout(Qt::Vertical, this)),
    m_contactInfo(0),
    m_icon(new Plasma::IconWidget(this)),
    m_edit(new Plasma::PushButton(this)),
    m_item(item),
    m_addressee(0),
    m_show(false),
    m_info(false)
{
    KABC::Addressee addressee = m_item.payload<KABC::Addressee>();
    m_addressee = new KABC::Addressee(addressee);

    m_mainLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_edit->setMinimumHeight(25);
    m_edit->setMaximumHeight(25);
    m_edit->setText(i18n("Edit"));
    m_edit->hide();

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
            m_icon->setText(i18n("Contact is without name"));
        }

    } else {

        if (!m_addressee->formattedName().isEmpty()) {
            m_icon->setText(m_addressee->formattedName());
        } else {
            m_icon->setText(m_addressee->name());
        }
    }
}

void ContactWidgetItem::setContactInfo()
{
    m_contactInfo = new ContactWidgetItemInfo(this);

    ContactWidgetItemInfoLabel * label;

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {
        label = new ContactWidgetItemInfoLabel(m_contactInfo);
        label->setText(m_addressee->phoneNumber(KABC::PhoneNumber::Home).number());
        label->setIcon("contact-home-phone");

        m_contactInfo->addInfo(label);
    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {
        label = new ContactWidgetItemInfoLabel(m_contactInfo);
        label->setText(m_addressee->phoneNumber(KABC::PhoneNumber::Work).number());
        label->setIcon("contact-work-phone");

        m_contactInfo->addInfo(label);
    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty()) {
        label = new ContactWidgetItemInfoLabel(m_contactInfo);
        label->setText(m_addressee->phoneNumber(KABC::PhoneNumber::Cell).number());
        label->setIcon("contact-cell-phone");

        m_contactInfo->addInfo(label);
    }

    foreach(const QString email, m_addressee->emails()) {
        label = new ContactWidgetItemInfoLabel(m_contactInfo);
        label->setText(email);
        label->setIcon("contact-email");

        m_contactInfo->addInfo(label);

        connect(label, SIGNAL(clicked(QString)), SLOT(openEmail(QString)));
    }

    if (!m_addressee->url().pathOrUrl().isEmpty()) {
        label = new ContactWidgetItemInfoLabel(m_contactInfo);
        label->setText(m_addressee->url().pathOrUrl());
        label->setIcon("contact-homepage");

        m_contactInfo->addInfo(label);

        connect(label, SIGNAL(clicked(QString)), SLOT(openLink(QString)));
    }

    foreach(const QString im, m_addressee->customs()) {
        if (im.contains("messaging/aim")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/aim", "All"));
            label->setIcon("contact-aim");

            m_contactInfo->addInfo(label);

        } else if (im.contains("messaging/gadu")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/gadu", "All"));
            label->setIcon("contact-gadu");

            m_contactInfo->addInfo(label);

        } else if (im.contains("messaging/icq")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/icq", "All"));
            label->setIcon("contact-icq");

            m_contactInfo->addInfo(label);

        } else if (im.contains("messaging/irc")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/irc", "All"));
            label->setIcon("contact-irc");

            m_contactInfo->addInfo(label);

        } else if (im.contains("messaging/msn")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/msn", "All"));
            label->setIcon("contact-msn");

            m_contactInfo->addInfo(label);

        } else if (im.contains("messaging/skype")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/skype", "All"));
            label->setIcon("contact-skype");

            m_contactInfo->addInfo(label);

        } else if (im.contains("messaging/xmpp")) {
            label = new ContactWidgetItemInfoLabel(m_contactInfo);
            label->setText(m_addressee->custom("messaging/xmpp", "All"));
            label->setIcon("contact-jabber");

            m_contactInfo->addInfo(label);
        }
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
    if (m_addressee->name().toLower().contains(string.toLower()))
        return true;

    if (m_addressee->formattedName().toLower().contains(string.toLower()))
        return true;
    else
        return m_icon->text().toLower().contains(string.toLower());
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

    foreach(const QString email, m_addressee->emails()) {

        if (email.toLower().contains(string.toLower()))
            return true;
    }

    if (!m_addressee->url().pathOrUrl().isEmpty()) {

        if (m_addressee->url().pathOrUrl().toLower().contains(string.toLower()))
            return true;
    }

    foreach(const QString im, m_addressee->customs()) {

        if (im.contains("messaging/aim")) {

            if (m_addressee->custom("messaging/aim", "All").toLower().contains(string.toLower()))
                return true;

        } else if (im.contains("messaging/gadu")) {

            if (m_addressee->custom("messaging/gadu", "All").toLower().contains(string.toLower()))
                return true;

        } else if (im.contains("messaging/icq")) {

            if (m_addressee->custom("messaging/icq", "All").toLower().contains(string.toLower()))
                return true;

        } else if (im.contains("messaging/irc")) {

            if (m_addressee->custom("messaging/irc", "All").toLower().contains(string.toLower()))
                return true;

        } else if (im.contains("messaging/msn")) {

            if (m_addressee->custom("messaging/msn", "All").toLower().contains(string.toLower()))
                return true;

        } else if (im.contains("messaging/skype")) {

            if (m_addressee->custom("messaging/skype", "All").toLower().contains(string.toLower()))
                return true;

        } else if (im.contains("messaging/xmpp")) {

            if (m_addressee->custom("messaging/xmpp", "All").toLower().contains(string.toLower()))
                return true;
        }

    }

    return false;
}

bool ContactWidgetItem::isEmpty()
{
    bool empty = true;

    foreach(const QString im, m_addressee->customs()) {

        if (im.contains("messaging/aim"))
            empty = false;
        else if (im.contains("messaging/gadu"))
            empty = false;
        else if (im.contains("messaging/icq"))
            empty = false;
        else if (im.contains("messaging/irc"))
            empty = false;
        else if (im.contains("messaging/msn"))
            empty = false;
        else if (im.contains("messaging/skype"))
            empty = false;
        else if (im.contains("messaging/xmpp"))
            empty = false;
        if (!empty)
            break;
    }

    if (m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty() &&
            m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty() &&
            m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty() &&
            m_addressee->emails().isEmpty() &&
            m_addressee->url().isEmpty()) {

        if (empty)
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

void ContactWidgetItem::openLink(const QString & link)
{
    KToolInvocation::invokeBrowser(link);
}

void ContactWidgetItem::updateContact(const Akonadi::Item & item)
{
    if (m_show) {
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
