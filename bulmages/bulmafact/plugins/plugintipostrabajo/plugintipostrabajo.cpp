/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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
#include <QToolButton>

#include "plugintipostrabajo.h"
#include "listtipostrabajoview.h"
#include "tiptrab.h"
#include "busquedatipotrabajo.h"
#include "busquedaarticulo.h"

mytiptrab::mytiptrab() {}


mytiptrab::~mytiptrab() {}


void mytiptrab::elslot() {
    ListTiposTrabajoView *l = new ListTiposTrabajoView((Company *)m_bulmafact->getcompany(), 0);
    m_bulmafact->workspace()->addWindow(l);
    l->show();
}


void mytiptrab::inicializa(Bulmafact *bges) {
    /// Creamos el men&uacute;.
    m_bulmafact = bges;
    QAction *accion = new QAction("&Tipos de Trabajo", 0);
    accion->setStatusTip("Tipos de Trabajo");
    accion->setWhatsThis("Tipos de Trabajo");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuMaestro->insertAction(bges->actionTrabajadores, accion);
}


int entryPoint(Bulmafact *bges) {
    _depura("Estoy dentro del plugin de tipos de trabajo", 0);
    mytiptrab *plug = new mytiptrab();
    plug->inicializa(bges);
    return 0;
}

/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
int TrabajadorView_TrabajadorView_Post (TrabajadorView *trab) {
    _depura("TrabajadorView_TrabajadorView_Post", 0);

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing(2);
    hboxLayout160->setMargin(0);
    hboxLayout160->setObjectName(QString::fromUtf8("hboxLayout16"));

    QLabel *textLabel2_9_26 = new QLabel(trab->m_frameplugin);
    textLabel2_9_26->setObjectName(QString::fromUtf8("textLabel2_9_2"));
    hboxLayout160->addWidget(textLabel2_9_26);
    textLabel2_9_26->setText("Tipo Trabajo");

    BusquedaTipoTrabajo *tipotraba = new BusquedaTipoTrabajo(trab->m_frameplugin);
    tipotraba->setEmpresaBase(trab->empresaBase());
    tipotraba->setidtipotrabajo("");
    tipotraba->setObjectName(QString::fromUtf8("tipotraba"));
    hboxLayout160->addWidget(tipotraba);

   trab->m_frameplugin->setLayout(hboxLayout160);
   return 0;
}


int TrabajadorView_on_mui_guardar_clicked(TrabajadorView *trab) {

    BusquedaTipoTrabajo *l = trab->findChild<BusquedaTipoTrabajo *>("tipotraba");
    QString query = "UPDATE trabajador SET ";
    query += " idtipotrabajo = " + l->idtipotrabajo();
    query += " WHERE idtrabajador=" + trab->empresaBase()->sanearCadena(trab->idtrabajador());
    trab->empresaBase()->begin();
    trab->empresaBase()->ejecuta(query);
    trab->empresaBase()->commit();
    return 0;
}

int TrabajadorView_on_mui_lista_currentItemChanged_Post(TrabajadorView *trab) {
    BusquedaTipoTrabajo *l = trab->findChild<BusquedaTipoTrabajo *>("tipotraba");

    cursor2 *cur = trab->empresaBase()->cargacursor("SELECT idtipotrabajo FROM trabajador WHERE idtrabajador = " + trab->idtrabajador());
    if (!cur->eof()) {
	l->setidtipotrabajo(cur->valor("idtipotrabajo")); 
    }
    delete cur;
    return 0;
}

int AlmacenView_AlmacenView(AlmacenView *alm) {
   _depura("esxtoy en la clase almacen", 0);

    SubForm2Bf *form = new SubForm2Bf(alm);
    delete form->m_delegate;
    form->m_delegate = new QSubForm3BfDelegate(form);
    form->mui_list->setItemDelegate(form->m_delegate);
    form->setObjectName("mui_tipostrabajo");
    form->setEmpresaBase(alm->empresaBase());
    form->setDBTableName("almacentipotrabajo");
    form->setDBCampoId("idalmacen");
    form->addSHeader("nomtipotrabajo", DBCampo::DBvarchar, DBCampo::DBNoSave , SHeader::DBNone, QApplication::translate("AlmacenView", "ID nom tipo Trabajo"));
    form->addSHeader("numpers", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone , QApplication::translate("AlmacenView", "Numero de Cargos Necesarios"));
    form->addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate("AlmacenView", "ID almacen"));
    form->addSHeader("idtipotrabajo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, QApplication::translate("AlmacenView", "ID tipo Trabajo"));
    form->addSHeader("origidtipotrabajo", DBCampo::DBint, DBCampo::DBDupPrimaryKey | DBCampo::DBNoSave, SHeader::DBNoView, "idtipotrabajo");

    form->setinsercion(TRUE);
    form->setDelete(TRUE);
    form->setSortingEnabled(FALSE);

    /// Comprobamos que exista el layout.
       QHBoxLayout *m_hboxLayout1 = alm->mui_frameplugin->findChild<QHBoxLayout *>("hboxLayout1");
       if (!m_hboxLayout1) {
                m_hboxLayout1 = new QHBoxLayout(alm->mui_frameplugin);
                m_hboxLayout1->setSpacing(0);
                m_hboxLayout1->setMargin(0);
                m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
       } // end if
       m_hboxLayout1->addWidget(form);
    return 0;
}

