/***************************************************************************
                          aboutview.cpp  -  description
                             -------------------
    begin                : Fri Dec 27 2004
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

#include "adocumental.h"
#include "empresa.h"

#include <qtextbrowser.h>



#define COL_IDADOCUMENTAL 0
#define COL_IDASIENTO 1
#define COL_DESCRIPCIONADOCUMENTAL 2
#define COL_FECHAINTADOCUMENTAL 3
#define COL_FECHAASADOCUMENTAL 4
#define COL_ARCHIVOADOCUMENTAL 5
#define COL_ORDENASIENTO 6


myplugin1::myplugin1(empresa *emp) {
    empresaactual=emp;
    conexionbase=emp->bdempresa();
}// end myplugin1

myplugin1::~myplugin1() {}

// Esto debe coger un asiento y asociarle un archivo documental y abrirlo y enseï¿½r ambas cosas
void myplugin1::boton_nuevoasientodocumental() {
    fprintf(stderr,"boton_nuevoasientodocumental\n");
    adocumental *adoc= new adocumental(empresaactual,0,"adjuntar documento");
    adoc->presentaprimervacio();
    intapunts3view *intapunts = empresaactual->intapuntsempresa();
    intapunts->iniciar_asiento_nuevo();
    adoc->asociaasiento(intapunts->idAsiento());
    delete adoc;
}// end boton_nuevoasientodocumental


void myplugin1::boton_adjuntar() {
    adocumental *adoc= new adocumental(empresaactual,0,"adjuntar documento");
    adoc->setmodoconsulta();
    adoc->exec();
    /// FAlta por resolver esta salvedad
    intapunts3view *intapunts = empresaactual->intapuntsempresa();
    adoc->asociaasiento(intapunts->idAsiento());
    delete adoc;
}// end boton_adjuntar

/** Esta función se dispara para poner en marcha el archivo documental.
// El archivo documental es una opción mendiante la cual se pueden poner
// Junto a los asientos y otras entidades una seríe de documentos ligados
// Como puden ser PDF's, GIFS, Archivos de Sonido, etc etc etc.
// La idea principal es que se pueda conectar un scaner y se puedan escanear
// Las imagenes de facturas para después pasarlas.
*/
void myplugin1::archDoc() {
    adocumental *adoc= new adocumental(empresaactual,0,"Archivo Documental.");
    adoc->exec();
    delete adoc;
}// archDoc

void myplugin1::elslot() {
    fprintf(stderr,"SE ha activado el slot\n");
    QMessageBox::warning( 0, "Guardar Familia",
                          "Desea guardar los cambios.",
                          QMessageBox::Ok ,
                          QMessageBox::Cancel );
}// end elslot




adocumental::adocumental(empresa *emp,QWidget *parent, const char *name ) : adocumentalbase(parent,name,false,0) {
    empresaactual=emp;
    conexionbase = emp->bdempresa();
    modo =0;
    idadocumental = "";
    QString query;

    m_listado->setNumRows(0);
    m_listado->setNumCols(7);
    m_listado->horizontalHeader()->setLabel( COL_IDADOCUMENTAL, tr( "idarchivodocumental" ) );
    m_listado->horizontalHeader()->setLabel( COL_IDASIENTO, tr( "idasiento" ) );
    m_listado->horizontalHeader()->setLabel( COL_DESCRIPCIONADOCUMENTAL, tr( "Descripcion" ) );
    m_listado->horizontalHeader()->setLabel( COL_FECHAINTADOCUMENTAL, tr( "Fecha Doc" ) );
    m_listado->horizontalHeader()->setLabel( COL_FECHAASADOCUMENTAL, tr( "Fecah Asoc" ) );
    m_listado->horizontalHeader()->setLabel( COL_ARCHIVOADOCUMENTAL, tr( "Archivo" ) );
    m_listado->horizontalHeader()->setLabel( COL_ORDENASIENTO, tr( "Asiento" ) );

    m_listado->setColumnWidth(COL_IDADOCUMENTAL,200);
    m_listado->setColumnWidth(COL_IDASIENTO,200);
    m_listado->setColumnWidth(COL_DESCRIPCIONADOCUMENTAL,200);
    m_listado->setColumnWidth(COL_FECHAINTADOCUMENTAL,50);
    m_listado->setColumnWidth(COL_FECHAASADOCUMENTAL,50);
    m_listado->setColumnWidth(COL_ARCHIVOADOCUMENTAL,250);
    m_listado->setColumnWidth(COL_ORDENASIENTO,75);

    m_listado->hideColumn(COL_IDADOCUMENTAL);
    m_listado->hideColumn(COL_IDASIENTO);
    m_listado->hideColumn(COL_FECHAINTADOCUMENTAL);
    m_listado->hideColumn(COL_FECHAASADOCUMENTAL);

/*
    query = "SELECT * FROM configuracion WHERE nombre='RutaADocumental'";
    conexionbase->begin();
    cursor2 *cursoraux2=conexionbase->cargacursor(query,"thequery");
    conexionbase->commit();
    if (!cursoraux2->eof()) {
        RutaADocumental = cursoraux2->valor("valor");
    } else {
        RutaADocumental = "/tmp";
    }// end if
*/
    // Iniciamos la presentación
    inicializa();
}// end adocumental

adocumental::~adocumental() {}// end adocumental


