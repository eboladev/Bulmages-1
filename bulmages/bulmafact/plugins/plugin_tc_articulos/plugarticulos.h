
#ifndef PLUGARTICULOS_H
#define PLUGARTICULOS_H

#include <QObject>
#include "bulmafact.h"
#include "subform2bf.h"

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


/// Clase SubForm2BfDelegate
/** Se encarga del control de los Widgets de Edicion del sistema.*/
class QSubForm3BfDelegate : public QSubForm2BfDelegate
{
public:
    QSubForm3BfDelegate ( QObject * );
    virtual ~QSubForm3BfDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};


#endif

