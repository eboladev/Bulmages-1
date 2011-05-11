#ifndef BLPLREPORT_H
#define BLPLREPORT_H

#include <QObject>
#include <QMenu>
#include <QAction>


class PluginBl_BloqConf : public QObject
{
    Q_OBJECT

public:
    PluginBl_BloqConf();
    virtual ~PluginBl_BloqConf();

public slots:
    virtual void elslot();
    virtual void elslot1();
    virtual void elslot2();
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};


#endif

