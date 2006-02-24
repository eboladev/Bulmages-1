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
#ifndef CORRECTORWIDGET_H
#define CORRECTORWIDGET_H

#include <qstring.h>
#include <q3dockwindow.h>
#include "correctorwdt.h"
#include "postgresiface2.h"
#include "empresa.h"

/** @author Tomeu Borras
  * \brief Busca errores y incoherencias en la contabilidad y las reporta al usuario en forma de listado.
  *
  * Esta clase realiza una serie de consultas determinadas en busca de determinados errores contables como puedan ser asientos no cerrados, amortizaciones con plazos expirados, cobros no realizados, etc
  * y realiza un informe de los errores encontrados para pasarselos al usuario. Tambi� proporciona el acceso de forma sencilla a los errores encontrados. 
  * El corrector s�o tiene una instancia en toda la ejecuci� del programa, es la clase empresa la que se encarga de construirlo y una vez construido permanece siempre en ejecuci�.
 */



class correctorwidget : public correctorwdt
{
Q_OBJECT
public:
	/// Base de datos con la que se est�operando.
	postgresiface2 *conexionbase;
	/// Empresa que ha hecho la instancia del corrector.
	empresa *empresaactual;
	/// El informe generado se forma en HTML y se presenta mediante este QString
	QString textBrowser;
	/// El corrector es una ventana del tipo dock, que se puede anexar a las esquinas del worspace de la aplicaci�. Este puntero apunta a la ventana contenedora del corrector.
	Q3DockWindow *dock;
public:
    correctorwidget( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~correctorwidget();
    /// El corrector, al contrario de todos los dem� elementos tiene la inicializaci� de la base de datos y de la empresa realizada
    /// de forma diferente, debe usarse la funci� setEmpresa para inicializar el objeto. Esto es debido a que la construcci� del corrector
    /// es anterior a la construcci� de la clase empresa.
	void setEmpresa(empresa *empres) { empresaactual = empres; conexionbase = empres->bdempresa();}
	void agregarError(QString, QString, QString);
public slots:
	virtual void corregir();
	virtual void link(const QString &);
	virtual void alink(const QString &, const QString &);
	virtual void s_configurarReglas();
	virtual void cambia(bool a) {
	       if (a) {
		 dock->hide();
		 dock->show();
		 dock->showMaximized();
	       } else dock->hide();};
};

#endif
