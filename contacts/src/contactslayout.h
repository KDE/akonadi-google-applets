/*
    Akonadi google contact plasmoid - contactlayout.h
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



#ifndef CONTACTSLAYOUT_H
#define CONTACTSLAYOUT_H

#include <QGraphicsLinearLayout>


class ContactsLayout : public QGraphicsLinearLayout {

    public:
	
        ContactsLayout(Qt::Orientation orientation, QGraphicsLayoutItem * parent = 0);
	
        virtual ~ContactsLayout();

        virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;
};

#endif // CONTACTSLAYOUT_H
