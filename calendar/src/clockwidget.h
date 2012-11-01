/*
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


#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Plasma/Label>

class ClockWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    ClockWidget(QGraphicsItem* parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~ClockWidget();

    void setDateFormat(const int format);
    int dateFormat() const;

    void setFontColor(const QString color);
    QString fontColor() const;

public slots:
    void updateSize(const QSize & size, const Plasma::FormFactor factor);
    void updateClock(const QTime & time, const QDate & date);
    void updateClock(const QTime & time);
private slots:
    void updateTimeLabel();
    void updateDateLabel();
private:
    QGraphicsLinearLayout * m_layout;

    QTime m_time;
    QDate m_date;

    Plasma::Label * m_timeLabel;
    Plasma::Label * m_dateLabel;

    int m_dateFormat;
    QString m_color;
};

#endif // CLOCKWIDGET_H
