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

#include <QListWidgetItem>
#include <QCheckBox>

#include <Akonadi/Entity>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/CollectionFetchScope>

#include <KCalCore/Todo>

PlasmaTasks::PlasmaTasks(QObject * parent, const QVariantList & args)
    : Plasma::PopupApplet(parent, args),
      m_widget(0)
{
    setConfigurationRequired(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setPopupIcon(icon());

}

QGraphicsWidget * PlasmaTasks::graphicsWidget()
{

    if (!m_widget) {

        m_widget = new QGraphicsWidget(this);

        m_widget->setMinimumSize(300, 500);

        m_tasksList = new TaskWidget(m_widget);

        m_scroll = new Plasma::ScrollWidget(m_widget);
        m_scroll->setWidget(m_tasksList);
        m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical, m_widget);

        m_mainLayout->addItem(m_scroll);

        m_widget->setLayout(m_mainLayout);

        configChanged();

    }

    return m_widget;
}

void PlasmaTasks::configChanged()
{

    KConfigGroup conf = config();

    QList<Akonadi::Item::Id> list = conf.readEntry("collections", QList<Akonadi::Item::Id>());

    if (list.isEmpty()) {

        setConfigurationRequired(true);

    } else {

        setConfigurationRequired(false);

    }

    m_idList = list;

    m_tasksList->setCollections(m_idList);

}

void PlasmaTasks::createConfigurationInterface(KConfigDialog * parent)
{
    QWidget * widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    configDialog.loadCollections->setIcon(KIcon("view-refresh"));

    fetchCollections();

    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(configDialog.loadCollections, SIGNAL(clicked(bool)), SLOT(fetchCollections()));

    parent->addPage(widget, "General", icon());
}

void PlasmaTasks::configAccepted()
{
    KConfigGroup conf = config();

    QList<Akonadi::Item::Id> list;

    for (int i = 0; i < configDialog.collectionsList->count(); i++) {

        if (configDialog.collectionsList->item(i)->checkState()) {

            list.push_back(configDialog.collectionsList->item(i)->data(Qt::UserRole).toInt());

        }

    }

    conf.writeEntry("collections", list);

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

        if (collection.resource().contains("akonadi_googletasks_resource") &&
            collection.contentMimeTypes().contains(KCalCore::Todo::todoMimeType())) {

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

    }

    if (!m_idList.isEmpty()) {

        for (int i = 0; i < m_idList.count(); i++) {

            for (int j = 0; j < configDialog.collectionsList->count(); j++) {

                if (m_idList.at(i) == configDialog.collectionsList->item(j)->data(Qt::UserRole).toInt()) {

                    configDialog.collectionsList->item(j)->setCheckState(Qt::Checked);

                }

            }

        }

    }

}

PlasmaTasks::~PlasmaTasks()
{

}

#include "plasmatasks.moc"

