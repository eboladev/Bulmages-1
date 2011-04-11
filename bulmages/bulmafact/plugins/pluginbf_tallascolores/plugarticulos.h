
#ifndef PLUGARTICULOS_H
#define PLUGARTICULOS_H

#include <QObject>
#include "bfbulmafact.h"
#include "bfsubform.h"


class plugin_tc_articulos : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;
    BfBulmaFact *m_bulmafact;

public:
    plugin_tc_articulos();
    ~plugin_tc_articulos();
    void inicializa ( BfBulmaFact * );

public slots:
    void tallas();
    void colores();
};


/// Clase BfSubFormDelegate
/** Se encarga del control de los Widgets de Edicion del sistema.*/
class QSubForm3BfDelegate : public BfSubFormDelegate
{
public:
    QSubForm3BfDelegate ( QObject * );
    virtual ~QSubForm3BfDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif
