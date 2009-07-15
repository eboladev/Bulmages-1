

#ifndef BLTREEWIDGET_H
#define BLTREEWIDGET_H


#include <QTreeWidget>


class BlTreeWidget : public QTreeWidget {
Q_OBJECT
public:
   BlTreeWidget ( QWidget * parent = 0 );
   ~BlTreeWidget ();
};

#endif

