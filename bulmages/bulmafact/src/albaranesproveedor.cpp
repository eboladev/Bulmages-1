/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <Q3PopupMenu>
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>

#include "albaranesproveedor.h"
#include "albaranproveedorview.h"
#include "company.h"
#include "presupuestoview.h"
#include "qtable1.h"
#include "configuracion.h"
#include "funcaux.h"

#define COL_IDALBARANP 0
#define COL_NUMALBARANP 1
#define COL_DESCALBARANP 2
#define COL_REFALBARANP 3
#define COL_FECHAALBARANP 4
#define COL_COMENTALBARANP 5
#define COL_PROCESADOALBARANP 6
#define COL_IDPROVEEDOR 7
#define COL_IDFORMA_PAGO 8
#define COL_IDALMACEN 9
#define COL_NOMPROVEEDOR 10
#define COL_NOMALMACEN 11
#define COL_DESCFORMA_PAGO 12
#define COL_TOTALALBARANPROVEEDOR 13
#define COL_TOTALBASEIMP 14
#define COL_TOTALIMPUESTOS 15


void AlbaranesProveedor::guardaconfig() {
    QString aux = "";
    mver_idalbaranp->isChecked() ? aux += "1," : aux += "0,";
    mver_numalbaranp->isChecked() ? aux += "1," : aux += "0,";
    mver_descalbaranp->isChecked() ? aux += "1," : aux += "0,";
    mver_refalbaranp->isChecked() ? aux += "1," : aux += "0,";
    mver_fechaalbaranp->isChecked() ? aux += "1," : aux += "0,";
    mver_loginusuario->isChecked() ? aux += "1," : aux += "0,";
    mver_comentalbaranp->isChecked() ? aux += "1," : aux += "0,";
    mver_procesadoalbaranp->isChecked() ? aux += "1," :aux += "0,";
    mver_idproveedor->isChecked() ? aux += "1," : aux += "0,";
    mver_idforma_pago->isChecked() ? aux += "1," : aux += "0,";
    mver_idalmacen->isChecked() ? aux += "1," : aux += "0,";
    mver_nomproveedor->isChecked() ? aux += "1," : aux += "0,";
    mver_nomalmacen->isChecked() ? aux += "1," : aux += "0,";
    mver_descforma_pago->isChecked() ? aux += "1," : aux += "0,";
    mver_totalalbaranproveedor->isChecked() ? aux += "1," : aux += "0,";
    mver_totalbaseimp->isChecked() ? aux += "1," : aux += "0,";
    mver_totalimpuestos->isChecked() ? aux += "1," : aux += "0,";
    QFile file(confpr->valor(CONF_DIR_USER) + "confalbaranesproveedor.cfn");

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << aux << "\n";
        file.close();
    };
};


