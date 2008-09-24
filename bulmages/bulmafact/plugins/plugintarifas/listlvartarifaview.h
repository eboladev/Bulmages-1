
#ifndef LISTLVARTARIFAVIEW_H
#define LISTLVARTARIFAVIEW_H

#include <QObject>
#include "bulmafact.h"
#include "subform2bf.h"


class QSubFormVarTarifaBfDelegate : public QSubForm2BfDelegate
{
public:
    QSubFormVarTarifaBfDelegate ( QObject * );
    virtual ~QSubFormVarTarifaBfDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif

