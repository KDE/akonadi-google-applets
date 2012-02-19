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

#include <plasmacontacts.h>

#include <KABC/Addressee>

#include <Akonadi/Entity>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/ItemFetchJob>


#include <Plasma/Theme>
#include <Plasma/IconWidget>

PlasmaContacts::PlasmaContacts(QObject *parent, const QVariantList &args)
        : Plasma::Applet(parent, args),
        m_id(-1),
        m_findData(true),
        m_showEmails(true),
        m_showNumbers(true)
{
    setConfigurationRequired(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(DefaultBackground);

}

void PlasmaContacts::init()
{
    m_find = new Plasma::LineEdit(this);
    m_find->setClearButtonShown(true);
    m_find->setText(i18n(" Find "));

    m_layout = new QGraphicsLinearLayout();

    m_contactList = new QGraphicsWidget(this);
    m_contactList->setLayout(m_layout);

    m_scroll = new Plasma::ScrollWidget(this);
    m_scroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_scroll->setWidget(m_contactList);

    m_mainLayout = new QGraphicsLinearLayout(Qt::Vertical,this);
    m_mainLayout->addItem(m_find);
    m_mainLayout->addItem(m_scroll);

    setLayout(m_mainLayout);

    connect(m_find,SIGNAL(textChanged(QString)),SLOT(lineChanged(QString)));
    connect(m_find,SIGNAL(focusChanged(bool)),SLOT(lineFocusChanged(bool)));

    configChanged();


}

void PlasmaContacts::configChanged()
{

    KConfigGroup conf = config();

    qDebug() << "foo";

    if (conf.readEntry("collection",-1) != m_id) {

        setConfigurationRequired(false);

        m_id = conf.readEntry("collection",-1);

        fetchCollectionsForContacts();

    }


    changeOrientation((Qt::Orientation)(conf.readEntry("orientation",1)+1));

    if (conf.readEntry("findData",true) != m_findData) {

        m_findData = conf.readEntry("findData",true);

    }

}

void PlasmaContacts::createConfigurationInterface(KConfigDialog* parent)
{
    QWidget *widget = new QWidget(0);

    configDialog.setupUi(widget);

    KConfigGroup conf = config();

    configDialog.loadCollections->setIcon(KIcon("view-refresh"));
    configDialog.orientationBox->setCurrentIndex(conf.readEntry("orientation",1));
    configDialog.findData->setChecked(conf.readEntry("findData",true));

    if (m_id != -1) {

        configDialog.collectionBox->addItem(QString::number(m_id));

    }

    //configDialog.showEmails->setChecked(conf.readEntry("showEmails",true));
    //configDialog.showNumbers->setChecked(conf.readEntry("showNumbers",true));

    // TODO: connect changes for apply
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(configDialog.collectionBox, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));
    connect(configDialog.orientationBox, SIGNAL(currentIndexChanged(int)), parent, SLOT(settingsModified()));
    connect(configDialog.findData, SIGNAL(clicked(bool)), parent, SLOT(settingsModified()));
    connect(configDialog.loadCollections,SIGNAL(clicked(bool)),SLOT(fetchCollections()));

    parent->addPage(widget,"General",icon());
}

void PlasmaContacts::configAccepted()
{
    KConfigGroup conf = config();

    conf.writeEntry("collection",configDialog.collectionBox->currentText());
    conf.writeEntry("orientation",configDialog.orientationBox->currentIndex());
    conf.writeEntry("findData", configDialog.findData->isChecked());
    //conf.writeEntry("showEmails",configDialog.showEmails->isChecked());
    //conf.writeEntry("showNumbers",configDialog.showNumbers->isChecked());

    emit configNeedsSaving();

}

void PlasmaContacts::lineChanged(const QString & text)
{

    showContactsContainsText(text);

}

void PlasmaContacts::lineFocusChanged(bool change)
{

    if (change && m_find->text().contains(i18n(" Find "))) {

        m_find->setText("");

    }

}

