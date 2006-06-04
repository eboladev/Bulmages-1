

#ifndef ASIENTOLISTSUBFORM
#define ASIENTOLISTSUBFORM


#include "subform2bc.h"


/** @author Tomeu Borras Riera 
  * \brief Muestra un listado de todos los asientos existentes e informa de si estan abiertos o cerrados.
*/


class AsientoListSubform : public SubForm2Bc {
    Q_OBJECT
public:
    AsientoListSubform(QWidget *parent = 0, const char *name = 0);
    virtual ~AsientoListSubform() {}
    ;
public slots:
    virtual void contextMenuEvent (QContextMenuEvent *);
};

#endif

