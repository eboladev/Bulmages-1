/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <QTextStream>

#include "facturapview.h"
#include "company.h"
#include "listlinfacturapview.h"
#include "providerslist.h"
#include "pagoview.h"
#include "funcaux.h"
#include "albaranproveedorview.h"


/** Inicializa todos los componentes de la clase.
    mete la ventana en el workSpace. */
/**
\param comp
\param parent
**/
FacturaProveedorView::FacturaProveedorView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("FacturaProveedorView::FacturaProveedorView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi(this);

        setTitleName(tr("Factura Proveedor"));
        setDBTableName("facturap");
        setDBCampoId("idfacturap");
        addDBCampo("idfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("FacturaProveedor", "Id facturap"));
        addDBCampo("idproveedor", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("FacturaProveedor", "Id proveedor"));
        addDBCampo("numfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Numero"));
        addDBCampo("ffacturap", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Fecha"));
        addDBCampo("procesadafacturap", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Procesada facturap"));
        addDBCampo("comentfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Comentario facturap"));
        addDBCampo("reffacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Referencia facturap"));
        addDBCampo("descfacturap", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Descripcion facturap"));
        addDBCampo("idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Id trabajador"));
        addDBCampo("idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("FacturaProveedor", "Id forma de pago"));


        subform2->setEmpresaBase(comp);
        m_descuentos->setEmpresaBase(comp);
        mui_idforma_pago->setEmpresaBase(comp);
        mui_idforma_pago->setidforma_pago("");
        mui_idproveedor->setEmpresaBase(comp);
        mui_idproveedor->setidproveedor("");
        mui_reffacturap->setEmpresaBase(comp);

        setListaLineas(subform2);
        setListaDescuentos(m_descuentos);

        m_totalBases->setReadOnly(TRUE);
        m_totalBases->setAlignment(Qt::AlignRight);
        m_totalTaxes->setReadOnly(TRUE);
        m_totalTaxes->setAlignment(Qt::AlignRight);
        m_totalDiscounts->setReadOnly(TRUE);
        m_totalDiscounts->setAlignment(Qt::AlignRight);
        m_totalfacturap->setReadOnly(TRUE);
        m_totalfacturap->setAlignment(Qt::AlignRight);
        meteWindow(windowTitle(), this, FALSE);
    } catch (...) {
        mensajeInfo(tr("Error al crear la factura proveedor"));
    } // end try
    _depura("END FacturaProveedorView::FacturaProveedorView", 0);
}


/** Al destruir una factura de proveedor se hace un refresco del listado
    de facturas de proveedor. */
/**
**/
FacturaProveedorView::~FacturaProveedorView() {
    _depura("FacturaProveedorView::~FacturaProveedorView", 0);
    empresaBase()->refreshFacturasProveedor();
    _depura("END FacturaProveedorView::~FacturaProveedorView", 0);

}


///
/**
**/
void FacturaProveedorView::on_mui_pagar_clicked() {
    _depura("FacturaProveedorView::on_mui_pagar_clicked", 0);
    s_nuevoPago();
    _depura("END FacturaProveedorView::on_mui_pagar_clicked", 0);
}


///
/**
**/
void FacturaProveedorView::on_m_descuentos_editFinish(int, int) {
    _depura("FacturaProveedorView::on_m_descuentos_editFinish", 0);
    calculaypintatotales();
    _depura("END FacturaProveedorView::on_m_descuentos_editFinish", 0);
}


///
/**
**/
void FacturaProveedorView::on_subform2_editFinish(int, int) {
    _depura("FacturaProveedorView::on_subform2_editFinish", 0);
    calculaypintatotales();
    _depura("END FacturaProveedorView::on_subform2_editFinish", 0);
}


/** inicializar debe ser invocado cuando se crea una nueva ficha sin cargar ningun
    date de la base de datos (por ejemplo una nueva ficha).
    Sirve para inicializar los componenetes sin necesidad de query alguno */
/**
**/
void FacturaProveedorView::inicializar() {
    _depura("FacturaProveedorView::inicializar", 0);
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_cargaInicial();
    _depura("END FacturaProveedorView::inicializar", 0);
}


/** Pinta los totales de la Factura de proveedor.
    Este metodo se llama desde FacturaProveedor. */
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void FacturaProveedorView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq) {
    _depura("PresupuestoView::pintatotales", 0);
    m_totalBases->setText(QString(base.toQString()));
    m_totalTaxes->setText(QString(iva.toQString()));
    m_totalfacturap->setText(QString(total.toQString()));
    m_totalDiscounts->setText(QString(desc.toQString()));
    m_totalIRPF->setText(QString(irpf.toQString()));
    m_totalReqEq->setText(QString(reqeq.toQString()));
    _depura("END PresupuestoView::pintatotales", 0);
}


