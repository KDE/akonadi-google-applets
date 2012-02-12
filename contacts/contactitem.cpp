#include "contactitem.h"

#include <kicon.h>

#include <KABC/Picture>

#include <Plasma/Dialog>

ContactItem::ContactItem(KABC::Addressee * addr, QGraphicsWidget* parent): Plasma::IconWidget(parent)
{        
   // contact_picture = new Plasma::IconWidget(this);
    setOrientation(Qt::Horizontal);
    setDrawBackground(true);
    
    addressee = addr;
        
    if (addr->photo().isEmpty()) {
		
	// TODO: 
	setIcon(KIcon("user-identity"));
	
    } else {

	QPixmap pixmap;
    
        pixmap.convertFromImage(addr->photo().data());
    
        setIcon(KIcon(QIcon(pixmap)));
	
    }
        
    if (addr->name().isEmpty()) {
    
        // TODO: 
	setText(addressee->emails().at(0));
	
    } else {
	
	setText(addr->name());
	
    }
    
    setMaximumSize(250,50);
    setMinimumSize(250,50);
    
    connect(this,SIGNAL(clicked()),SLOT(clickedOnContact()));
    
}

void ContactItem::clickedOnContact()
{
    
    /*Plasma::Dialog * dialog = new Plasma::Dialog();
    
    Plasma::Label * label = new Plasma::Label(this);
    
    label->setText(text());
    
    dialog->setGraphicsWidget(label);*/
    
    qDebug() << "Clicked on " << text();
    
    /*dialog->show();*/

}