void AlbaranesProveedor::cargaconfig() {
    QFile file(confpr->valor(CONF_DIR_USER) + "confalbaranesproveedor.cfn");
    QString line;
    if (file.open( QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        /// line of text excluding '\n'
        line = stream.readLine();
        file.close();
    } else {
        return;
    };

    mver_idalbaranp->setChecked(line.at(0) == '1');
    mver_numalbaranp->setChecked(line.at(2) == '1');
    mver_descalbaranp->setChecked(line.at(4) == '1');
    mver_refalbaranp->setChecked(line.at(6) == '1');
    mver_fechaalbaranp->setChecked(line.at(8) == '1');
    mver_loginusuario->setChecked(line.at(10) == '1');
    mver_comentalbaranp->setChecked(line.at(12) == '1');
    mver_procesadoalbaranp->setChecked(line.at(14) == '1');
    mver_idproveedor->setChecked(line.at(16) == '1');
    mver_idforma_pago->setChecked(line.at(18) == '1');
    mver_idalmacen->setChecked(line.at(20) == '1');
    mver_nomproveedor->setChecked(line.at(22) == '1');
    mver_nomalmacen->setChecked(line.at(24) == '1');
    mver_descforma_pago->setChecked(line.at(26) == '1');
    mver_totalalbaranproveedor->setChecked(line.at(28) == '1');
    mver_totalbaseimp->setChecked(line.at(30) == '1');
    mver_totalimpuestos->setChecked(line.at(32) == '1');
};


void AlbaranesProveedor::s_configurar() {
    if (mver_idalbaranp->isChecked()) {
        mui_list->showColumn(COL_IDALBARANP);
    } else {
        mui_list->hideColumn(COL_IDALBARANP);
    };

    if (mver_numalbaranp->isChecked()) {
        mui_list->showColumn(COL_NUMALBARANP);
    } else {
        mui_list->hideColumn(COL_NUMALBARANP);
    };

    if (mver_descalbaranp->isChecked()) {
        mui_list->showColumn(COL_DESCALBARANP);
    } else {
        mui_list->hideColumn(COL_DESCALBARANP);
    };

    if (mver_refalbaranp->isChecked()) {
        mui_list->showColumn(COL_REFALBARANP);
    } else {
        mui_list->hideColumn(COL_REFALBARANP);
    };

    if (mver_fechaalbaranp->isChecked()) {
        mui_list->showColumn(COL_FECHAALBARANP);
    } else {
        mui_list->hideColumn(COL_FECHAALBARANP);
    };

    if (mver_comentalbaranp->isChecked()) {
        mui_list->showColumn(COL_COMENTALBARANP);
    } else {
        mui_list->hideColumn(COL_COMENTALBARANP);
    };

    if (mver_procesadoalbaranp->isChecked()) {
        mui_list->showColumn(COL_PROCESADOALBARANP);
    } else {
        mui_list->hideColumn(COL_PROCESADOALBARANP);
    };

    if (mver_idproveedor->isChecked()) {
        mui_list->showColumn(COL_IDPROVEEDOR);
    } else {
        mui_list->hideColumn(COL_IDPROVEEDOR);
    };

    if (mver_idforma_pago->isChecked()) {
        mui_list->showColumn(COL_IDFORMA_PAGO);
    } else {
        mui_list->hideColumn(COL_IDFORMA_PAGO);
    };

    if (mver_idalmacen->isChecked()) {
        mui_list->showColumn(COL_IDALMACEN);
    } else {
        mui_list->hideColumn(COL_IDALMACEN);
    };

    if (mver_nomproveedor->isChecked()) {
        mui_list->showColumn(COL_NOMPROVEEDOR);
    } else {
        mui_list->hideColumn(COL_NOMPROVEEDOR);
    };

    if (mver_nomalmacen->isChecked()) {
        mui_list->showColumn(COL_NOMALMACEN);
    } else {
        mui_list->hideColumn(COL_NOMALMACEN);
    };

    if (mver_descforma_pago->isChecked()) {
        mui_list->showColumn(COL_DESCFORMA_PAGO);
    } else {
        mui_list->hideColumn(COL_DESCFORMA_PAGO);
    }

    if (mver_totalalbaranproveedor->isChecked()) {
        mui_list->showColumn(COL_TOTALALBARANPROVEEDOR);
    } else {
        mui_list->hideColumn(COL_TOTALALBARANPROVEEDOR);
    };

    if (mver_totalbaseimp->isChecked()) {
        mui_list->showColumn(COL_TOTALBASEIMP);
    } else {
        mui_list->hideColumn(COL_TOTALBASEIMP);
    };

    if (mver_totalimpuestos->isChecked()) {
        mui_list->showColumn(COL_TOTALIMPUESTOS);
    } else {
        mui_list->hideColumn(COL_TOTALIMPUESTOS);
    };

    if (confpr->valor(CONF_MOSTRAR_ALMACEN) != "YES") {
        mui_list->hideColumn(COL_NOMALMACEN);
    };
};


AlbaranesProveedor::AlbaranesProveedor(QWidget *parent, const char *name, Qt::WFlags flag)
        : QWidget(parent, name, flag) {
    setupUi(this);
    cargaconfig();
    m_companyact = NULL;
    m_modo = 0;
    mdb_idalbaranp = "";
    meteWindow(caption(), this);
    hideBusqueda();
    hideConfiguracion();
};


AlbaranesProveedor::AlbaranesProveedor(company *comp, QWidget *parent, const char *name,
Qt::WFlags flag) : QWidget(parent,name, flag) {
    setupUi(this);
    m_companyact = comp;
    m_proveedor->setcompany(comp);
    m_articulo->setcompany(comp);
    cargaconfig();
    presenta();
    m_modo = 0;
    mdb_idalbaranp = "";
    meteWindow(caption(), this);
    hideBusqueda();
    hideConfiguracion();
};


AlbaranesProveedor::~AlbaranesProveedor() {
    m_companyact->refreshAlbaranesProveedor();
    m_companyact->sacaWindow(this);
    guardaconfig();
};



void AlbaranesProveedor::presenta() {
    _depura("AlbaranesProveedor::presenta().", 1);

    if (m_companyact != NULL ) {

        cursor2 * cur = m_companyact->cargacursor("SELECT *, calctotalalbpro(idalbaranp) AS total, calcbimpalbpro(idalbaranp) AS base, calcimpuestosalbpro(idalbaranp) AS impuestos FROM albaranp LEFT " \
                        "JOIN proveedor ON albaranp.idproveedor = " \
                        "proveedor.idproveedor LEFT JOIN almacen ON " \
                        "albaranp.idalmacen=almacen.idalmacen LEFT JOIN " \
                        "forma_pago ON albaranp.idforma_pago = " \
                        "forma_pago.idforma_pago WHERE 1=1 " + generaFiltro());

        mui_list->cargar(cur);
        delete cur;

        /// Hacemos el calculo del total.
        cur = m_companyact->cargacursor("SELECT SUM(calctotalalbpro(idalbaranp)) " \
                                        "AS total FROM albaranp LEFT JOIN proveedor ON " \
                                        "albaranp.idproveedor = proveedor.idproveedor LEFT " \
                                        "JOIN almacen ON albaranp.idalmacen=almacen.idalmacen " \
                                        "WHERE 1=1 " + generaFiltro());
        m_total->setText(cur->valor("total"));
        delete cur;
    }
    s_configurar();
    _depura("END AlbaranesProveedor::presenta().", 0);
};


QString AlbaranesProveedor::generaFiltro() {
    /// Tratamiento de los filtros.
    _depura("Tratamos el filtro.", 0);
    QString filtro = "";

    if (m_filtro->text() != "") {
        filtro = " AND ( descalbaranp LIKE '%" + m_filtro->text() + "%' ";
        filtro +=" OR nomproveedor LIKE '%" + m_filtro->text() + "%') ";
    } else {
        filtro = "";
    };

    if (m_proveedor->idproveedor() != "") {
        filtro += " AND albaranp.idproveedor=" + m_proveedor->idproveedor();
    };

    if (!m_procesados->isChecked()) {
        filtro += " AND NOT procesadoalbaranp";
    };

    if (m_articulo->idarticulo() != "") {
        filtro += " AND idalbaranp IN (SELECT DISTINCT idalbaranp FROM lalbaranp " \
                  "WHERE idarticulo='" + m_articulo->idarticulo() + "')";
    };

    if (m_fechain->text() != "") {
        filtro += " AND fechaalbaranp >= '" + m_fechain->text() + "' ";
    };

    if (m_fechafin->text() != "") {
        filtro += " AND fechaalbaranp <= '" + m_fechafin->text() + "' ";
    };

    //filtro += " ORDER BY idalbaranp";
    return (filtro);
};




void AlbaranesProveedor::editar(int  row) {
    _depura("AlbaranesProveedor::editar",0);
    mdb_idalbaranp = mui_list->DBvalue(QString("idalbaranp"),row);
    if (m_modo ==0 ) {
        AlbaranProveedorView *prov = new AlbaranProveedorView(m_companyact,0,theApp->translate("Edicion de Albaranes a Proveedor", "company"));
        if (prov->cargar(mdb_idalbaranp)) {
            return;
        }
        m_companyact->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idalbaranp));
        // close();
    }// end if
    _depura("END AlbaranesProveedor::editar",0);
}

