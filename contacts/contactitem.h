#ifndef CONTACTITEM_H
#define CONTACTITEM_H

#include <QObject>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <KABC/Addressee>

#include <Plasma/IconWidget>
#include <Plasma/Label>
#include <Plasma/ToolTipContent>

class ContactItem : public Plasma::IconWidget
{
    Q_OBJECT

public:
    ContactItem(KABC::Addressee * addr, QGraphicsWidget *parent = 0);
    
    virtual ~ContactItem() { };
    
    const KABC::Addressee * addressee() { return m_addressee; }
    
    bool containsString(const QString & string);
    
    void setShowInfo(bool emails, bool numbers);

    
    // TODO
    bool containsMail();
    bool containsPhone();
    
    // TODO
    void hideContact();
    void showContact();
    
public slots:
    
    
private:
        
    void setWidgetText();
    
    KABC::Addressee * m_addressee;
        
};


#endif // CONTACTITEM_H
