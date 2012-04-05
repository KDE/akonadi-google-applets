/*
    Akonadi google calendar plasmoid - calendarwidget.cpp
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


#include "calendarwidget.h"

#include <KIntSpinBox>

CalendarWidget::CalendarWidget(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags),
    m_mainLayout(0),
    m_changeLayout(0),
    m_daysLayout(0),
    m_spin(0),
    m_combo(0)
{
    m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_daysLayout = new QGraphicsGridLayout(m_mainLayout);
    m_changeLayout = new QGraphicsLinearLayout(m_mainLayout);
    
    m_spin = new Plasma::SpinBox(this);
    m_spin->nativeWidget()->setMaximum(2099);
    m_spin->setValue(QDate::currentDate().year());
    
    connect(m_spin,SIGNAL(valueChanged(int)),SLOT(yearChanged(int)));
    
    m_combo = new Plasma::ComboBox(this);
    m_combo->addItem(i18n("January"));
    m_combo->addItem(i18n("February"));
    m_combo->addItem(i18n("March"));
    m_combo->addItem(i18n("April"));
    m_combo->addItem(i18n("May"));
    m_combo->addItem(i18n("June"));
    m_combo->addItem(i18n("July"));
    m_combo->addItem(i18n("August"));
    m_combo->addItem(i18n("September"));
    m_combo->addItem(i18n("October"));
    m_combo->addItem(i18n("November"));
    m_combo->addItem(i18n("December"));
    m_combo->setCurrentIndex(QDate::currentDate().month()-1);
    
    connect(m_combo,SIGNAL(currentIndexChanged(int)),SLOT(monthChanged(int)));
    
    m_changeLayout->addItem(m_combo);
    m_changeLayout->addItem(m_spin);

    
    QFont fnt = font();
    fnt.setPointSize(fnt.pointSize()-3);
    
    QStringList daysList;
    daysList << i18n("Mon") << i18n("Tue") << i18n("Wed") << i18n("Thu")
             << i18n("Fri") << i18n("Sat") << i18n("Sun");
    
    Plasma::IconWidget * m_weekDay;         
             
    for (int i = 0; i < 7; i++) {
        
        m_weekDay = new Plasma::IconWidget(this);
        m_weekDay->setMinimumSize(10,10);
        m_weekDay->setText(daysList.at(i));
        m_weekDay->setFont(fnt);
        m_daysLayout->addItem(m_weekDay,0,i);
        
    }
             
    CalendarWidgetDayItem * m_dayItem;
    
    for (int i = 0; i < 7; i++) {
        
        for (int j = 1; j < 7; j++) {
            
            m_dayItem = new CalendarWidgetDayItem(this);
            m_dayItem->setDay(QDate::currentDate());
            
            m_daysLayout->addItem(m_dayItem,j,i);
            
        }
        
    }
    
    m_mainLayout->addItem(m_changeLayout);
    m_mainLayout->addItem(m_daysLayout);
    
    setLayout(m_mainLayout);
}

void CalendarWidget::setDay(QDate date)
{
    
    if (!date.isValid()) {
        
        return;
        
    }
    
    // TODO: better solution
    
    m_date = date;
    
    QDate dateActual(m_date.year(),m_date.month(),1);

    int daysInMonth = dateActual.daysInMonth();
    int weekDay = dateActual.dayOfWeek();
    int weekNumber = 1;
    
    CalendarWidgetDayItem * day;
   
    QDate dateBefore = dateActual;
    
    // Date before
    
    if (dateActual.month() == 1) {
        
        dateBefore.setDate(dateActual.year()-1,12,1);
        
    } else {
        
        dateBefore.setDate(dateActual.year(),dateActual.month()-1,1);
        
    }
    
    dateBefore = dateBefore.addDays(dateBefore.daysInMonth()-weekDay+1);
    
    for (int i = 0; i < weekDay; i++) {
        
        day = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(weekNumber,i));
        day->setDay(dateBefore);
        day->setActual(false);
        
        dateBefore = dateBefore.addDays(1);
    }
    
    // Actual date
    
    for (int i = 0; i < daysInMonth; i++) {
                
        day = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(weekNumber,weekDay-1));
        day->setDay(dateActual);
        day->setActual(true);
        
        dateActual = dateActual.addDays(1);
        
        if (weekDay == 7) {
            
            weekDay = 1;
            weekNumber++;
            
        } else {
            
            weekDay++;
            
        }
                
    }
    
    // Date after
    
    QDate dateAfter = dateActual;
    
    if (dateActual.month() == 12) {
        
        dateAfter.setDate(dateActual.year()+1,1,1);
        
    } else {
        
        dateAfter.setDate(dateActual.year(),dateActual.month()+1,1);
        
    }
    
    for (int i = weekDay-1; i < 7; i++) {
        
        day = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(weekNumber,i));
        day->setDay(dateActual);
        day->setActual(false);
        
        dateActual = dateActual.addDays(1);
        
    }
    
    if (weekNumber == 5) {
    
        weekNumber++;
        
        for (int i = 0; i < 7; i++) {
        
            day = static_cast<CalendarWidgetDayItem*>(m_daysLayout->itemAt(weekNumber,i));
            day->setDay(dateActual);
            day->setActual(false);
        
            dateActual = dateActual.addDays(1);
 
        }
        
    }
}

void CalendarWidget::yearChanged(int year)
{

    QDate dt(year,m_date.month(),m_date.day());
    
    if (dt.isValid()) {
        
        setDay(dt);
        
    } else {
        
        dt.setDate(year,m_date.month(),1);
        setDay(dt);
        
    }
    
}

void CalendarWidget::monthChanged(int month)
{
    QDate dt(m_date.year(),month+1,m_date.day());
    
    if (dt.isValid()) {
        
        setDay(dt);
        
    } else {
        
        dt.setDate(m_date.year(),month+1,1);
        setDay(dt);
        
    }
}