void AlbaranesProveedor::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        editar(a);
    else
        _depura("Debe seleccionar una linea",2);
}





void AlbaranesProveedor::imprimir() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "albaranesproveedor.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "albaranesproveedor.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.ascii());

    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) +
                  "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) +
                  "logo.jpg";
#endif

    system(archivologo.ascii());

    QFile file;
    file.setName(archivod);
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    /// Linea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += "<tr>";

    if (mver_idalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Id.</td>";
    }

    if (mver_numalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Num. Albaran</td>";
    }

    if (mver_descalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Descripcion</td>";
    }

    if (mver_refalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Referencia</td>";
    }

    if(mver_fechaalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Fecha</td>";
    }

    if(mver_loginusuario->isChecked()) {
        fitxersortidatxt += "<td>Usuario</td>";
    }

    if (mver_comentalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Comentarios</td>";
    }

    if (mver_procesadoalbaranp->isChecked()) {
        fitxersortidatxt += "<td>Procesado</td>";
    }

    if (mver_idproveedor->isChecked()) {
        fitxersortidatxt += "<td>Id. Proveedor</td>";
    }

    if (mver_idforma_pago->isChecked()) {
        fitxersortidatxt += "<td>Id. F. Pago</td>";
    }

    if (mver_idalmacen->isChecked()) {
        fitxersortidatxt += "<td>Id. Almacen</td>";
    }

    if (mver_nomproveedor->isChecked()) {
        fitxersortidatxt += "<td>Proveedor</td>";
    }

    if (mver_nomalmacen->isChecked()) {
        fitxersortidatxt += "<td>Almacen</td>";
    }

    if (mver_descforma_pago->isChecked()) {
        fitxersortidatxt += "<td>F. Pago</td>";
    }

    if (mver_totalalbaranproveedor->isChecked()) {
        fitxersortidatxt += "<td>Total</td>";
    }

    if (mver_totalbaseimp->isChecked()) {
        fitxersortidatxt += "<td>Base Imp.</td>";
    }

    if (mver_totalimpuestos->isChecked()) {
        fitxersortidatxt += "<td>Impuestos</td>";
    }

    fitxersortidatxt += "</tr>";
    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM albaranp LEFT JOIN proveedor " \
                   "ON albaranp.idproveedor = proveedor.idproveedor LEFT JOIN almacen ON " \
                   "albaranp.idalmacen=almacen.idalmacen LEFT JOIN forma_pago ON " \
                   "albaranp.idforma_pago = forma_pago.idforma_pago WHERE 1=1 " +
                   generaFiltro());
    while(!cur->eof()) {
        fitxersortidatxt += "<tr>";

        if (mver_idalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("idalbaranp") + "</td>";
        };

        if (mver_numalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("numalbaranp") + "</td>";
        };

        if (mver_descalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("descalbaranp") + "</td>";
        };

        if (mver_refalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("refalbaranp") + "</td>";
        };

        if (mver_fechaalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("fechaalbaranp") + "</td>";
        };

        if (mver_loginusuario->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("idtrabajador") + "</td>";
        };

        if (mver_comentalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("comentalbaranp") + "</td>";
        };

        if (mver_procesadoalbaranp->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("procesadoalbaranp") + "</td>";
        };

        if (mver_idproveedor->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("idproveedor") + "</td>";
        };

        if (mver_idforma_pago->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("idforma_pago") + "</td>";
        };

        if (mver_idalmacen->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("idalmacen") + "</td>";
        };

        if (mver_nomproveedor->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("nomproveedor") + "</td>";
        };

        if (mver_nomalmacen->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("nomalmacen") + "</td>";
        };

        if (mver_descforma_pago->isChecked()) {
            fitxersortidatxt += "<td>" + cur->valor("descforma_pago") + "</td>";
        };

        /// Calculamos el total del presupuesto y lo presentamos.
        cursor2 *cur1 = m_companyact->cargacursor("SELECT calctotalalbpro(" +
                        cur->valor("idalbaranp") + ") AS total, calcbimpalbpro(" +
                        cur->valor("idalbaranp") + ") AS base, calcimpuestosalbpro(" +
                        cur->valor("idalbaranp") + ") AS impuestos");

        if (mver_totalalbaranproveedor->isChecked()) {
            fitxersortidatxt += "<td>" + cur1->valor("total") + "</td>";
        };

        if (mver_totalbaseimp->isChecked()) {
            fitxersortidatxt += "<td>" + cur1->valor("base") + "</td>";
        };

        if (mver_totalimpuestos->isChecked()) {
            fitxersortidatxt += "<td>" + cur1->valor("impuestos") + "</td>";
        };

        delete cur1;
        fitxersortidatxt += "</tr>";
        cur->siguienteregistro();
    };

    delete cur;
    fitxersortidatxt += "</blockTable>";
    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }

    invocaPDF("albaranesproveedor");
}


