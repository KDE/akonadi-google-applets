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

#ifndef CONTACTITEM_H
#define CONTACTITEM_H

#include <QObject>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <KABC/Addressee>

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/ToolTipContent>

class ContactItem : public Plasma::IconWidget
{
    Q_OBJECT

public:
    ContactItem(KABC::Addressee * addr, QGraphicsWidget *parent = 0);
    
    ~ContactItem();
    
    const KABC::Addressee * addressee() { return m_addressee; }
    
    bool containsString(const QString & string);
    bool containsStringInData(const QString & string);
    
    void setTooltipText();
 
    
public slots:
    
    void linkActivated(QString);
    
private:
        
    void setWidgetText();
    
    KABC::Addressee * m_addressee;
        
};


#endif // CONTACTITEM_H
