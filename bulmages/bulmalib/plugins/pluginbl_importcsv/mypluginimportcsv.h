#ifndef MYPLUGINIMPORTCSV_H
#define MYPLUGINIMPORTCSV_H

#include <QObject>

#include "blfunctions.h"


class mypluginimportcsv : public QObject
{
    Q_OBJECT

public:
    mypluginimportcsv();
    virtual ~mypluginimportcsv();

public slots:
    virtual void elslot();
};

#endif
