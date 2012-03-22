#ifndef PLBLREPORT
#define PLBLREPORT

#include <QObject>



class PluginBl_Report : public QObject
{
    Q_OBJECT

public:
    PluginBl_Report();
    virtual ~PluginBl_Report();

public slots:
    virtual void elslot1();
    virtual void elslot2();
};


#endif

