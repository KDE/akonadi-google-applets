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


#ifndef AGENDACONFIG_H
#define AGENDACONFIG_H

#include <QWidget>

#include <Akonadi/Entity>

#include "ui_agendaconfig.h"

class AgendaConfig : public QWidget
{
    Q_OBJECT

public:
    explicit AgendaConfig(QWidget * parent = 0);

    void clear();
    void addItem(const QString & text, const int & id);

    void setDateColor(const QColor & color);
    QString dateColor() const;

    void setUpcomingColor(const QColor & color);
    QString upcomingDateColor() const;

    void setEventBackgroundColor(const QColor & color);
    QString eventBackgroundColor() const;

    void setWeeks(const int & weeks);
    int weeks() const;

    void setUpcomingDays(const int & days);
    int upcomingDays() const;

    void setCalendarsColors(const QMap<Akonadi::Entity::Id, QString> & colors);
    QMap<Akonadi::Entity::Id, QString> calendarsColors() const;

private slots:
    void calendarChanged(const int & index);
    void colorChanged(const QColor & color);
    void loadCalendarsClicked();

signals:
    void updateCalendars();
    void changed();

private:
    Ui::agendaConfig * m_agendaConfig;

    QMap<Akonadi::Entity::Id, QString> m_calendarsColors;
};

#endif // AGENDACONFIG_H
