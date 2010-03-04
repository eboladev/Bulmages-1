

#ifndef BLTREEWIDGET_H
#define BLTREEWIDGET_H

#include <QTreeWidget>

#include "blfunctions.h"


class BL_EXPORT BlTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    BlTreeWidget ( QWidget * parent = 0 );
    ~BlTreeWidget ();

    virtual void creaMenu ( QMenu * );
    virtual void procesaMenu ( QAction * );

public slots:
    virtual void contextMenuEvent ( QContextMenuEvent * );

signals:
    void pintaMenu ( QMenu * );
    void trataMenu ( QAction * );

};

#endif