void adocumental::inicializa() {
    QString query = "SELECT * FROM adocumental LEFT JOIN asiento ON adocumental.idasiento=asiento.idasiento ORDER BY ordenasiento";
    conexionbase->begin();
    cursor2 *cursoraux1=conexionbase->cargacursor(query,"elquery");
    conexionbase->commit();
    m_listado->setNumRows(cursoraux1->numregistros());
    int i=0;
    while (!cursoraux1->eof()) {
        m_listado->setText(i,COL_IDADOCUMENTAL, cursoraux1->valor("idadocumental"));
        m_listado->setText(i,COL_IDASIENTO, cursoraux1->valor("idasiento"));
        m_listado->setText(i,COL_DESCRIPCIONADOCUMENTAL, cursoraux1->valor("descripcionadocumental"));
        m_listado->setText(i,COL_FECHAINTADOCUMENTAL, cursoraux1->valor("fechaintadocumental"));
        m_listado->setText(i,COL_FECHAASADOCUMENTAL, cursoraux1->valor("fechaasadocumental"));
        m_listado->setText(i,COL_ARCHIVOADOCUMENTAL, cursoraux1->valor("archivoadocumental"));
        m_listado->setText(i,COL_ORDENASIENTO, cursoraux1->valor("ordenasiento"));
        cursoraux1->siguienteregistro ();
        i++;
    }// end while
    delete cursoraux1;
}// end inicializa

void adocumental::doubleclicked(int row, int, int, const QPoint &) {
    idadocumental = m_listado->text(row, COL_IDADOCUMENTAL);
    fprintf(stderr,"Archivo Documental: %s\n", idadocumental.ascii());
    if (modo ==0 ) { // Es el modo edicion
        QString archivo = m_listado->text(row, COL_ARCHIVOADOCUMENTAL);
        QString comando = "konqueror "+archivo+" &";
        system(comando.ascii());
    } else { // Es el modo consulta
        done(1);
    }// end if
}// end dobleclicked


void adocumental::newADocumental(QString archivo) {
        QString SQLQuery = "INSERT INTO adocumental (archivoadocumental) VALUES ('"+archivo+"')";
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
}// end newAdocumental

void adocumental::boton_newadocumental() {
    QString fn = QFileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER), tr("Todos (*.*)"), 0,tr("Agregar Documento"),tr("Elige el nombre de archivo"));
    if (!fn.isEmpty()) {
        newADocumental(fn);
    }// end if
    inicializa();
}// end boton_newadocumental

inline QString adocumental::getidadocumental() {
    return (idadocumental);
}// end getidadocumental


void adocumental::asociaasiento(QString idasiento) {
    fprintf(stderr,"AsociaAsiento: \n");
    fprintf(stderr,"idasiento:%s, idadocumental:%s\n", idasiento.ascii(), idadocumental.ascii());
    if ((idadocumental != "") && (idasiento != "")) {
        QString SQLQuery = "UPDATE adocumental SET idasiento= "+idasiento+" WHERE idadocumental="+idadocumental;
        fprintf(stderr,"%s\n", SQLQuery.ascii());
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    }// end if
    inicializa();
}// end adocumental


/******************************************************
 * Esta función coge el primer archivo documental no  *
 * asociado a ning asiento y lo muestra asignando a idasiento
 * su valor */
void adocumental::presentaprimervacio() {
    int i=0;
    while (m_listado->text(i,COL_IDASIENTO) != "" && i < m_listado->numRows())
        i++;
    if (i < m_listado->numRows())
        doubleclicked(i, 0, 0, QPoint::QPoint(0,0));
}// end presentarprimervacio

void adocumental::boton_desasociar() {
    idadocumental = m_listado->text(m_listado->currentRow(), COL_IDADOCUMENTAL);
    if (idadocumental != "") {
        QString SQLQuery = "UPDATE adocumental SET idasiento= NULL WHERE idadocumental="+idadocumental;
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    }// end if
    inicializa();
}// end botondesasociar

void adocumental::s_deleteADocumental() {
    idadocumental = m_listado->text(m_listado->currentRow(), COL_IDADOCUMENTAL);
    if (idadocumental != "") {
        QString SQLQuery = "DELETE FROM  adocumental WHERE idadocumental="+idadocumental;
        conexionbase->begin();
        conexionbase->ejecuta(SQLQuery);
        conexionbase->commit();
    }// end if
    inicializa();
}// end s_deleteADocumental


void adocumental::s_saveADocumental() {
    int row = m_listado->currentRow();
    idadocumental = m_listado->text(row, COL_IDADOCUMENTAL);
    if (idadocumental != "") {
        QString Query;
        Query  = "UPDATE adocumental SET ";
        Query += "descripcionadocumental='"+m_listado->text(row,COL_DESCRIPCIONADOCUMENTAL)+"'";
        Query += " WHERE idadocumental = "+idadocumental;
        conexionbase->begin();
        conexionbase->ejecuta(Query);
        conexionbase->commit();
    }// end if
    inicializa();
} // end s_saveADocumental

void adocumental::s_agregarDirectorio() {
    QString fn = QFileDialog::getExistingDirectory(
                     confpr->valor(CONF_DIR_USER),
                     this,
                     "get existing directory",
                     "Choose a directory",
                     TRUE );
    QDir d(fn);
    d.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
    d.setSorting( QDir::Size | QDir::Reversed );

    const QFileInfoList *list = d.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi;
    while ( (fi = it.current()) != 0 ) {
	QString fn = fi->filePath();
	newADocumental(fn);
        ++it;
    }// end while
    inicializa();
}// end s_agregarDirectorio