void AlbaranesProveedor::on_mui_borrar_clicked()  {
    _depura("AlbaranesProveedor::on_mui_borrar_clicked", 0);
    mdb_idalbaranp =  mui_list->DBvalue(QString("idalbaranp"));

    if (m_modo == 0 && mdb_idalbaranp != "") {
        AlbaranProveedorView *bud = new AlbaranProveedorView(m_companyact,
                                    m_companyact->m_pWorkspace,
                                    theApp->translate("Edicion de albaranes de proveedores",
                                                      "company"));
        bud->cargar(mdb_idalbaranp);
        bud->borrar();
        delete bud;
    };

    presenta();
}





/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================


AlbaranesProveedorListSubform::AlbaranesProveedorListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("albaranp");
    setDBCampoId("idalbaranp");
    addSHeader("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, "idalbaranp");
    addSHeader("numalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "numalbaranp");
    addSHeader("descalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "descalbaranp");
    addSHeader("refalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "refalbaranp");
    addSHeader("fechaalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechaalbaranp");
    addSHeader("comentalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentalbaranp");
    addSHeader("procesadoalbaranp", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "procesadoalbaranp");
    addSHeader("idproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idproveedor");
    addSHeader("idforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idforma_pago");
    addSHeader("idalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "idalmacen");
    addSHeader("nomproveedor", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomproveedor");
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomalmacen");
    addSHeader("descforma_pago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "descforma_pago");
    addSHeader("total", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "total");
    addSHeader("base", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "base");
    addSHeader("impuestos", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "impuestos");
    setinsercion(FALSE);
};
