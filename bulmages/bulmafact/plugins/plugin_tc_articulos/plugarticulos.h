
#ifndef PLUGARTICULOS_H
#define PLUGARTICULOS_H

#include <QObject>
#include "bulmafact.h"

class plugin_tc_articulos : public QObject
{
    Q_OBJECT

public:
    postgresiface2 *conexionbase;
    Bulmafact *m_bulmafact;

public:
    plugin_tc_articulos();
    ~plugin_tc_articulos();
    void inicializa ( Bulmafact * );

public slots:
    void tallas();
    void colores();
};

#endif