/** Metodo que responde al hacer un nuevo pago desde la factura.
    Crea una instancia de \ref PagoView y la lanza poniendole los parametros
    del pago iguales a los de la factura de Proveedor.
*/
/// \TODO: Deberia ser directamente on_mui_pagar_clicked()
/**
**/
void FacturaProveedorView::s_nuevoPago() {
    _depura("FacturaProveedorView::s_nuevoPago", 0);
    PagoView *bud = empresaBase()->newPagoView();
    empresaBase()->m_pWorkspace->addWindow(bud);
    bud->setDBvalue("idproveedor", DBvalue("idproveedor"));
    bud->setDBvalue("cantpago", m_totalfacturap->text());
    bud->setDBvalue("refpago", DBvalue("reffacturap"));
    bud->setDBvalue("comentpago", DBvalue("descfacturap"));
    bud->pintar();
    bud->show();
    _depura("END FacturaProveedorView::s_nuevoPago", 0);
}


/** SLOT que responde  a la pulsacion del boton ver albaranes.
    Consulta todos los albaranes con la misma referencia.
    Los instancia y los carga. */
/**
**/
void FacturaProveedorView::on_mui_veralbaranes_clicked() {
    _depura("FacturaProveedorView::on_mui_veralbaranes_clicked", 0);
    QString query = "SELECT * FROM albaranp WHERE refalbaranp='"+DBvalue("reffacturap")+"'";
    cursor2 *cur = empresaBase()->cargacursor(query);
    while (!cur->eof()) {
        AlbaranProveedorView *albpro = empresaBase()->newAlbaranProveedorView();
        albpro->cargar(cur->valor("idalbaranp"));
        empresaBase()->m_pWorkspace->addWindow( albpro);
        albpro->show();
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END FacturaProveedorView::on_mui_veralbaranes_clicked", 0);
}


///
/**
\param id
**/
void FacturaProveedorView::on_mui_idproveedor_valueChanged(QString id) {
    _depura("FacturaProveedorView::on_m_proveedor_valueChanged", 0);
    subform2->setIdProveedor(id);
    mui_idforma_pago->setIdProveedor(id);
    _depura("END FacturaProveedorView::on_m_proveedor_valueChanged", 0);
}


/** Hace el borrado de la factura.
    Primero borra las lineas de factura.
    Luego borra los descuentos de factura.
    Y por ultimo delega en DBRecord el borrado del registro.
    Si algo falla devuelve -1.
    Si todo va bien devuelve 0.
*/
/**
\return
**/
int FacturaProveedorView::borrarPre() {
    _depura("FacturaProveedorView::borrar", 0);
    m_listalineas->borrar();
    m_listadescuentos->borrar();
    _depura("END FacturaProveedorView::borrar", 0);
    return 0;
}



/** Carga un FacturaProveedor.
	Carga el DBRecord con DBload y luego llama a la carga de las lineas y los descuentos.
	Al finalizar hace un pintado para que la informacion se vea actualizada.
	Si algo falla genera una excepcion -1.
	Si todo va bien devuelve 0.
*/
/**
\param idfacturap
\return
**/
int FacturaProveedorView::cargarPost(QString idfacturap) {
    _depura("FacturaProveedorView::cargar", 0);
    m_listalineas->cargar(idfacturap);
    m_listadescuentos->cargar(idfacturap);
    _depura("END FacturaProveedorView::cargar", 0);
    return 0;
}

/** Hace el guardado de una Factura de Proveedor.
	guarda el registro a traves de DBRecord y luego actualiza el identificador de facturap en
	las lineas y en los descuentos para guardarlos.
	Si algo falla genera una excepcion -1.
	Si todo va bien devuelve 0.
*/
/**
\return
**/
int FacturaProveedorView::guardarPost() {
    _depura("FacturaProveedorView::guardar()", 0);
    m_listalineas->setColumnValue("idfacturap", DBvalue("idfacturap"));
    m_listadescuentos->setColumnValue("idfacturap", DBvalue("idfacturap"));
    m_listalineas->guardar();
    m_listadescuentos->guardar();
    _depura("END FacturaProveedorView::guardar()", 0);
    return 0;
}


/** Imprime una factura de proveedor
*/
/// \DEPRECATED: No se prevee que se tengan que imprimir facturas de proveedor.
/**
\return
**/
void FacturaProveedorView::imprimirFacturaProveedor() {
    /// Hacemos el lanzamiento de plugins para este caso.
    int res = g_plugins->lanza("imprimirFacturaProveedor", this);
    if (res)
        return;

    base basesimp;

    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "facturap.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "facturap.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.toAscii().constData());

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());

    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt="";

    /// Linea de totales del presupuesto
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + DBvalue("idproveedor");
    cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
    if (!cur->eof()) {
        buff.replace("[dirproveedor]", cur->valor("dirproveedor"));
        buff.replace("[poblproveedor]", cur->valor("poblproveedor"));
        buff.replace("[telproveedor]", cur->valor("telproveedor"));
        buff.replace("[nomproveedor]", cur->valor("nomproveedor"));
        buff.replace("[cifproveedor]", cur->valor("cifproveedor"));
    } // end if
    delete cur;

    buff.replace("[numfacturap]", DBvalue("numfacturap"));
    buff.replace("[ffacturap]", DBvalue("ffacturap"));
    buff.replace("[comentfacturap]", DBvalue("comentfacturap"));
    buff.replace("[descfacturap]", DBvalue("descfacturap"));
    buff.replace("[reffacturap]", DBvalue("reffacturap"));

    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.5cm, 1.5cm, 1.5cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Codigo") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Concepto") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Cant.") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Precio") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Desc.") + "</td>\n";
    fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Total") + "</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    int i = 0;

    SDBRecord *linea;
    for (int i = 0; i < m_listalineas->rowCount(); ++i) {
        linea = m_listalineas->lineaat(i);
        Fixed base = Fixed(linea->DBvalue("cantlfacturap").toAscii().constData()) * Fixed(linea->DBvalue("pvplfacturap").toAscii().constData());
        basesimp[linea->DBvalue("ivalfacturap")] = basesimp[linea->DBvalue("ivalfacturap")] + base - base * Fixed(linea->DBvalue("descuentolfacturap").toAscii().constData()) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("codigocompletoarticulo") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("desclfacturap") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("cantlfacturap") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("pvplfacturap") + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->DBvalue("descuentolfacturap") + " %</td>\n";
        fitxersortidatxt += "        <td>" + (base - base * Fixed (linea->DBvalue("descuentolfacturap")) / 100).toQString() + "</td>\n";
        fitxersortidatxt += "</tr>";
        i++;
    } // end for

    while (i++ < 15)
        fitxersortidatxt += "<tr></tr>";

    fitxersortidatxt += "</blockTable>\n";
    buff.replace("[story]", fitxersortidatxt);

    Fixed basei("0.00");
    base::Iterator it;
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    Fixed porcentt("0.00");
    SDBRecord *linea1;
    if (m_listadescuentos->rowCount()) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Descuento") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Porcentaje") + "</td>\n";
        fitxersortidatxt += "        <td>" + QApplication::translate("FacturaProveedor", "Total") + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
            linea1 = m_listadescuentos->lineaat(i);
            porcentt = porcentt + Fixed(linea1->DBvalue("proporciondfacturap").toAscii().constData());
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("conceptdfacturap") + "</td>\n";
            fitxersortidatxt += "        <td>" + linea1->DBvalue("proporciondfacturap") + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf("-%s", ( Fixed(linea1->DBvalue("proporciondfacturap")) * basei / 100).toQString().toAscii().constData()) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace("[descuentos]", fitxersortidatxt);

    /// Impresion de los totales.
    fitxersortidatxt= "";
    QString tr1 = "";	/// Rellena el primer tr de titulares.
    QString tr2 = "";	/// Rellena el segundo tr de cantidades.
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";

    Fixed totbaseimp("0.00");
    Fixed parbaseimp("0.00");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "        <td>" + QApplication::translate("FacturaProveedor", "Base ") + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s",parbaseimp.toQString().toAscii().constData()) + "</td>\n";
    } // end for

    Fixed totiva("0.0");
    Fixed pariva("0.0");
    for (it = basesimp.begin(); it != basesimp.end(); ++it) {
        if (porcentt > 0) {
            pariva = (it.value() - it.value() * porcentt / 100) * Fixed(it.key()) / 100;
        } else {
            pariva = it.value() * Fixed(it.key()) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "        <td>" + QApplication::translate("FacturaProveedor", "IVA ") + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf("%s", pariva.toQString().toAscii().constData()) + "</td>\n";
    } // end for
    tr1 += "        <td>" + QApplication::translate("FacturaProveedor", "Total ") + "</td>\n";
    tr2 += "        <td>" + l.sprintf("%s", (totiva + totbaseimp).toQString().toAscii().constData()) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace("[totales]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file) ;
        stream << buff;
        file.close();
    } // end if
    invocaPDF("facturap");
}



