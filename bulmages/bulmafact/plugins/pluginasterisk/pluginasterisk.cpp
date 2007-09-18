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

#include "pluginasterisk.h"


int entryPoint(Bulmafact *) {
    _depura("Estoy dentro del plugin de validacion con Asterisk", 0);
    return 0;
}

/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
int TrabajadorView_TrabajadorView_Post (TrabajadorView *trab) {
    _depura("TrabajadorView_TrabajadorView_Post", 0);

    trab->addDBCampo("passasterisktrabajador", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Trabajador", "Password."));


    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing(2);
    hboxLayout160->setMargin(0);
    hboxLayout160->setObjectName(QString::fromUtf8("hboxLayout19"));

    QLabel *textLabel2_9_26 = new QLabel(trab->m_frameplugin);
    textLabel2_9_26->setObjectName(QString::fromUtf8("textLabel2_9_9"));
    hboxLayout160->addWidget(textLabel2_9_26);
    textLabel2_9_26->setText("Password para Validacion Asterisk");

    QLineEdit *passtrab = new QLineEdit(trab->m_frameplugin);
    passtrab->setObjectName(QString::fromUtf8("mui_passasterisktrabajador"));
    hboxLayout160->addWidget(passtrab);


    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QVBoxLayout(trab->m_frameplugin);
        m_hboxLayout1->setSpacing(0);
        m_hboxLayout1->setMargin(0);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if

    m_hboxLayout1->addLayout(hboxLayout160);

    return 0;
}



int AlmacenView_AlmacenView(AlmacenView *alm) {
    _depura("esxtoy en la clase almacen", 0);

    alm->addDBCampo("extasteriskalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Almacen", "Extension."));

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing(2);
    hboxLayout160->setMargin(0);
    hboxLayout160->setObjectName(QString::fromUtf8("hboxLayout19"));

    QLabel *textLabel2_9_26 = new QLabel(alm->mui_frameplugin);
    textLabel2_9_26->setObjectName(QString::fromUtf8("textLabel2_9_9"));
    hboxLayout160->addWidget(textLabel2_9_26);
    textLabel2_9_26->setText("Extensión Asterisk");

    QLineEdit *extalm = new QLineEdit(alm->mui_frameplugin);
    extalm->setObjectName(QString::fromUtf8("mui_extasteriskalmacen"));
    hboxLayout160->addWidget(extalm);

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = alm->mui_frameplugin->findChild<QVBoxLayout *>("hboxLayout1");
    if (!m_hboxLayout1) {
        m_hboxLayout1 = new QVBoxLayout(alm->mui_frameplugin);
        m_hboxLayout1->setSpacing(0);
        m_hboxLayout1->setMargin(0);
        m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    } // end if

    m_hboxLayout1->addLayout(hboxLayout160);

    return 0;
}


int TrabajadorView_on_mui_guardar_clicked(TrabajadorView *trab) {

    QLineEdit *l = trab->findChild<QLineEdit *>("mui_passasterisktrabajador");
    QString query = "UPDATE trabajador SET ";
    query += " passasterisktrabajador = '" + l->text() + "'";
    query += " WHERE idtrabajador=" + trab->empresaBase()->sanearCadena(trab->idtrabajador());
    trab->empresaBase()->begin();
    trab->empresaBase()->ejecuta(query);
    trab->empresaBase()->commit();
    return 0;
}

int TrabajadorView_on_mui_lista_currentItemChanged_Post(TrabajadorView *trab) {
    QLineEdit *l = trab->findChild<QLineEdit *>("mui_passasterisktrabajador");

    cursor2 *cur = trab->empresaBase()->cargacursor("SELECT passasterisktrabajador FROM trabajador WHERE idtrabajador = " + trab->idtrabajador());
    if (!cur->eof()) {
	l->setText(cur->valor("passasterisktrabajador")); 
    }
    delete cur;
    return 0;
}


/*
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
*/

