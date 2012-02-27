/*
    Akonadi google contact plasmoid - contactlayout.cpp
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



#include "contactslayout.h"

#include <KDebug>

ContactsLayout::ContactsLayout (Qt::Orientation orientation, QGraphicsLayoutItem *parent):
	QGraphicsLinearLayout (orientation, parent)
{
    setMinimumWidth(250);
}

ContactsLayout::~ContactsLayout()
{

}

QSizeF ContactsLayout::sizeHint (Qt::SizeHint which, const QSizeF &constraint) const
{
	QSizeF hint;
	int cnt = count();

	if (cnt == 0)
		hint.setHeight(0);
	else
		hint = QGraphicsLinearLayout::sizeHint (which, constraint);

	kDebug() << "SizeHint for" << cnt << "items: " << hint;

	return hint;
}

