/*
    Copyright (C) 2012  Jan Grulich <jgrulich@redhat.com>

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

#include "agendawidgeteventitem.h"

#include <KRun>

AgendaWidgetEventItem::AgendaWidgetEventItem(const Akonadi::Entity::Id & id, QGraphicsWidget * parent):
    QGraphicsWidget(parent),
    m_mainLayout(new QGraphicsLinearLayout(this)),
    m_textLayout(new QGraphicsLinearLayout(Qt::Vertical, m_mainLayout)),
    m_line(new QGraphicsWidget),
    m_eventName(new Plasma::IconWidget(this)),
    m_timeText(0),
    m_hasStartTime(false),
    m_hasEndTime(false),
    m_id(id)
{
    m_textLayout->setContentsMargins(5, 0, 0, 0);
    m_textLayout->setSpacing(1);

    m_line->setMinimumHeight(5);
    m_line->setMaximumHeight(15);
    m_line->setMaximumWidth(7);
    m_line->setAutoFillBackground(true);

    QColor color = Qt::green;
    color.setAlphaF(0.5);
    QPalette palette;
    palette = m_line->palette();
    palette.setColor(QPalette::Window, color);
    m_line->setPalette(palette);

    m_eventName->setOrientation(Qt::Horizontal);
    m_eventName->setMinimumWidth(50);
    m_eventName->setMaximumHeight(15);
    m_textLayout->addItem(m_eventName);

    m_mainLayout->addItem(m_line);
    m_mainLayout->setAlignment(m_line, Qt::AlignHCenter);
    m_mainLayout->addItem(m_textLayout);
    m_mainLayout->setAlignment(m_textLayout, Qt::AlignCenter);

    setLayout(m_mainLayout);

    connect(m_eventName, SIGNAL(clicked()), SLOT(edit()));

}
void AgendaWidgetEventItem::setColor(const QString & color)
{
    QColor clr = QColor(color);
    clr.setAlphaF(0.5);
    QPalette palette;
    palette = m_line->palette();
    palette.setColor(QPalette::Window, clr);
    m_line->setPalette(palette);
}

void AgendaWidgetEventItem::setEventName(const QString & name)
{
    m_eventName->setText(name);
}

void AgendaWidgetEventItem::setEventTime(const QTime & start, const QTime & end)
{
    QString time;

    if (start.isNull()) {
	m_hasStartTime = false;
    } else {
	m_hasStartTime = true;
	m_startTime = start;
    }

    if (end.isNull()) {
	m_hasEndTime = false;
    } else {
	m_hasEndTime = true;
	m_endTime = end;
    }

    if (m_hasStartTime && m_hasEndTime) {
	time += KGlobal::locale()->formatTime(m_startTime);
	time += " - ";
	time += KGlobal::locale()->formatTime(m_endTime);
    } else if (m_hasStartTime && !m_hasEndTime) {
	time += i18n("From");
	time += " ";
	time += KGlobal::locale()->formatTime(m_startTime);
    } else if (!m_hasStartTime && m_hasEndTime) {
	time += i18n("Till");
	time += " ";
	time += KGlobal::locale()->formatTime(m_endTime);
    }

    m_timeText = new Plasma::IconWidget(this);
    m_timeText->setOrientation(Qt::Horizontal);
    m_timeText->setMinimumWidth(50);
    m_timeText->setMaximumHeight(15);
    m_timeText->setText(time);

    m_textLayout->addItem(m_timeText);

    m_line->setMaximumHeight(30);

    connect(m_timeText, SIGNAL(clicked()), SLOT(edit()));
}

bool AgendaWidgetEventItem::operator<(AgendaWidgetEventItem * item)
{
    if (m_hasStartTime && item->m_hasStartTime) {
        if (m_startTime == item->m_startTime) {
            if (m_hasEndTime && item->m_hasEndTime) {
                return (m_endTime > item->m_endTime);
            } else {
                if (m_hasEndTime) {
                    return false;
                } else {
                    return true;
                }
            }
        } else {
            return (m_startTime > item->m_startTime);
        }
    }

    if (m_hasStartTime && !item->m_hasStartTime) {
        if (!m_hasEndTime && item->m_hasEndTime) {
            return (m_startTime > item->m_endTime);
        }
        return false;
	
    } else if (!m_hasStartTime && item->m_hasStartTime) {
        if (m_hasEndTime && !item->m_hasEndTime) {
            return (m_endTime > item->m_startTime);
        }
        return true;
    }

    if (m_hasEndTime && item->m_hasEndTime) {
        if (m_endTime == item->m_endTime) {
            if (m_hasEndTime && item->m_hasEndTime) {
                return (m_endTime > item->m_endTime);
            } else {
                if (m_hasEndTime) {
                    return false;
                } else {
                    return true;
                }
            }
        } else {
            return (m_endTime < item->m_endTime);
        }
    }

    if (m_hasEndTime && !item->m_hasEndTime) {
        return false;
    } else if (!m_hasEndTime && item->m_hasEndTime) {
        return true;
    }

    return (m_eventName->text().toLower() > item->m_eventName->text().toLower());
}

bool AgendaWidgetEventItem::operator==(const Akonadi::Entity::Id & id)
{
    return (m_id == id);
}

void AgendaWidgetEventItem::edit()
{
    QString cmd = "kincidenceeditor " + QString::number(m_id);

    KRun::runCommand(cmd, 0);
}