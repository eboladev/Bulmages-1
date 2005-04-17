/***************************************************************************
                          propiedadesempresa.cpp  -  description
                             -------------------
    begin                : Fri Nov 29 2002
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

#include <qmessagebox.h>
 
#include "propiedadesempresa.h"
using namespace std;

propiedadesempresa::propiedadesempresa(QWidget *parent, const char *name, bool modal) : propiedemp(parent,name,modal) , dialogChanges(this) {
}// end propiedadesempresa


propiedadesempresa::~propiedadesempresa() {}// end ~propiedadesempresa


int propiedadesempresa::inicializa(postgresiface2 *conn) {
    int num;
    int i;

    QString query = "SELECT * FROM configuracion WHERE nombre='CodCuenta'";
    conexionbase = conn;
    conexionbase->begin();
    cursor2 *curs = conexionbase->cargacursor(query,"query");
    conexionbase->commit();
    num = curs->numregistros();
    if (num >0) {
        modcodigo->setText(curs->valor(2));
    }// end if
    delete curs;

    tpropiedades->setNumCols(2);
    query = "SELECT * FROM configuracion";
    conexionbase->begin();
    curs = conexionbase->cargacursor(query,"queryconf");
    conexionbase->commit();
    num=curs->numregistros();
    tpropiedades->setNumRows(100);
    i=0;
    while (!curs->eof()) {
        tpropiedades->setText(i,0,curs->valor("nombre"));
        tpropiedades->setText(i,1,curs->valor("valor"));
        curs->siguienteregistro();
        i++;
    }// end while
    delete curs;


    //Este bloque de código realiza la consulta para obtener los datos fiscales de la empresa
    QString empresa, ano;


    query="select nombre,valor from configuracion";//Tiene que usar la empresa elegida, no bulmages!!!! TODO

    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(query,"datos");
    conexionbase->commit();
    
    QString n,v;
    int nTuples=cur->numregistros();
    QLineEdit *p;
    for (int i=0;i<nTuples;i++) {
        p=NULL;
        n=cur->valor("nombre");
        v=cur->valor("valor");
        cur->siguienteregistro();

        if (n=="NombreEmpresa")
            p=m_nomEmpresa;
        if (n=="CIF")
            p=lineCIF;
        if (n=="TipoVia")
            p=lineTipoVia;
        if (n=="NombreVia")
            p=lineNombreVia;
        if (n=="NumeroVia")
            p=lineNumeroVia;
        if (n=="Escalera")
            p=lineEscalera;
        if (n=="Piso")
            p=linePiso;
        if (n=="Puerta")
            p=linePuerta;
        if (n=="CodPostal")
            p=lineCodPostal;
        if (n=="Municipio")
            p=lineMunicipio;
        if (n=="Provincia")
            p=lineProvincia;
        if (n=="Pais")
            p=linePais;

        if (p) {
            cout << n << " = " << v << "\n";
            p->setText(v);
        }
    }
    delete cur;
    
    dialogChanges_cargaInicial(); 
    
    return(0);
}// end inicializa


void propiedadesempresa::s_saveConfig() {
    QString query = "DELETE FROM configuracion";
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
    int i=0;
    while (tpropiedades->text(i,0) != "") {
        QString SQLQuery;
        SQLQuery.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (%d,'%s','%s')",i,tpropiedades->text(i,0).ascii(),tpropiedades->text(i,1).ascii());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
        i++;
    }// end while

    //Este bloque de codigo guarda los datos fiscales en la tabla configuracion
    conexionbase->begin();
    update_value(conexionbase,"NombreEmpresa",m_nomEmpresa->text());
    update_value(conexionbase,"CIF",lineCIF->text());
    update_value(conexionbase,"TipoVia",lineTipoVia->text());
    update_value(conexionbase,"NombreVia",lineNombreVia->text());
    update_value(conexionbase,"NumeroVia",lineNumeroVia->text());
    update_value(conexionbase,"Escalera",lineEscalera->text());
    update_value(conexionbase,"Piso",linePiso->text());
    update_value(conexionbase,"Puerta",linePuerta->text());
    update_value(conexionbase,"CodPostal",lineCodPostal->text());
    update_value(conexionbase,"Municipio",lineMunicipio->text());
    update_value(conexionbase,"Provincia",lineProvincia->text());
    update_value(conexionbase,"Pais",linePais->text());
    conexionbase->commit();
    dialogChanges_cargaInicial(); 
    
    //done(1);
}// end accept


void propiedadesempresa::update_value(postgresiface2 *m,QString n,QString v) {
    QString query="SELECT * from configuracion where nombre='"+n+"'";
    cursor2 *cur = m->cargacursor(query,"configuracion");
    if (cur->numregistros()==0) {
        query.sprintf("INSERT into configuracion (idconfiguracion,nombre,valor) values ((select max(idconfiguracion)+1 from configuracion),'%s','%s')",n.ascii(),v.ascii());
    } else
        query="UPDATE configuracion set valor='"+v+"' where nombre='"+n+"'";

    delete cur;
    cout << m->ejecuta(query) << "\n";

}


void propiedadesempresa::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Cambios",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		s_saveConfig();	
    }// end if
    QDialog::close();
}// end close
