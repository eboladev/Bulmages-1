#ifndef PLBLEMBED_H
#define PLBLEMBED_H

#ifndef Q_OS_WIN32

#include <QtCore/QObject>



class PluginBl_Embed : public QObject
{
    Q_OBJECT

public:
   
    PluginBl_Embed();
    virtual ~PluginBl_Embed();

public slots:
    virtual void elslot();
    virtual void elslot1();
    virtual void embedPYS();
};


#endif
#endif
