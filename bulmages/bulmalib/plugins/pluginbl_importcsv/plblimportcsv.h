#ifndef PLBLIMPORTCSV
#define PLBLIMPORTCSV

#include <QtCore/QObject>

#include "blfunctions.h"


class PluginBl_ImportCSV : public QObject
{
    Q_OBJECT

public:
    PluginBl_ImportCSV();
    virtual ~PluginBl_ImportCSV();

public slots:
    virtual void elslot();
};

#endif
