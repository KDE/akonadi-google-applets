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
    
    m_addressee = addr;
    
    if (m_addressee->photo().isEmpty()) {
		
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

    if (m_addressee->name().isEmpty()) {
    
        QStringList emails = m_addressee->emails();
	
	if (!emails.empty()) {
	 
	    setText(emails.first());
	    
	} else {
	 
	    setText("Contact without name");
	    
	}
	
    } else {
	
	setText(m_addressee->name());
	
    }
    
}

void ContactItem::setTooltipText()
{

    // TODO: show other info about contact (maybe)
    // TODO: improve formating of tooltip
    
    QString main = text();
    
    // TODO: show more numbers 
    
    QString sub; 
    
    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Home).isEmpty()) {
	
	sub += QString::fromUtf8("Domů: ") + m_addressee->phoneNumber(KABC::PhoneNumber::Home).number();
	
    }
    
    if (!m_addressee->phoneNumber(KABC::PhoneNumber::Work).isEmpty()) {
	
	sub += QString::fromUtf8("Práce: ") + m_addressee->phoneNumber(KABC::PhoneNumber::Work).number();
	
    }
    
    // TODO: show more emails
    
    if (!m_addressee->emails().isEmpty()) {
	
	sub += "Email: " + m_addressee->emails().first();
	
    }
    
    // TODO: 
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

bool ContactItem::containsPhone()
{

    return (!m_addressee->phoneNumbers().isEmpty());    
    
}

bool ContactItem::containsMail()
{

    return (!m_addressee->emailLabel().isEmpty());
    
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





