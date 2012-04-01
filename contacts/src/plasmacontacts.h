/*
    Akonadi google contact plasmoid - plasmacontacts.h
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

#ifndef PLASMA_CONTACTS_HEADER
#define PLASMA_CONTACTS_HEADER

#include <KConfigDialog>
#include <KIcon>

#include <Plasma/PopupApplet>
#include <Plasma/ScrollWidget>
#include <Plasma/LineEdit>

#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>

#include "contactwidget.h"
#include "ui_config.h"

class PlasmaContacts : public Plasma::PopupApplet
{
    Q_OBJECT

    public:

        PlasmaContacts(QObject * parent, const QVariantList & args);
        ~PlasmaContacts() {};

        void init();
        void createConfigurationInterface(KConfigDialog * parent);

    private slots:

        void configAccepted();

        void fetchCollections();
        void fetchCollectionsFinished(KJob * job);

        void lineChanged(const QString & text);
        void lineFocusChanged(bool change);

    private:

        virtual QGraphicsWidget * graphicsWidget();
	
        void configChanged();

        Ui::config configDialog;

        ContactWidget * m_contactList;
        QGraphicsWidget * m_widget;
        QGraphicsLinearLayout * m_mainLayout;

        Plasma::ScrollWidget * m_scroll;
        Plasma::LineEdit * m_find;
};

K_EXPORT_PLASMA_APPLET(plasma_google_contacts, PlasmaContacts)

#endif
