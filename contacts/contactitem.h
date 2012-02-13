#ifndef CONTACTITEM_H
#define CONTACTITEM_H

#include <QObject>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include <KABC/Addressee>

#include <Plasma/IconWidget>
#include <Plasma/Label>


class ContactItem : public Plasma::IconWidget
{
    Q_OBJECT

public:
    ContactItem(KABC::Addressee * addr, QGraphicsWidget *parent = 0);
    
    virtual ~ContactItem() { };
    
    const KABC::Addressee * getAddressee() { return addressee; }
    
    bool containsString(QString string);
    
    // TODO
    void hideContact();
    void showContact();
    
public slots:
    
    
private:
        
    void setWidgetText();
    void setTooltipText();
    
    KABC::Addressee * addressee;
    
};


#endif // CONTACTITEM_H
