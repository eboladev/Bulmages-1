//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AMORTIZACIONDLGAMORTIZACIONVIEW_H
#define AMORTIZACIONDLGAMORTIZACIONVIEW_H

#include "amortizaciondlg.h"
#include "postgresiface2.h"

class empresa;

/**@author Tomeu Borras*/
class amortizacionview : public amortizaciondlg
{
Q_OBJECT
public:
empresa *empresaactual;
postgresiface2 *conexionbase;
QString idctaactivo; // Esta variable indica cual es el indice de la cuenta de activo de la amortizacion que estamos viendo. Si no se está mostrando una cuenta este valor vale "".
QString idctaamortizacion; // Esta variable indica cual es el indice de la cuenta de amortizacion de la amortizacion que estamos viendo. Si no se está mostrando una cuenta este valor vale "".

QString idamortizacion;  // Esta variable indica cual es la amortizacion que se està enseñando. Si no tiene valor es que no se esta mostrando ninguna.
QString idainteligente;   // Esta variable indica cual es el asiento plantilla de amortización.
public:
    amortizacionview::amortizacionview(empresa *, QWidget *, const char *, bool );
    ~amortizacionview();
    void inicializa(QString);
public slots:
	virtual void accept();
	virtual void cambiofechacompra();
	virtual void cambiofecha1cuota();
        virtual void calculaamortizacion();
        virtual void contextMenuRequested(int, int, const QPoint &);
        virtual void buscactaactivo();
        virtual void buscactaamortizacion();
        virtual void codigo_textChanged(const QString &);
        virtual void codigo_lostfocus();
        virtual void return_ctaactivo();
        virtual void return_ctaamortizacion();
};
#endif
