#ifndef IVAINCLUIDOVIEW_H
#define IVAINCLUIDOVIEW_H

#include "blwidget.h"
#include "articuloview.h"
#include "bfcompany.h"

#include "ui_ivaincluidobase.h"


class BfCompany;

class IVAincluidoView : public BlWidget, public Ui_IVAincluidoBase
{
    Q_OBJECT
    
private:
    ArticuloView *m_trab;
    
public:
    IVAincluidoView(ArticuloView *trab);
    ~IVAincluidoView();

public slots:
    void on_mui_calcula_iva_inc_clicked();
    void on_mui_calcula_sin_iva_clicked();
};


#endif

