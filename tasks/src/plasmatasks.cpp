/*
    Akonadi google tasks plasmoid - plasmatasks.cpp
    Copyright (C) 2012  Jan Grulich <grulja@gmail.com>

    This program is free software: you can redistribute it and/or modif y
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for  more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <plasmatasks.h>

#include <Plasma/Theme>

#include <KIcon>
#include <KJob>

#include <QListWidgetItem>

#include <Akonadi/Entity>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/ItemCreateJob>

#include <KCalCore/Todo>

PlasmaTasks::PlasmaTasks(QObject * parent, const QVariantList & args)
    : Plasma::PopupApplet(parent, args),
      m_widget(0)
{
    setConfigurationRequired(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setPopupIcon(icon());

    graphicsWidget();
}

QGraphicsWidget * PlasmaTasks::graphicsWidget()
{
    if (!m_widget) {

        m_widget = new QGraphicsWidget(this);

        m_widget->setMinimumSize(300, 500);

        m_tasksList = new TaskWidget(m_widget);

        m_scroll = new Plasma::ScrollWidget(m_widget);
        m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_scroll->setWidget(m_tasksList);

        m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical, m_widget);

        m_mainLayout->addItem(m_scroll);

        m_add = new Plasma::PushButton(m_widget);
        m_add->setText(i18n("Add task"));
        m_add->setMaximumHeight(25);

        m_buttonLayout = new QGraphicsLinearLayout(m_mainLayout);

        m_buttonLayout->addItem(m_add);

        connect(m_add, SIGNAL(clicked()), SLOT(addTask()));

        m_mainLayout->addItem(m_buttonLayout);

        m_widget->setLayout(m_mainLayout);

        configChanged();

    }

    return m_widget;
}

void PlasmaTasks::configChanged()
{
    KConfigGroup conf = config();

    m_tasksList->setAutoDeleteCompleted(conf.readEntry("autoDel", false));
    m_tasksList->setAutoHideCompleted(conf.readEntry("autoHide", false));

    m_tasksList->setExpiredColor(conf.readEntry("expiredColor", "#c80000"));
    m_tasksList->setTodayColor(conf.readEntry("todayColor", "#e64600"));
    m_tasksList->setWeekColor(conf.readEntry("weekColor", "#e6f000"));
    m_tasksList->setOtherColor(conf.readEntry("otherColor", ""));

    m_tasksList->setOrderBy((TaskWidget::OrderBy)conf.readEntry("orderMode", 0));

    QList<Akonadi::Item::Id> list = conf.readEntry("collections", QList<Akonadi::Item::Id>());

    if (list.isEmpty()) {

        setConfigurationRequired(true);

    } else {

        setConfigurationRequired(false);

    }

    m_tasksList->setCollections(list);



}

void PlasmaTasks::createConfigurationInterface(KConfigDialog * parent)
{
    QWidget * widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    configDialog.loadCollections->setIcon(KIcon("view-refresh"));

    fetchCollections();

    configDialog.autoDel->setChecked(m_tasksList->autoDeleteCompleted());
    configDialog.autoHide->setChecked(m_tasksList->autoHideCompleted());

    connect(configDialog.autoDel, SIGNAL(clicked(bool)), SLOT(uncheckHideTasks()));
    connect(configDialog.autoHide, SIGNAL(clicked(bool)), SLOT(uncheckDelTasks()));

    parent->addPage(widget, i18n("General"), icon());

    QWidget * widget1 = new QWidget(0);

    appearanceconfigDialog.setupUi(widget1);

    appearanceconfigDialog.expiredColor->setColor(QColor(m_tasksList->expiredColor()));
    appearanceconfigDialog.todayColor->setColor(QColor(m_tasksList->todayColor()));
    appearanceconfigDialog.weekColor->setColor(QColor(m_tasksList->weekColor()));
    appearanceconfigDialog.otherColor->setColor(QColor(m_tasksList->otherColor()));

    appearanceconfigDialog.orderBy->setCurrentIndex(m_tasksList->orderBy());

    parent->addPage(widget1, i18n("Appearance"), "preferences-desktop");

    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(configDialog.loadCollections, SIGNAL(clicked(bool)), SLOT(fetchCollections()));
}

void PlasmaTasks::configAccepted()
{
    KConfigGroup conf = config();

    QList<Akonadi::Item::Id> list;

    for(int i = 0; i < configDialog.collectionsList->count(); i++) {

        if(configDialog.collectionsList->item(i)->checkState()) {

            list.push_back(configDialog.collectionsList->item(i)->data(Qt::UserRole).toInt());

        }

    }

    conf.writeEntry("autoHide", configDialog.autoHide->isChecked());
    conf.writeEntry("autoDel", configDialog.autoDel->isChecked());

    conf.writeEntry("collections", list);

    if (appearanceconfigDialog.expiredColor->color().name() != m_tasksList->expiredColor()) {

        conf.writeEntry("expiredColor", appearanceconfigDialog.expiredColor->color().name());
    }

    if (appearanceconfigDialog.todayColor->color().name() != m_tasksList->todayColor()) {

        conf.writeEntry("todayColor", appearanceconfigDialog.todayColor->color().name());
    }

    if (appearanceconfigDialog.weekColor->color().name() != m_tasksList->weekColor()) {

        conf.writeEntry("weekColor", appearanceconfigDialog.weekColor->color().name());
    }

    if (appearanceconfigDialog.otherColor->color().name() != m_tasksList->otherColor()) {

        conf.writeEntry("otherColor", appearanceconfigDialog.otherColor->color().name());
    }

    if (appearanceconfigDialog.orderBy->currentIndex() != m_tasksList->orderBy()) {

        conf.writeEntry("orderMode", appearanceconfigDialog.orderBy->currentIndex());
    }

    emit configNeedsSaving();
}

void PlasmaTasks::fetchCollections()
{
    while (configDialog.collectionsList->count() != 0) {

        delete configDialog.collectionsList->item(0);

    }

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);

    job->fetchScope();

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchCollectionsFinished(KJob *)));
}

void PlasmaTasks::fetchCollectionsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob * fetchJob = qobject_cast<Akonadi::CollectionFetchJob *>(job);
    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach(const Akonadi::Collection & collection, collections) {

#ifndef ALL_COLLECTIONS
        if ((collection.resource().contains("akonadi_googlecalendar_resource")) || (collection.resource().contains("akonadi_googletasks_resource"))) {
#endif
            if (collection.contentMimeTypes().contains(KCalCore::Todo::todoMimeType())) {

                Akonadi::EntityDisplayAttribute * attribute = collection.attribute< Akonadi::EntityDisplayAttribute > ();

                QListWidgetItem * item = new QListWidgetItem();

                if (!attribute) {

                    item->setText(collection.name());

                } else {

                    item->setText(attribute->displayName());

                }

                item->setData(Qt::UserRole, collection.id());
                item->setCheckState(Qt::Unchecked);

                configDialog.collectionsList->insertItem(configDialog.collectionsList->count(), item);

            }
#ifndef ALL_COLLECTIONS
        }
#endif
    }

    if (!m_tasksList->idList().isEmpty()) {

        for (int i = 0; i < m_tasksList->idList().count(); i++) {

            for (int j = 0; j < configDialog.collectionsList->count(); j++) {

                if (m_tasksList->idList().at(i) == configDialog.collectionsList->item(j)->data(Qt::UserRole).toInt()) {

                    configDialog.collectionsList->item(j)->setCheckState(Qt::Checked);

                }

            }

        }

    }

}

void PlasmaTasks::fetchCollectionsForEditor()
{
    m_collections.clear();

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);

    job->fetchScope();

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchCollectionsForEditorFinished(KJob *)));
}

void PlasmaTasks::fetchCollectionsForEditorFinished(KJob * job)
{
    Akonadi::CollectionFetchJob * fetchJob = qobject_cast<Akonadi::CollectionFetchJob *>(job);
    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach (const Akonadi::Collection & collection, collections) {

#ifndef ALL_COLLECTIONS
        if ((collection.resource().contains("akonadi_googlecalendar_resource")) || (collection.resource().contains("akonadi_googletasks_resource")))
#endif
	    if (collection.contentMimeTypes().contains(KCalCore::Todo::todoMimeType())) {
	
		m_collections.push_back(collection);
		
	    }
#ifndef ALL_COLLECTIONS
        }
#endif
    }

    m_editor->setCollections(m_collections);
}

void PlasmaTasks::addTask()
{
    fetchCollectionsForEditor();

    if (!m_tasksList->idList().isEmpty()) {

        m_editor = new TaskEditor();

        m_editor->setAllDay(true);
        m_editor->setStartDate(KDateTime::currentLocalDateTime());
        m_editor->setDueDate(KDateTime::currentLocalDateTime().addDays(7));

        KDialog * dialog = new KDialog();
        dialog->setCaption(i18n("New task"));
        dialog->setButtons(KDialog::Ok | KDialog::Cancel);

        dialog->setMainWidget(m_editor);

        connect(dialog, SIGNAL(okClicked()), SLOT(createTask()));
        connect(dialog, SIGNAL(okClicked()), dialog, SLOT(delayedDestruct()));
        connect(dialog, SIGNAL(cancelClicked()), dialog, SLOT(delayedDestruct()));

        dialog->show();
	
    }

}

void PlasmaTasks::uncheckDelTasks()
{
    configDialog.autoDel->setChecked(false);
}
void PlasmaTasks::uncheckHideTasks()
{
    configDialog.autoHide->setChecked(false);
}

void PlasmaTasks::createTask()
{
    KCalCore::Todo::Ptr todo(new KCalCore::Todo);

    m_editor->updateTodo(todo);

    Akonadi::Item item;

    item.setMimeType(KCalCore::Todo::todoMimeType());
    item.setPayload<KCalCore::Todo::Ptr>(todo);

    for (int i = 0; i < m_collections.count(); i++) {

        if (m_collections.at(i).id() == m_editor->selectedCollection()) {

            Akonadi::ItemCreateJob * job = new Akonadi::ItemCreateJob(item, m_collections.at(i));
            connect(job, SIGNAL(result(KJob *)), SLOT(addFinished(KJob *)));

            break;
        }

    }

}

void PlasmaTasks::addFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "Error occurred";

    } else {

        qDebug() << "Item added successfully";

    }

}

#include "plasmatasks.moc"

