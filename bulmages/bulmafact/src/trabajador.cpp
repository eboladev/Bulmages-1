//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: Tomeu Borr� Riera, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

/*
CREATE TABLE trabajador (
    idtrabajador serial PRIMARY KEY,
    nomtrabajador character varying NOT NULL,
    apellidostrabajador character varying,
    dirtrabajador character varying,
    nsstrabajador character varying,
    teltrabajador character varying,
    moviltrabajador character varying,
    emailtrabajador character varying,
    fototrabajador character varying,
    activotrabajador boolean DEFAULT TRUE NOT NULL
);
*/

#include "trabajador.h"
#include "company.h"

#include <Q3ListView>
#include <QLineEdit>
#include <QMessageBox>
#include <Q3FileDialog>
#include <QPixmap>
#include <QLabel>
#include <qdialog.h>

#define COL_IDTRABAJADOR   0
#define COL_NOMTRABAJADOR  1
#define COL_APELLIDOSTRABAJADOR 2
#define COL_DIRTRABAJADOR  4
#define COL_NSSTRABAJADOR 3
#define COL_TELTRABAJADOR 5
#define COL_MOVILTRABAJADOR 6
#define COL_EMAILTRABAJADOR 7
#define COL_FOTOTRABAJADOR 8
#define COL_ACTIVOTRABAJADOR 9

/** Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte */
Trabajador::Trabajador(company *emp,QWidget *parent, const char *name)
        : Trabajadorbase(parent, name) , dialogChanges(this) {
    companyact=emp;
    m_listTrabajador->addColumn("id",0);
    m_listTrabajador->addColumn("nombre",-1);
    m_listTrabajador->addColumn("dias",0);
    m_listTrabajador->addColumn("descuento",0);
    m_listTrabajador->addColumn("aux",0);
    m_listTrabajador->addColumn("aux",0);
    m_listTrabajador->addColumn("aux",0);
    m_listTrabajador->addColumn("aux",0);
    m_listTrabajador->addColumn("aux",0);
    m_listTrabajador->addColumn("aux",0);
    m_archivoimagen="";
    setModoEdicion();
    pintar();
}


/** Carga el query de la base de datos y carga el qlistview */
void Trabajador::pintar() {
    Q3ListViewItem * it;
    cursor2 *cursoraux1;
    m_listTrabajador->clear();
    cursoraux1 = companyact->cargacursor("SELECT * FROM trabajador ORDER BY apellidostrabajador");
    while (!cursoraux1->eof()) {
        it =new Q3ListViewItem(m_listTrabajador);
        it->setText(COL_IDTRABAJADOR, cursoraux1->valor("idtrabajador"));
        it->setText(COL_NOMTRABAJADOR, cursoraux1->valor("nomtrabajador"));
        it->setText(COL_APELLIDOSTRABAJADOR, cursoraux1->valor("apellidostrabajador"));
        it->setText(COL_DIRTRABAJADOR, cursoraux1->valor("dirtrabajador"));
        it->setText(COL_TELTRABAJADOR, cursoraux1->valor("teltrabajador"));
        it->setText(COL_MOVILTRABAJADOR, cursoraux1->valor("moviltrabajador"));
        it->setText(COL_EMAILTRABAJADOR, cursoraux1->valor("emailtrabajador"));
        it->setText(COL_FOTOTRABAJADOR, cursoraux1->valor("fototrabajador"));
        it->setText(COL_ACTIVOTRABAJADOR, cursoraux1->valor("activotrabajador"));
        it->setText(COL_NSSTRABAJADOR, cursoraux1->valor("nsstrabajador"));
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}// end pintar


Trabajador::~Trabajador() {}

#include <QCheckBox>
void Trabajador::s_lista(Q3ListViewItem *it) {
    if (it != NULL) {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        m_nomtrabajador->setText(it->text(COL_NOMTRABAJADOR));
        m_apellidostrabajador->setText(it->text(COL_APELLIDOSTRABAJADOR));
        m_nsstrabajador->setText(it->text(COL_NSSTRABAJADOR));
        m_idtrabajador= it->text(COL_IDTRABAJADOR);
        m_dirtrabajador->setText(it->text(COL_DIRTRABAJADOR));
        m_teltrabajador->setText(it->text(COL_TELTRABAJADOR));
        m_moviltrabajador->setText(it->text(COL_MOVILTRABAJADOR));
        m_emailtrabajador->setText(it->text(COL_EMAILTRABAJADOR));
        if (it->text(COL_ACTIVOTRABAJADOR) == "t") {
            m_activotrabajador->setChecked(TRUE);
        } else {
            m_activotrabajador->setChecked(FALSE);
        }// end if
        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
        m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_PERSONAL)+m_idtrabajador+".jpg"));
    }// end if
}// end s_lista



