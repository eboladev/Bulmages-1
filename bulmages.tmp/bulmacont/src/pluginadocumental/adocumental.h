/***************************************************************************
                          aboutview.h  -  description
                             -------------------
    begin                : Fri Dec 27 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ADOCUMENTAL_H
#define ADOCUMENTAL_H

#include <qwidget.h>
#include "adocumentalbase.h"
#include "postgresiface2.h"

#include <qmessagebox.h>
class empresa;

class myplugin1 : public QObject {
    Q_OBJECT
public:
    empresa *empresaactual;
    postgresiface2 *conexionbase;
public:
    myplugin1(empresa *);
    ~myplugin1();
public slots:
    virtual void elslot();
    virtual void boton_adjuntar();
    virtual void boton_nuevoasientodocumental();
    virtual void archDoc();
};



/**  *@author Tomeu Borrás Riera  */

class empresa;

class adocumental : public adocumentalbase  {
   Q_OBJECT
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
   QString RutaADocumental;
/* En el modo edicion la ventana responde como si fuese un listado sobre el que se pueden editar los elementos la hacer doble click sobre ellos */
/* En el modo consulta la ventana responde como si fuese un desplegable y se puede seleccionar el elemento con el que se desea tratar */
   
   int modo;  // Si modo vale 0 entonces es modo edicion
              // Si modo vale 1 entonces es modo consulta
              
   QString idadocumental; // Almacena por un breve lapso de tiemo el idadocumental que se ha seleccionado.
public: 
	adocumental(empresa *, QWidget *parent=0, const char *name=0);
	~adocumental();
        void setmodoedicion() {modo=0;};
        void setmodoconsulta() {modo=1;};
        QString getidadocumental();
        void asociaasiento(QString);
        void presentaprimervacio();
        void inicializa(); // A veces es necesario repintar las cosas. Esta funcion es la que lo hace.
	void newADocumental(QString);
public slots:
   virtual void doubleclicked(int, int, int, const QPoint &);
   virtual void boton_newadocumental();
   virtual void boton_desasociar();
   virtual void s_deleteADocumental();   
   virtual void s_saveADocumental();
   virtual void s_agregarDirectorio();
};

#endif