int Ficha_cargar(Ficha *fich) {
	SubForm3 *form = fich->findChild<SubForm3 *>("mui_tipostrabajo");
	if (form) 
		form->cargar("SELECT *, tipotrabajo.idtipotrabajo AS origidtipotrabajo FROM almacentipotrabajo LEFT JOIN tipotrabajo ON almacentipotrabajo.idtipotrabajo = tipotrabajo.idtipotrabajo WHERE idalmacen = " + fich->DBvalue("idalmacen"));
	return 0;
}

int Ficha_guardar_Post(Ficha *fich) {
	SubForm3 *form = fich->findChild<SubForm3 *>("mui_tipostrabajo");
	if (form) {
		form->setColumnValue("idalmacen", fich->DBvalue("idalmacen"));
		form->guardar();
	}
	return 0;
}

/// ============================= SUBFORM3BFDELEGATE =============================================
/// ===============================================================
///  Tratamientos del Item Delegate
/// ===============================================================
QSubForm3BfDelegate::QSubForm3BfDelegate(QObject *parent = 0) : QSubForm2BfDelegate(parent) {
    _depura("QSubForm3BfDelegate::QSubForm3BfDelegate", 0);
    _depura("END QSubForm3BfDelegate::QSubForm3BfDelegate", 0);
}


QSubForm3BfDelegate::~QSubForm3BfDelegate() {
    _depura("QSubForm3BfDelegate::~QSubForm3BfDelegate", 0);
    _depura("END QSubForm3BfDelegate::~QSubForm3BfDelegate", 0);
}


QWidget *QSubForm3BfDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    _depura("QSubForm3BfDelegate::createEditor", 0);
    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());

    if (linea->nomcampo() == "nomtipotrabajo") {
        BusquedaTipoTrabajoDelegate *editor = new BusquedaTipoTrabajoDelegate(parent);
        editor->setEmpresaBase((Company *)m_subform->empresaBase());
        return editor;
    } else  {
        return QSubForm2BfDelegate::createEditor(parent, option, index);
    } // end if
    _depura("END QSubForm3BfDelegate::createEditor", 0);
}


void QSubForm3BfDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    _depura("QSubForm3BfDelegate::setModelData", 0);

    /// Si la fila o columna pasadas son invalidas salimos.
    if (index.column() < 0 || index.row() < 0)
        return;

    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    if (linea->nomcampo() == "nomtipotrabajo") {
        BusquedaTipoTrabajoDelegate *comboBox = static_cast<BusquedaTipoTrabajoDelegate*>(editor);
        QString value = comboBox->currentText();
        model->setData(index, value);
	m_subform->lineaat(index.row())->setDBvalue("idtipotrabajo", comboBox->id());
    } else {
        QSubForm2BfDelegate::setModelData(editor, model, index);
    } // end if
    _depura("END QSubForm3BfDelegate::setModelData", 0);
}


void QSubForm3BfDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    _depura("QSubForm3BfDelegate::setEditorData", 0);
    SHeader *linea;
    linea = m_subform->cabecera()->at(index.column());
    if (linea->nomcampo() == "nomtipotrabajo") {
        QString value = index.model()->data(index, Qt::DisplayRole).toString();
        BusquedaTipoTrabajoDelegate *comboBox = static_cast<BusquedaTipoTrabajoDelegate*>(editor);
        comboBox->set(value);
    } else {
        QSubForm2BfDelegate::setEditorData(editor, index);
    } // end if
    _depura("END QSubForm3BfDelegate::setEditorData", 0);
}


