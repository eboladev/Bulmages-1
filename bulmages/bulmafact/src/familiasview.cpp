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
#include <Q3ListView>
#include <QMap>
#include <QLineEdit>
#include <Q3TextEdit>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>


#include "familiasview.h"
#include "company.h"
#include "funcaux.h"


#define COL_NOMFAMILIA 0
#define COL_CODCOMPLETOFAMILIA 1
#define COL_DESCFAMILIA 2
#define COL_IDFAMILIA  3
#define COL_CODFAMILIA 4


familiasview::familiasview(company *comp, QWidget *parent, const char *name) : QDialog(parent, name), dialogChanges(this) {
    setupUi(this);
    companyact = comp;

    m_listFamilias->setColumnCount(3);
    QStringList headers;
    headers << "NOMBRE" << "CODIGO" << "DESCRIPCION" << "IDFAMILIA" << "CODIGOCOMPLETO";
    m_listFamilias->setHeaderLabels(headers);

    m_listFamilias->setColumnHidden(COL_IDFAMILIA, TRUE);
    m_listFamilias->setColumnHidden(COL_CODFAMILIA, TRUE);

    m_idfamilia="";
    setModoEdicion();
    pintar();
}// end familiasview


familiasview::~familiasview() {}


void familiasview::pintar() {
    _depura("familiasview::pintar",0);
    QTreeWidgetItem * it;
    QMap <int, QTreeWidgetItem*> Lista1;
    int padre;
    int idfamilia=0;
    cursor2 *cursoraux1, *cursoraux2;

    /// vaciamos el arbol
    while (m_listFamilias->topLevelItemCount () > 0) {
        it = m_listFamilias->takeTopLevelItem(0);
        delete it;
    }// end while

    cursoraux1 = companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia ISNULL ORDER BY idfamilia");
    while (!cursoraux1->eof()) {
        padre = cursoraux1->valor("padrefamilia").toInt();
        idfamilia = cursoraux1->valor("idfamilia").toInt();
        it =new QTreeWidgetItem(m_listFamilias);
        Lista1[idfamilia]=it;
        it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA,cursoraux1->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        m_listFamilias->expandItem(it);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;
    cursoraux2= companyact->cargacursor("SELECT * FROM familia WHERE padrefamilia IS NOT NULL ORDER BY idfamilia");
    while (!cursoraux2->eof()) {
        padre = cursoraux2->valor("padrefamilia").toInt();
        idfamilia = cursoraux2->valor("idfamilia").toInt();
        it = new QTreeWidgetItem(Lista1[padre]);
        Lista1[idfamilia]=it;
        it->setText(COL_NOMFAMILIA, cursoraux2->valor("nombrefamilia"));
        it->setText(COL_CODFAMILIA,cursoraux2->valor("codigofamilia"));
        it->setText(COL_DESCFAMILIA, cursoraux2->valor("descfamilia"));
        it->setText(COL_IDFAMILIA, cursoraux2->valor("idfamilia"));
        it->setText(COL_CODCOMPLETOFAMILIA, cursoraux2->valor("codigocompletofamilia"));
        m_listFamilias->expandItem(it);
        cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;
    m_idfamilia="";
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
}// end pintar


QString familiasview::codigoCompletoFamilia() {
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    return it->text(COL_CODCOMPLETOFAMILIA);
};

QString familiasview::idFamilia() {
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    return it->text(COL_IDFAMILIA);
};


QString familiasview::nombreFamilia() {
    QTreeWidgetItem *it = m_listFamilias->currentItem();
    return it->text(COL_NOMFAMILIA);
};

/**
  * Se ha seleccionado un item en la lista
  * Lo que hacemos es mostar el elemento
  * Si el anterior ha sido modificado pedimos para actuar en consecuencia.
  */
void familiasview::on_m_listFamilias_itemDoubleClicked(QTreeWidgetItem *it) {
    if (m_modoConsulta) {
        m_idfamilia = it->text(COL_IDFAMILIA);
        done(1);
    }// end if
}// end seleccionado


/**
  * Se ha seleccionado un item en la lista
  * Lo que hacemos es mostar el elemento
  * Si el anterior ha sido modificado pedimos para actuar en consecuencia.
  */
void familiasview::on_m_listFamilias_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * ) {
    QString idfamiliaold = current->text(COL_IDFAMILIA);
    if (idfamiliaold != "") {
        // Si usamos el trataModificado peta porque si se guarda se sobreescribe el puntero it.
        trataModificado();
        m_idfamilia = idfamiliaold;
        /*
            QList<QTreeWidgetItem *> listit =  m_listFamilias->findItems(m_idfamilia,Qt::MatchExactly, COL_IDFAMILIA);
            QTreeWidgetItem *it = listit.first();
        */
        mostrarplantilla();
    }// end if
}// end seleccionado


void familiasview::mostrarplantilla() {
    fprintf(stderr,"mostramos la plantilla\n");
    QString query;
    query= "SELECT * from familia WHERE idfamilia="+m_idfamilia;
    cursor2 *cursorfamilia = companyact->cargacursor(query);
    if (!cursorfamilia->eof()) {
        m_nomFamilia->setText(cursorfamilia->valor("nombrefamilia"));
        m_descFamilia->setText(cursorfamilia->valor("descfamilia"));
        m_codCompletoFamilia->setText(cursorfamilia->valor("codigocompletofamilia"));
        m_codFamilia->setText(cursorfamilia->valor("codigofamilia"));
    }// end if
    delete cursorfamilia;
    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    fprintf(stderr,"Terminamos la ejecución de familiasview::mostrarplantilla\n");
}// end mostrarplantilla


/** Antes de salir de la ventana debemos hacer la comprobación de si se ha modificado algo
  * Esta función está dedicada a Francina, Bienvenida al mundo :))
  */
void familiasview::close() {
    trataModificado();
    done(0);
}// end close


bool familiasview::trataModificado() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if ( QMessageBox::warning( this, "Guardar Familia",
                                   "Desea guardar los cambios.",
                                   QMessageBox::Ok ,
                                   QMessageBox::Cancel ) == QMessageBox::Ok)
            on_mui_guardar_clicked();
        return (TRUE);
    }// end if
    return(FALSE);
}// end trataModificado


