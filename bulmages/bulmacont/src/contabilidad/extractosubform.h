

#ifndef _EXTRACTO_SUBFORM
#define _EXTRACTO_SUBFORM

#include "subform2bc.h"

class ExtractoSubForm : public SubForm2Bc {
    Q_OBJECT
public:
    ExtractoSubForm(QWidget *parent = 0, const char *name = 0);
    virtual ~ExtractoSubForm() {}
    ;
public slots:
    virtual void contextMenuEvent (QContextMenuEvent *);
};


#endif

