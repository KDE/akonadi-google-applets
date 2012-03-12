/*
    Akonadi google tasks plasmoid - plasmatasks.h
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

#ifndef PLASMA_TASKS_HEADER
#define PLASMA_TASKS_HEADER

#include <QList>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <KConfigDialog>
#include <KIcon>

#include <Plasma/PushButton>
#include <Plasma/PopupApplet>
#include <Plasma/ScrollWidget>

#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>

#include "taskeditor.h"
#include "taskwidget.h"
#include "ui_config.h"
#include "ui_appearanceconfig.h"

class PlasmaTasks : public Plasma::PopupApplet {
    
    Q_OBJECT

    public:

        PlasmaTasks(QObject * parent, const QVariantList & args);
	
        ~PlasmaTasks();
	
        void createConfigurationInterface(KConfigDialog * parent);

    public slots:
	
	void addTask();
	
	void uncheckHideTasks();
	void uncheckDelTasks();
	
    private slots:

        void configAccepted();

	void createTask();
	
        void fetchCollections();
	void fetchCollectionsForEditor();
        void fetchCollectionsFinished(KJob * job);
	void fetchCollectionsForEditorFinished(KJob * job);
	
	void addFinished(KJob * job);

    private:

        void configChanged();

        virtual QGraphicsWidget * graphicsWidget();

        QGraphicsWidget * m_widget;
        QGraphicsLinearLayout * m_mainLayout;
	QGraphicsLinearLayout * m_buttonLayout;
	
        TaskWidget * m_tasksList;
	TaskEditor * m_editor;
	
	Plasma::PushButton * m_add;
	Plasma::PushButton * m_del;
        Plasma::ScrollWidget * m_scroll;

        Ui::config configDialog;
	Ui::appearanceconfig appearanceconfigDialog;
	
        QList<Akonadi::Collection> m_collections;
	
};

K_EXPORT_PLASMA_APPLET(plasma_google_tasks, PlasmaTasks)

#endif
