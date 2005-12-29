//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tiposarticuloview.h"
#include "company.h"
#include <q3listview.h>
#include <qmap.h>
#include <qlineedit.h>
#include <q3textedit.h>
#include <qmessagebox.h>

#define COL_IDTIPOARTICULO 0
#define COL_CODTIPOARTICULO 1
#define COL_DESCTIPOARTICULO  2


tiposarticuloview::tiposarticuloview(company *comp, QWidget *parent, const char *name) : tiposarticulodlg(parent, name), dialogChanges(this) {
    companyact = comp;
    m_listTipos->addColumn("ID",-1);
    m_listTipos->addColumn("CODIGO",-1);
    m_listTipos->addColumn("DESCRIPCION",-1);
    m_idtipo="";
    setModoEdicion();
    pintar();
}// end familiasview


tiposarticuloview::~tiposarticuloview() {}// end ~familiasview


void tiposarticuloview::pintar() {
    Q3ListViewItem * it;
    QMap <int, Q3ListViewItem*> Lista1;
    int idtipo=0;
    cursor2 *cursoraux1;
    m_listTipos->clear();
    cursoraux1 = companyact->cargacursor("SELECT * FROM tipo_articulo ORDER BY codtipo_articulo");
    while (!cursoraux1->eof()) {
        idtipo = cursoraux1->valor("idtipo_articulo").toInt();
        it =new Q3ListViewItem(m_listTipos);
        Lista1[idtipo]=it;
        it->setText(COL_IDTIPOARTICULO, cursoraux1->valor("idtipo_articulo"));
        it->setText(COL_CODTIPOARTICULO,cursoraux1->valor("codtipo_articulo"));
        it->setText(COL_DESCTIPOARTICULO, cursoraux1->valor("desctipo_articulo"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    m_idtipo="";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}// end pintar


QString tiposarticuloview::codtipo_articulo() {
    Q3ListViewItem *it = m_listTipos->currentItem();
    return it->text(COL_CODTIPOARTICULO);
};

QString tiposarticuloview::idtipo_articulo() {
    Q3ListViewItem *it = m_listTipos->currentItem();
    return it->text(COL_IDTIPOARTICULO);
};

QString tiposarticuloview::desctipo_articulo() {
    Q3ListViewItem *it = m_listTipos->currentItem();
    return it->text(COL_DESCTIPOARTICULO);
};


/**
  * Se ha seleccionado un item en la lista
  * Lo que hacemos es mostar el elemento
  * Si el anterior ha sido modificado pedimos para actuar en consecuencia.
  */
void tiposarticuloview::s_doubleClicked(Q3ListViewItem *it) {
    if (m_modoConsulta) {
        m_idtipo = it->text(COL_IDTIPOARTICULO);
        done(1);
    }// end if
}// end seleccionado


/**
  * Se ha seleccionado un item en la lista
  * Lo que hacemos es mostar el elemento
  * Si el anterior ha sido modificado pedimos para actuar en consecuencia.
  */
void tiposarticuloview::s_changedTipo() {
    QString idtipoold = m_listTipos->currentItem()->text(COL_IDTIPOARTICULO);
    if (idtipoold != "") {
        // Si usamos el trataModificado peta porque si se guarda se sobreescribe el puntero it.
        trataModificado();
        m_idtipo = idtipoold;
        m_listTipos->findItem(m_idtipo, COL_IDTIPOARTICULO);
        mostrarplantilla();
    }// end if
}// end seleccionado


void tiposarticuloview::mostrarplantilla() {
    fprintf(stderr,"mostramos la plantilla\n");
    QString query;
    query= "SELECT * from tipo_articulo WHERE idtipo_articulo="+m_idtipo;
    cursor2 *cursortipo = companyact->cargacursor(query);
    if (!cursortipo->eof()) {
        m_codTipo->setText(cursortipo->valor("codtipo_articulo"));
        m_descTipo->setText(cursortipo->valor("desctipo_articulo"));
    }// end if
    delete cursortipo;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    fprintf(stderr,"Terminamos la ejecución de tiposarticuloview::mostrarplantilla\n");
}// end mostrarplantilla


/** Antes de salir de la ventana debemos hacer la comprobaci� de si se ha modificado algo
  * Esta funci� est�dedicada a Francina, Bienvenida al mundo
  */
void tiposarticuloview::close() {
    trataModificado();
    done(0);
}// end close


bool tiposarticuloview::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Familia",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            s_saveTipo();
        return (TRUE);
    }// end if
    return(FALSE);
}// end trataModificado


/** SLOT que responde a la pulsaci� del bot� de guardar el tipo de iva que se est�editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void tiposarticuloview::s_saveTipo() {
    QString query = "UPDATE tipo_articulo SET codtipo_articulo='"+
                    companyact->sanearCadena(m_codTipo->text())+"', desctipo_articulo= '"+
                    companyact->sanearCadena(m_descTipo->text())+"' WHERE idtipo_articulo="+m_idtipo;
    companyact->begin();
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    }// end if
    companyact->commit();
    //pintar();
    // Vamos a hacer algo no reentrante.
    Q3ListViewItem *it =  m_listTipos->findItem(m_idtipo, COL_IDTIPOARTICULO);
    cursor2 *cursoraux1 = companyact->cargacursor("SELECT * FROM tipo_articulo WHERE idtipo_articulo="+m_idtipo);
    if (!cursoraux1->eof()) {
        it->setText(COL_IDTIPOARTICULO, cursoraux1->valor("idtipo_articulo"));
        it->setText(COL_CODTIPOARTICULO,cursoraux1->valor("codtipo_articulo"));
        it->setText(COL_DESCTIPOARTICULO, cursoraux1->valor("desctipo_articulo"));
    }// end if
    delete cursoraux1;
}// end s_saveTipoIVA


/** SLOT que responde a la pulsaci� del bot� de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void tiposarticuloview::s_newTipo() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();

    QString query = "INSERT INTO tipo_articulo (codtipo_articulo, desctipo_articulo) VALUES ('XXXXXX','Descripcion del tipo')";
    companyact->begin();
    int error = companyact->ejecuta(query);
	if (error) {
		companyact->rollback();
		return;
	}// end if
    cursor2 *cur = companyact->cargacursor("SELECT max(idtipo_articulo) AS idtipo FROM tipo_articulo");
    companyact->commit();
    m_idtipo = cur->valor("idtipo");
    delete cur;
    pintar();
}// end s_newTipoIVA

/** SLOT que responde a la pulsaci� del bot� de borrar la familia que se est�editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void tiposarticuloview::s_deleteTipo() {
    trataModificado();
    QString query = "DELETE FROM tipo_articulo WHERE idtipo_articulo="+m_idtipo;
    int error = companyact->ejecuta(query);
	if (error) return;
    pintar();
}// end s_saveTipoIVA


