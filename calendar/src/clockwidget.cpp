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


#include "clockwidget.h"

#include <Plasma/DataEngineManager>

ClockWidget::ClockWidget(QGraphicsItem* parent, Qt::WindowFlags wFlags):
    QGraphicsWidget(parent, wFlags),
    m_layout(new QGraphicsLinearLayout(Qt::Vertical, this)),
    m_timeLabel(new Plasma::Label(this)),
    m_dateLabel(0),
    m_dateFormat(0),
    m_color("none")
{
    m_timeLabel->setAlignment(Qt::AlignCenter);

    m_layout->setSpacing(2);
    m_layout->setContentsMargins(0, 0, 0, 2);
    m_layout->addItem(m_timeLabel);

    if (m_dateFormat) {
	m_dateLabel = new Plasma::Label(this);
	m_dateLabel->setAlignment(Qt::AlignCenter);
	m_layout->addItem(m_dateLabel);
    }

    setFontColor(m_color);
    setLayout(m_layout);
}

void ClockWidget::updateTimeLabel()
{
    QString time = KGlobal::locale()->formatTime(m_time);
    m_timeLabel->setText(time);
}

void ClockWidget::updateDateLabel()
{
    QString date;

    if (m_dateFormat == 1) {
        date = KGlobal::locale()->formatDate(m_date, KLocale::ShortDate);
    } else if (m_dateFormat == 2) {
        date = KGlobal::locale()->formatDate(m_date, KLocale::LongDate);
    } else if (m_dateFormat == 3) {
        date = KGlobal::locale()->formatDate(m_date, KLocale::IsoDate);
    }

    m_dateLabel->setText(date);
}

void ClockWidget::updateSize(const QSize & size, const Plasma::FormFactor factor)
{
    int timeSize = m_timeLabel->font().pixelSize();
    int dateSize = font().pixelSize();

    if (m_dateLabel)
	dateSize = m_dateLabel->font().pixelSize();

    if (factor == Plasma::Horizontal) {
	if (m_dateLabel) {
	    timeSize = ((size.height()/3) * 2) - 2;
	    dateSize = (size.height()/3) - 1;
	} else {
	    timeSize = size.height() - 2;
	}
    } else if (factor == Plasma::Vertical) {
	if (m_dateLabel) {
	    dateSize = (size.width()/6) - 1;
	}
	timeSize = ((size.width()/6) * 2) - 2;
    }

    if (((size.height() - dateSize - timeSize) <= 4) && m_dateLabel) {
	timeSize -= 1;
    }

    if (m_dateLabel) {
	if (dateSize < 8 && factor == Plasma::Horizontal) {
	    m_layout->setOrientation(Qt::Horizontal);
	    timeSize = ((size.height()/3) * 3) - 3;
	    dateSize = ((size.height()/3) * 3) - 2;
	    m_layout->setSpacing(4);
	} else {
	    m_layout->setSpacing(2);
	    m_layout->setOrientation(Qt::Vertical);
	}
    }

    if ((dateSize < 10 && factor == Plasma::Vertical) && m_dateLabel) {
	dateSize = timeSize;
    }

    QFont timeFont = m_timeLabel->font();
    timeFont.setPixelSize(timeSize);
    m_timeLabel->setFont(timeFont);
    m_timeLabel->setMaximumHeight(timeSize);

    if (m_dateLabel) {
	QFont dateFont = m_dateLabel->font();
	dateFont.setPixelSize(dateSize);
	m_dateLabel->setFont(dateFont);
	m_dateLabel->setMaximumHeight(dateSize);
    }

    update();
}

void ClockWidget::updateClock(const QTime & time, const QDate & date)
{
    m_time = time;
    m_date = date;

    updateTimeLabel();
    if (m_dateLabel)
	updateDateLabel();
}

void ClockWidget::updateClock(const QTime& time)
{
    m_time = time;

    updateTimeLabel();
}

void ClockWidget::setDateFormat(const int format)
{
    if (m_dateFormat == 0 && format != 0) {
	m_dateLabel = new Plasma::Label(this);
	m_dateLabel->setAlignment(Qt::AlignCenter);
	m_layout->addItem(m_dateLabel);
    } else if (m_dateFormat != 0 && format == 0) {
	m_layout->removeItem(m_dateLabel);
	delete m_dateLabel;
	m_dateLabel = 0;
    }

    m_dateFormat = format;
    if (m_dateLabel)
	updateDateLabel();
}

int ClockWidget::dateFormat() const
{
    return m_dateFormat;
}

void ClockWidget::setFontColor(const QString & color)
{
    m_color = color;

    QPalette palette;

    if (m_color != "none") {
	palette.setColor(QPalette::WindowText,QColor(m_color));
    }

    m_timeLabel->setPalette(palette);
    if (m_dateLabel)
	m_dateLabel->setPalette(palette);
}

QString ClockWidget::fontColor() const
{
    return m_color;
}

ClockWidget::~ClockWidget()
{ }

