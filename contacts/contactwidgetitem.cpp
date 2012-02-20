/*
    Akonadi google contact plasmoid - contactwidgetitem.cpp
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


#include "contactwidgetitem.h"

#include <KIcon>
#include <QLabel>

ContactWidgetItem::ContactWidgetItem(KABC::Addressee * addressee, QGraphicsWidget* parent)
        : QGraphicsWidget(parent),
        m_homeNumber(0),
        m_officeNumber(0),
        m_cellPhone(0),
        m_mail(0),
        m_mail2(0),
        m_show(false)

{

    m_addressee = addressee;

    m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_mainLayout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_icon = new Plasma::IconWidget(this);

    // SET ICON

    m_icon->setOrientation(Qt::Horizontal);
    m_icon->setDrawBackground(true);
    m_icon->setMinimumSize(250,50);
    m_icon->setMaximumHeight(50);

    if (m_addressee->photo().isEmpty()) {

        m_icon->setIcon(KIcon("user-identity"));

    } else {

        QPixmap pixmap;

        pixmap.convertFromImage(m_addressee->photo().data());

        m_icon->setIcon(KIcon(QIcon(pixmap)));

    }

    if (m_addressee->name().isEmpty()) {

        QStringList emails = m_addressee->emails();

        if (!emails.empty()) {

            m_icon->setText(emails.first());

        } else {

            m_icon->setText(i18n("Contact without name"));

        }

    } else {

        m_icon->setText(m_addressee->name());

    }

    setInfo();

    // SET LAYOUT

    m_mainLayout->addItem(m_icon);

    setLayout(m_mainLayout);

    connect(m_icon, SIGNAL(clicked()), SLOT(showInfo()));

}

void ContactWidgetItem::setInfo()
{

    QString text;

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {

        m_homeNumber = new Plasma::Label(this);

        text = "<strong>" + i18n("→ Home number: ") + "</strong>" + m_addressee->phoneNumber(KABC::PhoneNumber::Home).number();

        m_homeNumber->setText(text);
        m_homeNumber->nativeWidget()->setIndent(10);
        m_homeNumber->hide();
    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {

        m_officeNumber = new Plasma::Label(this);

        text = "<strong>" + i18n("→ Office number: ") + "</strong>" + m_addressee->phoneNumber(KABC::PhoneNumber::Work).number();

        m_officeNumber->setText(text);
        m_officeNumber->nativeWidget()->setIndent(10);
        m_officeNumber->hide();

    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty()) {

        m_cellPhone = new Plasma::Label(this);

        text = "<strong>" + i18n("→ Cell phone: ") + "</strong>" + m_addressee->phoneNumber(KABC::PhoneNumber::Cell).number();

        m_cellPhone->setText(text);
        m_cellPhone->nativeWidget()->setIndent(10);
        m_cellPhone->hide();
    }

    // TODO: show more emails

    if (!m_addressee->emails().isEmpty()) {

        m_mail = new Plasma::Label(this);

        text = "<strong>" + i18n("→ Email: ") + "</strong>" + m_addressee->emails().first();

        m_mail->setText(text);
        m_mail->nativeWidget()->setIndent(10);
        m_mail->hide();

    }

}


void ContactWidgetItem::showInfo()
{

    if (m_show) {


        if (m_homeNumber) {

            m_mainLayout->removeItem(m_homeNumber);
            m_homeNumber->hide();

        }

        if (m_officeNumber) {

            m_mainLayout->removeItem(m_officeNumber);
            m_officeNumber->hide();

        }

        if (m_cellPhone) {

            m_mainLayout->removeItem(m_cellPhone);
            m_cellPhone->hide();

        }

        if (m_mail) {

            m_mainLayout->removeItem(m_mail);
            m_mail->hide();

        }

        m_show = false;

    } else {


        if (m_homeNumber) {

            m_mainLayout->addItem(m_homeNumber);
            m_homeNumber->show();

        }

        if (m_officeNumber) {

            m_mainLayout->addItem(m_officeNumber);
            m_officeNumber->show();

        }

        if (m_cellPhone) {

            m_mainLayout->addItem(m_cellPhone);
            m_cellPhone->show();

        }

        if (m_mail) {

            m_mainLayout->addItem(m_mail);
            m_mail->show();

        }

        m_show = true;

    }

}

bool ContactWidgetItem::containsString(const QString & string)
{

    return m_icon->text().toLower().contains(string.toLower());

}

bool ContactWidgetItem::containsStringInData(const QString & string)
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

ContactWidgetItem::~ContactWidgetItem()
{

    delete m_addressee;
}

