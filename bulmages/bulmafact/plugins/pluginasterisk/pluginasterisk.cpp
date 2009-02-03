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


///
/**
\return
**/
int entryPoint ( Bulmafact * )
{
    _depura ( "Estoy dentro del plugin de validacion con Asterisk", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginasterisk", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView_Post ( TrabajadorView *trab )
{
    _depura ( "TrabajadorView_TrabajadorView_Post", 0 );

    trab->addDBCampo ( "passasterisktrabajador", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Password." ) );
    trab->addDBCampo ( "validasiempreasterisktrabajador", DBCampo::DBboolean, DBCampo::DBNothing, _( "Valida siempre." ) );


    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Password para Validacion Asterisk" );

    QLineEdit *passtrab = new QLineEdit ( trab->m_frameplugin );
    passtrab->setObjectName ( QString::fromUtf8 ( "mui_passasterisktrabajador" ) );
    hboxLayout160->addWidget ( passtrab );

    QLabel *textLabel2_9_27 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_27->setObjectName ( QString::fromUtf8 ( "textLabel2_9_10" ) );
    hboxLayout160->addWidget ( textLabel2_9_27 );
    textLabel2_9_27->setText ( "Valida siempre" );

    QCheckBox *vals = new QCheckBox ( trab->m_frameplugin );
    vals->setObjectName ( QString::fromUtf8 ( "mui_validasiempreasterisktrabajador" ) );
    hboxLayout160->addWidget ( vals );


    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );


    /// Agregamos el subformulario de validaciones.
    SubForm2Bf *l = new SubForm2Bf ( trab );
    l->setObjectName ( QString::fromUtf8 ( "m_validacionestrab" ) );
    l->setEmpresaBase ( trab->empresaBase() );
    l->setDBTableName ( "valasterisk" );
    l->setDBCampoId ( "idvalasterisk" );
    l->addSHeader ( "fechavalasterisk", DBCampo::DBdate, DBCampo::DBNotNull , SHeader::DBNone, _( "Fecha" ) );
    l->addSHeader ( "horavalasterisk", DBCampo::DBvarchar, DBCampo::DBNotNull , SHeader::DBNone, _( "Hora" ) );
    l->addSHeader ( "nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , _( "Almacen" ) );
    l->addSHeader ( "aperturaalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , _( "Apertura mañanas" ) );
    l->addSHeader ( "cierrealmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , _( "Cierre mañanas" ) );
    l->addSHeader ( "apertura1almacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , _( "Apertura tardes" ) );
    l->addSHeader ( "cierre1almacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , _( "Cierre tardes" ) );
    l->addSHeader ( "archvalasterisk", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , _( "Audicion" ) );
    l->addSHeader ( "idalmacen", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite , _( "Id almacen" ) );
    l->addSHeader ( "idvalasterisk", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id validacion" ) );
    l->addSHeader ( "idtrabajador", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id trabajador" ) );
    l->setinsercion ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );
    trab->dialogChanges_setQObjectExcluido ( l->mui_list );
    trab->mui_tab->addTab ( l, "Validaciones Asterisk" );
    return 0;
}


///
/**
\param alm
\return
**/
int AlmacenView_AlmacenView ( AlmacenView *alm )
{
    _depura ( "AlmacenView_AlmacenView", 0 );

    alm->addDBCampo ( "extasteriskalmacen", DBCampo::DBvarchar, DBCampo::DBNothing, _( "Extension." ) );

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( alm->mui_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Extensión Asterisk" );

    QLineEdit *extalm = new QLineEdit ( alm->mui_frameplugin );
    extalm->setObjectName ( QString::fromUtf8 ( "mui_extasteriskalmacen" ) );
    hboxLayout160->addWidget ( extalm );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = alm->mui_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( alm->mui_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );


    /// Añadimos el subformulario de validaciones
    SubForm2Bf *l = new SubForm2Bf ( alm );
    l->setObjectName ( QString::fromUtf8 ( "m_validacionesalm" ) );
    l->setEmpresaBase ( alm->empresaBase() );
    l->setDBTableName ( "valasterisk" );
    l->setDBCampoId ( "idvalasterisk" );
    l->addSHeader ( "fechavalasterisk", DBCampo::DBdate, DBCampo::DBNotNull , SHeader::DBNone, _( "Fecha" ) );
    l->addSHeader ( "horavalasterisk", DBCampo::DBvarchar, DBCampo::DBNotNull , SHeader::DBNone, _( "Hora" ) );
    l->addSHeader ( "nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "Trabajador" ) );
    l->addSHeader ( "apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Apellidos" ) );
    l->addSHeader ( "archvalasterisk", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite, _( "Audicion" ) );
    l->addSHeader ( "idalmacen", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id almacen" ) );
    l->addSHeader ( "idvalasterisk", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id validacion" ) );
    l->addSHeader ( "idtrabajador", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id trabajador" ) );
    l->setinsercion ( TRUE );
    l->setDelete ( TRUE );
    l->setSortingEnabled ( FALSE );

    alm->mui_tab->addTab ( l, "Validaciones Asterisk" );

    _depura ( "END AlmacenView_AlmacenView", 0 );
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_guardar_clicked ( TrabajadorView *trab )
{

    QLineEdit * l = trab->findChild<QLineEdit *> ( "mui_passasterisktrabajador" );
    QCheckBox * l9 = trab->findChild<QCheckBox *> ( "mui_validasiempreasterisktrabajador" );

    QString query = "UPDATE trabajador SET ";
    query += " passasterisktrabajador = '" + l->text() + "'";
    query += " ,validasiempreasterisktrabajador = ";
    if ( l9->isChecked() ) {
        query += "TRUE";
    } else {
        query += "FALSE";
    } // end if
    query += " WHERE idtrabajador=" + trab->empresaBase() ->sanearCadena ( trab->idtrabajador() );
    trab->empresaBase() ->begin();
    trab->empresaBase() ->ejecuta ( query );

    SubForm2Bf *l1 = trab->findChild<SubForm2Bf *> ( "m_validacionestrab" );
    l1->setColumnValue ( "idtrabajador", trab->idtrabajador() );
    l1->guardar();
    trab->empresaBase() ->commit();
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView *trab )
{
    QLineEdit * l = trab->findChild<QLineEdit *> ( "mui_passasterisktrabajador" );
    QCheckBox * l9 = trab->findChild<QCheckBox *> ( "mui_validasiempreasterisktrabajador" );

    cursor2 *cur = trab->empresaBase() ->cargacursor ( "SELECT passasterisktrabajador, validasiempreasterisktrabajador FROM trabajador WHERE idtrabajador = " + trab->idtrabajador() );
    if ( !cur->eof() ) {
        l->setText ( cur->valor ( "passasterisktrabajador" ) );
        l9->setChecked ( ( cur->valor ( "validasiempreasterisktrabajador" ) == "t" ) );
    } // end if
    delete cur;

    /// Cargamos las validaciones de asterisk.
    SubForm2Bf *l1 = trab->findChild<SubForm2Bf *> ( "m_validacionestrab" );
    l1->cargar ( "SELECT * FROM valasterisk NATURAL LEFT JOIN almacen WHERE idtrabajador = " + trab->idtrabajador() + " ORDER BY fechavalasterisk, horavalasterisk" );

    return 0;
}


///
/**
\param fich
\return
**/
int Ficha_cargar ( Ficha *fich )
{
    SubForm3 * form = fich->findChild<SubForm3 *> ( "m_validacionesalm" );
    if ( form )
        form->cargar ( "SELECT * FROM valasterisk NATURAL LEFT JOIN trabajador WHERE idalmacen = " + fich->DBvalue ( "idalmacen" ) + " ORDER BY fechavalasterisk, horavalasterisk" );
    return 0;
}


int Ficha_guardar_Post ( Ficha *fich )
{
    SubForm3 * form = fich->findChild<SubForm3 *> ( "m_validacionesalm" );
    if ( form ) {
        form->setColumnValue ( "idalmacen", fich->DBvalue ( "idalmacen" ) );
        form->guardar();
    }
    return 0;
}


