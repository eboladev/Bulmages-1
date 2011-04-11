#ifndef BFSUBFORMTASKMENU_H
#define BFSUBFORMTASKMENU_H

#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>

QT_BEGIN_NAMESPACE
class QAction;
class QExtensionManager;
QT_END_NAMESPACE
class BfSubForm;


class BfSubFormTaskMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)

public:
    BfSubFormTaskMenu(BfSubForm *sf, QObject *parent);

    QAction *preferredEditAction() const;
    QList<QAction *> taskActions() const;

private slots:
    void editState();

private:
    QAction *editStateAction;
    BfSubForm *bfsf;
};



class BfSubFormTaskMenuFactory : public QExtensionFactory
{
    Q_OBJECT

public:
    BfSubFormTaskMenuFactory(QExtensionManager *parent = 0);

protected:
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};

#endif
