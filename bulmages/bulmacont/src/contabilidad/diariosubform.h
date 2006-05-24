

#ifndef _DIARIO_SUBFORM
#define _DIARIO_SUBFORM

#include "subform2bc.h"

class DiarioSubForm : public SubForm2Bc {
    Q_OBJECT
public:
    DiarioSubForm(QWidget *parent = 0, const char *name = 0);
    virtual ~DiarioSubForm() {}
    ;
public slots:
    virtual void contextMenuEvent (QContextMenuEvent *);
};


#endif

