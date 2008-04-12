#ifndef MYPLUGINIMPORTCSV_H
#define MYPLUGINIMPORTCSV_H

#include <QObject>


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

