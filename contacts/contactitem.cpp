/*
    Akonadi google contact plasmoid
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

#include "contactitem.h"

#include <KIcon>
#include <KRun>

#include <QIcon>

#include <KABC/Picture>
#include <KABC/PhoneNumber>

#include <Plasma/ToolTipManager>
#include <Plasma/ToolTipContent>


ContactItem::ContactItem(KABC::Addressee * addr, QGraphicsWidget* parent): Plasma::IconWidget(parent)
{
    setOrientation(Qt::Horizontal);
    setDrawBackground(true);

    m_addressee = addr;

    if (m_addressee->photo().isEmpty()) {

        setIcon(KIcon("user-identity"));

    } else {

        QPixmap pixmap;

        pixmap.convertFromImage(addr->photo().data());

        setIcon(KIcon(QIcon(pixmap)));

    }

    setMinimumSize(200,50);
    setMaximumHeight(50);

    setWidgetText();
    setTooltipText();
}

void ContactItem::setWidgetText()
{

    if (m_addressee->name().isEmpty()) {

        QStringList emails = m_addressee->emails();

        if (!emails.empty()) {

            setText(emails.first());

        } else {

            setText(i18n("Contact without name"));

        }

    } else {

        setText(m_addressee->name());

    }

}

void ContactItem::setTooltipText()
{

    QString main = text() + "<br/>";

    // TODO: show more numbers

    QString sub;


    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {

        sub += "<strong>" + i18n("Home number") + ": </strong>" + m_addressee->phoneNumber(KABC::PhoneNumber::Home).number() + "<br/>";

    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {

        sub += "<strong>" + i18n("Office number") + ": </strong>" + m_addressee->phoneNumber(KABC::PhoneNumber::Work).number() + "<br/>";

    }

    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Cell).isEmpty()) {

        sub += "<strong>" + i18n("Cell phone") + ": </strong>" + m_addressee->phoneNumber(KABC::PhoneNumber::Cell).number() + "<br/>";

    }

    // TODO: show more emails


    if (!m_addressee->emails().isEmpty()) {

        sub += "<strong>Email: </strong> <a href=\"mailto:" + m_addressee->emails().first() + "\">" + m_addressee->emails().first() + "</a><br/>";

    }


    Plasma::ToolTipContent m_tooltip;

    m_tooltip.setMainText(main);
    m_tooltip.setImage(icon());
    m_tooltip.setSubText(sub);
    m_tooltip.setAutohide(false);
    m_tooltip.setClickable(true);

    Plasma::ToolTipManager::self()->setContent(this, m_tooltip);

    //connect(Plasma::ToolTipManager::self(),SIGNAL(linkActivated(QString)),this,SLOT(linkActivated(QString)));


}

bool ContactItem::containsString(const QString & string)
{

    return text().toLower().contains(string.toLower());

}

bool ContactItem::containsStringInData(const QString & string)
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

/*void ContactItem::linkActivated(const QString string)
{

    qDebug() << "foo";
    KRun::runUrl(KUrl(string), "text/html", 0);
}*/

ContactItem::~ContactItem()
{

    delete m_addressee;

}