void PlasmaContacts::changeOrientation(Qt::Orientation orientation)
{

    m_layout->setOrientation(orientation);

    // TODO
    
    if (orientation == Qt::Vertical) {

       resize(300,300);

    } else {

       resize(600,100);

    }

}


void PlasmaContacts::fetchCollections()
{

    configDialog.collectionBox->clear();

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );

    job->fetchScope();

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsFinished(KJob*)));

}

void PlasmaContacts::fetchCollectionsFinished(KJob* job)
{

    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach ( const Akonadi::Collection &collection, collections ) {

        if (collection.resource().contains("akonadi_googlecontacts_resource")) {

            configDialog.collectionBox->addItem(QString::number(collection.id()));

        }

    }

}

void PlasmaContacts::fetchCollectionsForContacts()
{

    if (m_id == -1) {

        qDebug() << "No collection";

	return;
    }

    Akonadi::CollectionFetchJob * job = new Akonadi::CollectionFetchJob(Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );

    job->fetchScope();

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchCollectionsForContactsFinished(KJob*)));

}

void PlasmaContacts::fetchCollectionsForContactsFinished(KJob* job)
{
    if (job->error()) {

        qDebug() << "fetchCollections failed";

        return;
    }

    Akonadi::CollectionFetchJob *fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);

    const Akonadi::Collection::List collections = fetchJob->collections();

    foreach ( const Akonadi::Collection &collection, collections ) {

        if (collection.id() == m_id) {

            fetchItems(collection);

        }

    }

}

void PlasmaContacts::fetchItems(const Akonadi::Collection & collection)
{

    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(collection);

    connect(job,SIGNAL(result(KJob*)), SLOT(fetchItemsFinished(KJob*)));

    job->fetchScope().fetchFullPayload(true);


}

void PlasmaContacts::fetchItemsFinished(KJob * job)
{
    if (job->error()) {

        qDebug() << "fetchItems failed";

        return;
    }

    Akonadi::ItemFetchJob *fetchJob = qobject_cast<Akonadi::ItemFetchJob*>(job);

    const Akonadi::Item::List items = fetchJob->items();

    foreach ( const Akonadi::Item &item, items ) {

        KABC::Addressee tmp = item.payload<KABC::Addressee>();

        KABC::Addressee * addr = new KABC::Addressee(tmp);

        ContactItem * item;

        item = new ContactItem(addr,this);

        addContact(item);

    }

}

void PlasmaContacts::addContact(ContactItem * item)
{

    for (int i = 0; i < m_layout->count(); i++) {

        if ((item->addressee()->name().toLower()) < (((ContactItem*)m_layout->itemAt(i))->addressee()->name().toLower())) {

            m_layout->insertItem(i,item);

            return;

        }

    }

    m_layout->addItem(item);

}

void PlasmaContacts::showContactsContainsText(const QString & string)
{

    while (!m_list.isEmpty()) {

        addContact((ContactItem*)m_list.first());
        ((ContactItem*)m_list.first())->show();
        m_list.pop_front();

    }

    for (int i = 0; i < m_layout->count(); i++) {

        if ((( (!((ContactItem*)m_layout->itemAt(i))->containsString(string))) && m_findData && (!((ContactItem*)m_layout->itemAt(i))->containsStringInData(string)) ) ||
                (!m_findData && (!((ContactItem*)m_layout->itemAt(i))->containsString(string))))
	{

            ((ContactItem*)m_layout->itemAt(i))->hide();
            m_list.push_back(m_layout->itemAt(i));
            m_layout->removeAt(i);
            i--;

        }

    }

    qDebug() << m_layout->count();

}

/*void PlasmaContacts::changeTooltip()
{

     for (int i = 0; i < m_layout->count(); i++) {

	 //((ContactItem*)m_layout->itemAt(i))->setShowInfo(m_showEmails,m_showNumbers);

     }

}*/


#include "plasmacontacts.moc"


