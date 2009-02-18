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

    trab->addDBCampo ( "passasterisktrabajador", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Password." ) );
    trab->addDBCampo ( "validasiempreasterisktrabajador", BlDbField::DBboolean, BlDbField::DBNothing, _( "Valida siempre." ) );


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
    BfSubForm *l = new BfSubForm ( trab );
    l->setObjectName ( QString::fromUtf8 ( "m_validacionestrab" ) );
    l->setMainCompany ( trab->empresaBase() );
    l->setDBTableName ( "valasterisk" );
    l->setDBCampoId ( "idvalasterisk" );
    l->addSubFormHeader ( "fechavalasterisk", BlDbField::DBdate, BlDbField::DBNotNull , BlSubFormHeader::DBNone, _( "Fecha" ) );
    l->addSubFormHeader ( "horavalasterisk", BlDbField::DBvarchar, BlDbField::DBNotNull , BlSubFormHeader::DBNone, _( "Hora" ) );
    l->addSubFormHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone , _( "Almacen" ) );
    l->addSubFormHeader ( "aperturaalmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite , _( "Apertura manyanas" ) );
    l->addSubFormHeader ( "cierrealmacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite , _( "Cierre manyanas" ) );
    l->addSubFormHeader ( "apertura1almacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite , _( "Apertura tardes" ) );
    l->addSubFormHeader ( "cierre1almacen", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite , _( "Cierre tardes" ) );
    l->addSubFormHeader ( "archvalasterisk", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite , _( "Audicion" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoWrite , _( "Id almacen" ) );
    l->addSubFormHeader ( "idvalasterisk", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id validacion" ) );
    l->addSubFormHeader ( "idtrabajador", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id trabajador" ) );
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

    alm->addDBCampo ( "extasteriskalmacen", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Extension." ) );

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( alm->mui_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Extension Asterisk" );

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


    /// Anyadimos el subformulario de validaciones
    BfSubForm *l = new BfSubForm ( alm );
    l->setObjectName ( QString::fromUtf8 ( "m_validacionesalm" ) );
    l->setMainCompany ( alm->empresaBase() );
    l->setDBTableName ( "valasterisk" );
    l->setDBCampoId ( "idvalasterisk" );
    l->addSubFormHeader ( "fechavalasterisk", BlDbField::DBdate, BlDbField::DBNotNull , BlSubFormHeader::DBNone, _( "Fecha" ) );
    l->addSubFormHeader ( "horavalasterisk", BlDbField::DBvarchar, BlDbField::DBNotNull , BlSubFormHeader::DBNone, _( "Hora" ) );
    l->addSubFormHeader ( "nomtrabajador", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Trabajador" ) );
    l->addSubFormHeader ( "apellidostrabajador", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Apellidos" ) );
    l->addSubFormHeader ( "archvalasterisk", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNoWrite, _( "Audicion" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id almacen" ) );
    l->addSubFormHeader ( "idvalasterisk", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id validacion" ) );
    l->addSubFormHeader ( "idtrabajador", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id trabajador" ) );
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

    BfSubForm *l1 = trab->findChild<BfSubForm *> ( "m_validacionestrab" );
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

    BlDbRecordSet *cur = trab->empresaBase() ->cargacursor ( "SELECT passasterisktrabajador, validasiempreasterisktrabajador FROM trabajador WHERE idtrabajador = " + trab->idtrabajador() );
    if ( !cur->eof() ) {
        l->setText ( cur->valor ( "passasterisktrabajador" ) );
        l9->setChecked ( ( cur->valor ( "validasiempreasterisktrabajador" ) == "t" ) );
    } // end if
    delete cur;

    /// Cargamos las validaciones de asterisk.
    BfSubForm *l1 = trab->findChild<BfSubForm *> ( "m_validacionestrab" );
    l1->cargar ( "SELECT * FROM valasterisk NATURAL LEFT JOIN almacen WHERE idtrabajador = " + trab->idtrabajador() + " ORDER BY fechavalasterisk, horavalasterisk" );

    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_cargar ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_validacionesalm" );
    if ( form )
        form->cargar ( "SELECT * FROM valasterisk NATURAL LEFT JOIN trabajador WHERE idalmacen = " + fich->DBvalue ( "idalmacen" ) + " ORDER BY fechavalasterisk, horavalasterisk" );
    return 0;
}


int BlForm_guardar_Post ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_validacionesalm" );
    if ( form ) {
        form->setColumnValue ( "idalmacen", fich->DBvalue ( "idalmacen" ) );
        form->guardar();
    }
    return 0;
}

