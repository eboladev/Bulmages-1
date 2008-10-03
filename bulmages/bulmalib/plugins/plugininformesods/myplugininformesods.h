#ifndef MYPLUGININFORMESODS_H
#define MYPLUGININFORMESODS_H

#include <QObject>



class MyPluginInformesODS : public QObject
{
    Q_OBJECT

public:
    MyPluginInformesODS();
    virtual ~MyPluginInformesODS();

public slots:
    virtual void elslot1();
};


#endif

