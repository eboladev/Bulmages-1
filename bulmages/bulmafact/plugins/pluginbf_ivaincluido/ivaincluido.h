#ifndef IVAINCLUIDO_H
#define IVAINCLUIDO_H

#include "blwidget.h"
#include "articuloview.h"
#include "bfcompany.h"


class BfCompany;

class IVAincluido : public BlWidget
{
    Q_OBJECT
    
private:
    ArticuloView *m_trab;
    BlDoubleSpinBox *m_ivainc;
    
public:
    IVAincluido(ArticuloView *trab);
    ~IVAincluido();

public slots:
    void botonTranspasa_clicked();
};


#endif

