

#ifndef _BALANCE_SUBFORM
#define _BALANCE_SUBFORM

#include "subform2bc.h"

class BalanceSubForm : public SubForm2Bc {
    Q_OBJECT
public:
    BalanceSubForm(QWidget *parent = 0, const char *name = 0);
    virtual ~BalanceSubForm() {}
    ;
public slots:
    virtual void contextMenuEvent (QContextMenuEvent *);
};


#endif

