#ifndef PLBLREPORT2ODS
#define PLBLREPORT2ODS

#include <QtCore/QObject>


class PluginBl_Report2ODS : public QObject
{
    Q_OBJECT

public:
    PluginBl_Report2ODS();
    virtual ~PluginBl_Report2ODS();

public slots:
    virtual void elslot1();
};


#endif

