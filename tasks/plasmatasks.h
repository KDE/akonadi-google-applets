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

#include <KConfigDialog>
#include <KIcon>

#include <Plasma/PopupApplet>
#include <Plasma/ScrollWidget>

#include <QList>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/Collection>

#include "taskwidget.h"
#include "ui_config.h"

class PlasmaTasks : public Plasma::PopupApplet
{
    Q_OBJECT
    
public:

    PlasmaTasks(QObject *parent, const QVariantList &args);
    ~PlasmaTasks();

    void createConfigurationInterface(KConfigDialog *parent);

private:
    
    void configChanged();
    virtual QGraphicsWidget *graphicsWidget();
    
    Ui::config configDialog;
    
    QGraphicsWidget *m_widget;
    QGraphicsLinearLayout *m_mainLayout;
    
    TaskWidget * m_tasksList;
    
    Plasma::ScrollWidget *m_scroll;
    
    QList<Akonadi::Collection::Id> m_idList;
    
public slots:
    
    void configAccepted();
    
    void fetchCollections();
    void fetchCollectionsFinished(KJob *job);
};

K_EXPORT_PLASMA_APPLET(plasma_google_tasks, PlasmaTasks)

#endif
