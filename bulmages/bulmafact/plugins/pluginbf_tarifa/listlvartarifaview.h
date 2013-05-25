
#ifndef LISTLVARTARIFAVIEW_H
#define LISTLVARTARIFAVIEW_H

#include <QtCore/QObject>
#include "bfbulmafact.h"
#include "bfsubform.h"


class QSubFormVarTarifaBfDelegate : public BfSubFormDelegate
{
public:
    QSubFormVarTarifaBfDelegate ( QObject * );
    virtual ~QSubFormVarTarifaBfDelegate();
    virtual void setEditorData ( QWidget *, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index ) const;
    virtual QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif

