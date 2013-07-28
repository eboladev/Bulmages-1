#include <QtDesigner>
#include <QtGui/QtGui>

#include "bfsubform.h"
#include "bfsubformdialog.h"
#include "bfsubformtaskmenu.h"


BfSubFormTaskMenu::BfSubFormTaskMenu(BfSubForm *sf, QObject *parent)
    : QObject(parent)
{
    bfsf = sf;

    editStateAction = new QAction(tr("Editar campos..."), this);
    connect(editStateAction, SIGNAL(triggered()), this, SLOT(editState()));
}


void BfSubFormTaskMenu::editState()
{
    BfSubFormDialog dialog(bfsf);
    dialog.exec();
}


QAction *BfSubFormTaskMenu::preferredEditAction() const
{
    return editStateAction;
}


QList<QAction *> BfSubFormTaskMenu::taskActions() const
{
    QList<QAction *> list;
    list.append(editStateAction);
    return list;
}


BfSubFormTaskMenuFactory::BfSubFormTaskMenuFactory(QExtensionManager *parent)
    : QExtensionFactory(parent)
{
}


QObject *BfSubFormTaskMenuFactory::createExtension(QObject *object,
                                                   const QString &iid,
                                                   QObject *parent) const
{
    if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
        return 0;

    if (BfSubForm *bfsf = qobject_cast<BfSubForm*>(object))
        return new BfSubFormTaskMenu(bfsf, parent);

    return 0;
}

