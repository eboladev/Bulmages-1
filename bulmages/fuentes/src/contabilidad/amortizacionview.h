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
#include "dialogchanges.h"

class empresa;

/** 
  * @author Tomeu Borr� Riera 
  * \class amortizacionview amortizacionview.h
  * \brief Formulario de introducci� y/o visi� de amortizaciones.
  */
  
class amortizacionview : public amortizaciondlg , dialogChanges {
Q_OBJECT
public:
/// Empresa con la que se trabaja
empresa *empresaactual;
/// Base de datos con la que se trabaja  
postgresiface2 *conexionbase;
/// Esta variable indica cual es el indice de la cuenta de activo de la amortizacion que estamos viendo. Si no se est�mostrando una cuenta este valor vale "".
QString idctaactivo; 
 /// Esta variable indica cual es el indice de la cuenta de amortizacion de la amortizacion que estamos viendo. Si no se est�mostrando una cuenta este valor vale "".
QString idctaamortizacion;
/// Esta variable indica cual es la amortizacion que se est�ense�ndo. Si no tiene valor es que no se esta mostrando ninguna.
QString idamortizacion;  
/// Esta variable indica cual es el asiento plantilla de amortizaci�.
QString idainteligente;   
public:
    amortizacionview::amortizacionview(empresa *, QWidget *, const char *, bool );
    ~amortizacionview();
    void inicializa(QString);
    void trataModificado();
public slots:
	virtual bool close(bool);
	virtual void accept();
        virtual void calculaamortizacion();
        virtual void contextMenuRequested(int, int, const QPoint &);
	virtual void s_saveAmortizacion();
	virtual void s_newAmortizacion();
	virtual void s_deleteAmortizacion();
};
#endif
