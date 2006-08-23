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

#include "funcaux.h"
#include "propiedadesempresa.h"
#include "empresa.h"

using namespace std;

propiedadesempresa::propiedadesempresa(empresa *emp, QWidget *parent) : QWidget(parent, Qt::WDestructiveClose) , dialogChanges(this) {
	_depura("propiedadesempresa::propiedadesempresa", 0);
	setupUi(this);
        m_companyact = emp;
        inicializa();
        m_companyact->meteWindow(windowTitle(), this);
	_depura("END propiedadesempresa::propiedadesempresa", 0);
}


propiedadesempresa::~propiedadesempresa() {
	_depura("propiedadesempresa::~propiedadesempresa", 0);
	m_companyact->sacaWindow(this);
	_depura("END propiedadesempresa::~propiedadesempresa", 0);

}


int propiedadesempresa::inicializa() {
    int num;
    int i;

    QString query = "SELECT * FROM configuracion WHERE nombre='CodCuenta'";
    cursor2 *curs = m_companyact->cargacursor(query);
    num = curs->numregistros();
    if (num >0)   {
        modcodigo->setText(curs->valor("valor"));
    }// end if
    delete curs;

    tpropiedades->setNumCols(2);

    tpropiedades->horizontalHeader()->setLabel( 0, tr( "Propiedad" ) );
    tpropiedades->horizontalHeader()->setLabel( 1, tr( "Valor" ) );

    query = "SELECT * FROM configuracion";
    m_companyact->begin();
    curs = m_companyact->cargacursor(query,"queryconf");
    m_companyact->commit();
    num=curs->numregistros();
    tpropiedades->setNumRows(100);
    i=0;
    while (!curs->eof())   {
        tpropiedades->setText(i,0,curs->valor("nombre"));
        tpropiedades->setText(i,1,curs->valor("valor"));
        curs->siguienteregistro();
        i++;
    }// end while
    delete curs;


    //Este bloque de código realiza la consulta para obtener los datos fiscales de la empresa
    QString empresa, ano;


    query="select nombre,valor from configuracion";//Tiene que usar la empresa elegida, no bulmages!!!! TODO

    m_companyact->begin();
    cursor2 *cur = m_companyact->cargacursor(query,"datos");
    m_companyact->commit();

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
            cout << n.ascii() << " = " << v.ascii() << "\n";
            p->setText(v);
        }
    }
    delete cur;

    dialogChanges_cargaInicial();

    return(0);
}// end inicializa


void propiedadesempresa::s_saveConfig() {
    _depura("propiedadesempresa::s_saveConfig", 0);
    QString query = "DELETE FROM configuracion";
    m_companyact->ejecuta(query);
    int i=0;
    while (tpropiedades->text(i,0) != "") {
        QString SQLQuery;
        SQLQuery.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (%d,'%s','%s')",i,
                         m_companyact->sanearCadena(tpropiedades->text(i,0)).ascii(),
                         m_companyact->sanearCadena(tpropiedades->text(i,1)).ascii());
        m_companyact->ejecuta(SQLQuery);
        i++;
    }// end while

    //Este bloque de codigo guarda los datos fiscales en la tabla configuracion
    update_value("NombreEmpresa",m_nomEmpresa->text());
    update_value("CIF",lineCIF->text());
    update_value("TipoVia",lineTipoVia->text());
    update_value("NombreVia",lineNombreVia->text());
    update_value("NumeroVia",lineNumeroVia->text());
    update_value("Escalera",lineEscalera->text());
    update_value("Piso",linePiso->text());
    update_value("Puerta",linePuerta->text());
    update_value("CodPostal",lineCodPostal->text());
    update_value("Municipio",lineMunicipio->text());
    update_value("Provincia",lineProvincia->text());
    update_value("Pais",linePais->text());
    dialogChanges_cargaInicial();
    _depura("END propiedadesempresa::s_saveConfig", 0);

}


void propiedadesempresa::update_value(QString n,QString v) {
    _depura("propiedadesempresa::update_value", 0);
    QString query="SELECT * from configuracion where nombre='"+n+"'";
    cursor2 *cur = m_companyact->cargacursor(query,"configuracion");
    if (cur->numregistros()==0) {
        query.sprintf("INSERT into configuracion (idconfiguracion,nombre,valor) values ((select max(idconfiguracion)+1 from configuracion),'%s','%s')",n.ascii(),v.ascii());
    } else
        query="UPDATE configuracion set valor='"+v+"' where nombre='"+n+"'";
    delete cur;
    m_companyact->ejecuta(query);
}


bool propiedadesempresa::close(bool ok) {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::question( this, tr("Guardar Cambios"),
                                    tr("Desea guardar los cambios."),
                                    tr("Guardar"), tr("No Guardar") ,0,0,1) == 0) {
            s_saveConfig();
        }// end if
    }// end if
    return QWidget::close(ok);
}// end close

/** Esste SLOT corresponde a la pulsación del botón de modificar plan contable
    de la pantalla de configuracion.
    Coge todas las cuentas del plan contable y trata sus códigos de forma que los extiende
    o reduce al tamaño especificado en el parametro modcodigo.
    De momento esta función está incompleta y hace varias consideraciones que deberían ir solucionandose (no trata los digitos de cuenta y subcuenta) por lo que siempre considera que las cuentas son 4 digitos.
*/
void propiedadesempresa::extiendeCuentas() {
    unsigned int nlong = modcodigo->text().length();
    QString codigo;
    QString query = "SELECT * FROM cuenta";
    cursor2 *cur = m_companyact->cargacursor(query);
    while (! cur->eof()) {
        codigo = cur->valor("codigo");
        codigo = ajustacodigo(codigo,nlong);
        m_companyact->begin();
        query = "UPDATE cuenta SET codigo='"+codigo+"' WHERE idcuenta="+cur->valor("idcuenta");
        m_companyact->ejecuta(query);
        m_companyact->commit();
        cur->siguienteregistro();
    }// end while
    delete cur;
    query="UPDATE configuracion SET valor='"+modcodigo->text()+"' WHERE nombre='CodCuenta'";
    m_companyact->begin();
    m_companyact->ejecuta(query);
    m_companyact->commit();
    if ( QMessageBox::warning( this, "Salir",
                               "Para que los cambios tengan efecto, debe salir del programa y volver a entrar. ¿Salir ahora?",
                               "Salir", "No Salir",0, 0, 1) == 0) {
        exit(1);
    }// end if
    dialogChanges_cargaInicial();
}// end extiendeCuentas
