#ifndef PLASMA_CALENDAR_HEADER
#define PLASMA_CALENDAR_HEADER

#include <KIcon>
 
#include <Plasma/Applet>
 
 
class PlasmaCalendar : public Plasma::Applet
{
    Q_OBJECT
    public:
	
        PlasmaCalendar(QObject *parent, const QVariantList &args);
        ~PlasmaCalendar();
 
        void paintInterface(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
	void init();
 
    private:
        KIcon m_icon;
};
 
K_EXPORT_PLASMA_APPLET(akonadi_calendar, PlasmaCalendar)

#endif