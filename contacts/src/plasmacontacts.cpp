/*
    Akonadi google contact plasmoid - plasmacontacts.cpp
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

#include <plasmacontacts.h>

#include <QListWidgetItem>

#include <KABC/Addressee>

#include <Akonadi/Entity>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/CollectionFetchScope>

#include <Plasma/Theme>

PlasmaContacts::PlasmaContacts(QObject * parent, const QVariantList & args)
    : Plasma::PopupApplet(parent, args),
      m_widget(0)
{
    setConfigurationRequired(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);
    setPopupIcon(icon());
}

QGraphicsWidget * PlasmaContacts::graphicsWidget()
{
    if (!m_widget) {

        m_widget = new QGraphicsWidget(this);

        m_widget->setPreferredSize(300, 500);

        m_find = new Plasma::LineEdit(m_widget);
        m_find->setClearButtonShown(true);
        m_find->setText(i18n("Find contact"));

        m_contactList = new ContactWidget(m_widget);

        m_scroll = new Plasma::ScrollWidget(m_widget);
        m_scroll->setWidget(m_contactList);

        m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical, m_widget);

        m_mainLayout->addItem(m_find);
        m_mainLayout->addItem(m_scroll);

        m_widget->setLayout(m_mainLayout);

        connect(m_find, SIGNAL(textChanged(QString)), SLOT(lineChanged(QString)));
        connect(m_find, SIGNAL(focusChanged(bool)), SLOT(lineFocusChanged(bool)));

        configChanged();
    }

    return m_widget;
}

void PlasmaContacts::configChanged()
{
    KConfigGroup conf = config();

    m_find->setText("");

    if (conf.readEntry("findData", true) != m_contactList->findData()) {

        m_contactList->setFilterData(conf.readEntry("findData", true));

    }

    if (conf.readEntry("showEmptyContacts", true) != m_contactList->showEmptyContacts()) {

        m_contactList->setShowEmptyContacts(conf.readEntry("showEmptyContacts", true));

    }

    QList<Akonadi::Item::Id> list = conf.readEntry("collections", QList<Akonadi::Item::Id>());

    if (list.isEmpty()) {

        setConfigurationRequired(true);

    } else {

        setConfigurationRequired(false);

    }

    m_contactList->setCollections(list);
}

void PlasmaContacts::createConfigurationInterface(KConfigDialog * parent)
{
    QWidget * widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    configDialog.loadCollections->setIcon(KIcon("view-refresh"));
    configDialog.findData->setChecked(conf.readEntry("findData", true));

    fetchCollections();

    configDialog.showEmptyContacts->setChecked(conf.readEntry("showEmptyContacts", true));

    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(configDialog.findData, SIGNAL(clicked(bool)), parent, SLOT(settingsModified()));
    connect(configDialog.showEmptyContacts, SIGNAL(clicked(bool)), parent, SLOT(settingsModified()));
    connect(configDialog.loadCollections, SIGNAL(clicked(bool)), SLOT(fetchCollections()));

    parent->addPage(widget, i18n("General"), icon());
}

void PlasmaContacts::configAccepted()
{
    KConfigGroup conf = config();

    QList<Akonadi::Item::Id> list;

    for (int i = 0; i < configDialog.collectionsList->count(); i++) {

        if (configDialog.collectionsList->item(i)->checkState()) {

            list.push_back(configDialog.collectionsList->item(i)->data(Qt::UserRole).toInt());

        }

    }

    conf.writeEntry("collections", list);
    conf.writeEntry("findData", configDialog.findData->isChecked());
    conf.writeEntry("showEmptyContacts", configDialog.showEmptyContacts->isChecked());

    emit configNeedsSaving();
}

void PlasmaContacts::lineChanged(const QString & text)
{
    m_contactList->showContactsContains(text);
}

void PlasmaContacts::lineFocusChanged(bool change)
{
    if (change && m_find->text().contains(i18n("Find"))) {
	
        m_find->setText("");
	
    }
}

void PlasmaContacts::fetchCollections()
{
    while (configDialog.collectionsList->count() != 0) {

        delete configDialog.collectionsList->item(0);

    }

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this);

    job->fetchScope();

    connect(job, SIGNAL(result(KJob *)), SLOT(fetchCollectionsFinished(KJob *)));
}

void PlasmaContacts::fetchCollectionsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob * fetchJob = qobject_cast<Akonadi::CollectionFetchJob *> (job);
    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach (const Akonadi::Collection & collection, collections) {

#ifndef ALL_COLLECTIONS
        if (collection.resource().contains("akonadi_googlecontacts_resource")) {
#endif
            if (collection.contentMimeTypes().contains(KABC::Addressee::mimeType())) {

                Akonadi::EntityDisplayAttribute * attribute = collection.attribute< Akonadi::EntityDisplayAttribute > ();
	    
                QListWidgetItem * item = new QListWidgetItem();
	        
	        QString name;

	        if (collections.contains(collection.parentCollection())) {
		 
		    Akonadi::Collection col = collections.at(collections.indexOf(collection.parentCollection()));
		    Akonadi::EntityDisplayAttribute * attr = col.attribute< Akonadi::EntityDisplayAttribute > ();
		    
		    if (!attribute) {
			
			name = col.name();
			
		    } else {
			
			name = attr->displayName();
			
		    }
		    
		    name += " / ";
		}
	     
                if (!attribute) {

		    name += collection.name();
		    
                } else {

                   name += attribute->displayName();

		}
                
                item->setText(name);

                item->setData(Qt::UserRole, collection.id());
                item->setCheckState(Qt::Unchecked);

                configDialog.collectionsList->insertItem(configDialog.collectionsList->count(), item);

            }
#ifndef ALL_COLLECTIONS
        }
#endif
    }

    if (!m_contactList->collectionsList().isEmpty()) {

        for (int i = 0; i < m_contactList->collectionsList().count(); i++) {

            for (int j = 0; j < configDialog.collectionsList->count(); j++) {

                if (m_contactList->collectionsList().at(i) == configDialog.collectionsList->item(j)->data(Qt::UserRole).toInt()) {

                    configDialog.collectionsList->item(j)->setCheckState(Qt::Checked);

                }

            }

        }

    }

}

#include "plasmacontacts.moc"
