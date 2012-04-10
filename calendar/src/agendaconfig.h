/*
    Akonadi google calendar plasmoid - agendaconfig.h
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

        QMap<Akonadi::Entity::Id,QString> calendarsColors() const {
            return m_calendarsColors;
        }
        
        QString dateColor() const {
            return m_agendaConfig->dateColor->color().name();
        }
        
        QString upcomingDateColor() const {
            return m_agendaConfig->upcomingDateColor->color().name();
        }
        
        QString eventBackgroundColor() const {
            return m_agendaConfig->eventBackground->color().name();
        }
        
        int weeks() const {
            return m_agendaConfig->weeks->currentIndex()+1;
        }
        
        int upcomingDays() const {
            return m_agendaConfig->upcomingDays->currentIndex();
        }

        void clear();
        void addItem(const QString & text, const int & id);
        void setDateColor(const QColor & color);
        void setUpcomingColor(const QColor & color);
        void setEventBackgroundColor(const QColor & color);
        void setWeeks(const int & weeks);
        void setUpcomingDays(const int & days);
        void setCalendarsColors(const QMap<Akonadi::Entity::Id,QString> & colors);

    public slots:
    
        void calendarChanged(const int & index);
        void colorChanged(const QColor & color);
        
    private slots:

        void loadCalendarsClicked();

    signals:
    
        void updateCalendars();
        void changed();
        
    private:

        Ui::agendaConfig * m_agendaConfig;
        
        QMap<Akonadi::Entity::Id,QString> m_calendarsColors;
};

#endif // AGENDACONFIG_H