/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se est�editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void familiasview::on_mui_guardar_clicked() {
    _depura("familiasview::on_mui_guardar_clicked",0);
    QString query = "UPDATE familia SET nombrefamilia='"+
                    companyact->sanearCadena(m_nomFamilia->text())+"', descfamilia= '"+
                    companyact->sanearCadena(m_descFamilia->text())+"' , codigofamilia = '"+
                    companyact->sanearCadena(m_codFamilia->text())+"' WHERE idfamilia="+m_idfamilia;
    int error = companyact->ejecuta(query);
    if (error)
        return;
    QTreeWidgetItem *it = m_listFamilias->currentItem();

    if (it) {
        cursor2 *cursoraux1 = companyact->cargacursor("SELECT * FROM familia WHERE idfamilia="+m_idfamilia);
        if (!cursoraux1->eof()) {
            it->setText(COL_NOMFAMILIA, cursoraux1->valor("nombrefamilia"));
            it->setText(COL_CODFAMILIA,cursoraux1->valor("codigofamilia"));
            it->setText(COL_DESCFAMILIA, cursoraux1->valor("descfamilia"));
            it->setText(COL_IDFAMILIA, cursoraux1->valor("idfamilia"));
            it->setText(COL_CODCOMPLETOFAMILIA, cursoraux1->valor("codigocompletofamilia"));
        }// end if
        delete cursoraux1;
    }// end if
    dialogChanges_cargaInicial();
    _depura("END familiasview::on_mui_guardar_clicked",0);
}


/** SLOT que responde a la pulsación del botón de nuevo tipo de iva
  * Inserta en la tabla de ivas
  */
void familiasview::on_mui_crear_clicked() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    trataModificado();
    QString padrefamilia;
    if (m_idfamilia != "")
        padrefamilia = m_idfamilia;
    else
        padrefamilia = "NULL";

    QString query = "INSERT INTO familia (nombrefamilia, descfamilia, padrefamilia, codigofamilia) VALUES ('NUEVA FAMILIA','Descripcion de la familia',"+padrefamilia+",'XXX')";
    companyact->begin();
    int error = companyact->ejecuta(query);
    if (error) {
        companyact->rollback();
        return;
    }// end if
    cursor2 *cur = companyact->cargacursor("SELECT max(idfamilia) AS idfamilia FROM familia");
    companyact->commit();
    m_idfamilia = cur->valor("idfamilia");
    delete cur;
    pintar();
}// end s_newTipoIVA

/** SLOT que responde a la pulsación del botón de borrar la familia que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void familiasview::on_mui_borrar_clicked() {
    trataModificado();
    QString query = "DELETE FROM FAMILIA WHERE idfamilia="+m_idfamilia;
    int error = companyact->ejecuta(query);
    if (error)
        return;
    pintar();
}// end s_saveTipoIVA


void familiasview::on_mui_imprimir_clicked() {
    /// Copiamos el archivo
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"familias.rml";
    archivo = "cp "+archivo+" /tmp/familias.rml";
    system (archivo.ascii());
    /// Copiamos el logo
    archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";
    archivo = "cp "+archivo+" /tmp/logo.jpg";
    system (archivo.ascii());
    QFile file;
    file.setName( "/tmp/familias.rml" );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    // Línea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" colWidths=\"3cm, 15cm\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";
    fitxersortidatxt += "	<td>Código</td>";
    fitxersortidatxt += "	<td>Nombre</td>";
    fitxersortidatxt += "</tr>";

    cursor2 *cur=companyact->cargacursor("SELECT * FROM familia ORDER BY codigocompletofamilia");
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";
        fitxersortidatxt += "<td>"+cur->valor("codigocompletofamilia")+"</td>";
        fitxersortidatxt += "<td>"+cur->valor("nombrefamilia")+"</td>";
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    }// end if
    delete cur;
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }
    system("trml2pdf.py /tmp/familias.rml > /tmp/familias.pdf");
    system("kpdf /tmp/familias.pdf &");

}// end imprimir


void familiasview::on_mui_aceptar_clicked() {
    trataModificado();
    m_idfamilia = m_listFamilias->currentItem()->text(COL_IDFAMILIA);
    done(1);
}

