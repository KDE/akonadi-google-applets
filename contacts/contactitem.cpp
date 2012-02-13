#include "contactitem.h"

#include <KIcon>

#include <QIcon>

#include <KABC/Picture>
#include <KABC/PhoneNumber>

#include <Plasma/ToolTipManager>
#include <Plasma/ToolTipContent>


ContactItem::ContactItem(KABC::Addressee * addr, QGraphicsWidget* parent): Plasma::IconWidget(parent)
{        
    setOrientation(Qt::Horizontal);
    setDrawBackground(true);
    
    addressee = addr;
    
    if (addressee->photo().isEmpty()) {
		
	// TODO: 
	setIcon(KIcon("user-identity"));
	
    } else {

	QPixmap pixmap;
    
        pixmap.convertFromImage(addr->photo().data());
    
        setIcon(KIcon(QIcon(pixmap)));
	
    }
    
    setMaximumSize(250,50);
    setMinimumSize(250,50);
    
    setWidgetText();
    setTooltipText();
        
}

void ContactItem::setWidgetText()
{

    if (addressee->name().isEmpty()) {
    
        QStringList emails = addressee->emails();
	
	if (!emails.empty()) {
	 
	    setText(emails.first());
	    
	} else {
	 
	    setText("Contact without name");
	    
	}
	
    } else {
	
	setText(addressee->name());
	
    }
    

}

void ContactItem::setTooltipText()
{

    // TODO: show other info about contact (maybe)
    // TODO: improve formating of tooltip
    
    QString main = text();
    
    // TODO: show more numbers 
    
    QString sub; 
    
    if (!addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {
	
	sub += QString::fromUtf8("Domů: ") + addressee->phoneNumber(KABC::PhoneNumber::Home).number();
	
    }
    
    if (!addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {
	
	sub += QString::fromUtf8("Práce: ") + addressee->phoneNumber(KABC::PhoneNumber::Work).number();
	
    }
    
    // TODO: show more emails
    
    if (!addressee->emails().isEmpty()) {
	
	sub += "Email: " + addressee->emails().first();
	
    }
    
    Plasma::ToolTipContent data;
    
    data.setMainText(main);
    data.setImage(icon());
    data.setSubText(sub);
    data.setAutohide(false);
    
    Plasma::ToolTipManager::self()->setContent(this, data);
    
    data.setClickable(true);
        
}

bool ContactItem::containsString(QString string)
{

    return text().toLower().contains(string.toLower());
    
}

// TODO
void ContactItem::hideContact()
{

    hide();    
}

// TODO
void ContactItem::showContact()
{
    show();
    
}





