#ifndef PLBLBLOQCONF_H
#define PLBLBLOQCONF_H

#include <QtCore/QObject>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>


class PluginBl_BloqConf : public QObject
{
    Q_OBJECT

public:
    PluginBl_BloqConf();
    virtual ~PluginBl_BloqConf();

public slots:
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};


#endif