bool Trabajador::close(bool p) {
    fprintf (stderr,"close()\n");
    /// Si se ha modificado el contenido advertimos y guardamos.
    fprintf(stderr,"dialogo cierra\n");
    return QDialog::close(p);
}


void Trabajador::s_saveTrabajador() {
    QString m_textactivotrabajador = "FALSE";
    if (m_activotrabajador->isChecked())
        m_textactivotrabajador = "TRUE";

    QString query = "UPDATE trabajador SET ";
    query += "  nomtrabajador='"+companyact->sanearCadena(m_nomtrabajador->text())+"'";
    query += ", apellidostrabajador= '"+companyact->sanearCadena(m_apellidostrabajador->text())+"'";
    query += ", nsstrabajador = '"+companyact->sanearCadena(m_nsstrabajador->text())+"'";
    query += ", dirtrabajador = '"+companyact->sanearCadena(m_dirtrabajador->text())+"'";
    query += ", teltrabajador = '"+companyact->sanearCadena(m_teltrabajador->text())+"'";
    query += ", moviltrabajador = '"+companyact->sanearCadena(m_moviltrabajador->text())+"'";
    query += ", emailtrabajador = '"+companyact->sanearCadena(m_emailtrabajador->text())+"'";
    query += ", activotrabajador = "+companyact->sanearCadena(m_textactivotrabajador);
    query += " WHERE idtrabajador="+companyact->sanearCadena(m_idtrabajador);

    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    }// end if
    Q3ListViewItem *it =  m_listTrabajador->findItem(m_idtrabajador, COL_IDTRABAJADOR);
    it->setText(COL_IDTRABAJADOR, m_idtrabajador);
    it->setText(COL_NOMTRABAJADOR, m_nomtrabajador->text());
    it->setText(COL_APELLIDOSTRABAJADOR, m_apellidostrabajador->text());
    it->setText(COL_NSSTRABAJADOR, m_nsstrabajador->text());
    if (m_archivoimagen != "") {
        QString cadena = "cp "+m_archivoimagen+" "+confpr->valor(CONF_DIR_IMG_PERSONAL)+m_idtrabajador+".jpg";
        fprintf(stderr,"%s\n",cadena.ascii());
        system(cadena.ascii());
    }// end if
}// end if


bool Trabajador::trataModificado() {
    fprintf(stderr,"TrataModificado\n");
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Trabajador",
                                   "Desea guardar los cambios.",
                                   tr("Si"), tr("No"),0,0,1) == 0)
            s_saveTrabajador();
        return (TRUE);
    }// end if
    return(FALSE);
}// end trataModificado


/** SLOT que responde a la pulsaci� del bot� de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void Trabajador::s_newTrabajador() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString query = "INSERT INTO trabajador (nomtrabajador, apellidostrabajador, nsstrabajador) VALUES ('NUEVO TRABAJADOR','NUEVO TRABAJADOR','000000000000')";
    companyact->begin();
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    }// end if
    cursor2 *cur = companyact->cargacursor("SELECT max(idtrabajador) AS idtrabajador FROM trabajador");
    companyact->commit();
    m_idtrabajador = cur->valor("idtrabajador");
    delete cur;
    pintar();
}// end s_newTipoIVA


/** SLOT que responde a la pulsaci� del bot� de borrar la familia que se est�editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void Trabajador::s_deleteTrabajador() {
    trataModificado();
    companyact->begin();
    QString query = "DELETE FROM trabajador WHERE idtrabajador="+m_idtrabajador;
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    }// end if
    companyact->commit();
    pintar();
}// end s_saveTipoIVA


void Trabajador::s_cambiarImagen() {
    m_archivoimagen = Q3FileDialog::getOpenFileName(
                          "",
                          "Images (*.jpg)",
                          this,
                          "open file dialog",
                          "Choose a file" );
    fprintf(stderr," Archivo Seleccionado: %s\n",m_archivoimagen.ascii());
    m_imagen->setPixmap(QPixmap(m_archivoimagen));
}// end s_cambiarimagen

