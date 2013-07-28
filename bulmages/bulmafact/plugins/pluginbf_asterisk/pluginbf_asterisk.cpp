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

#include <QtWidgets/QToolButton>

#include "pluginbf_asterisk.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Asterisk", "0.11-002");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_asterisk", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView_Post ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG

    trab->addDbField ( "passasterisktrabajador", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Password." ) );
    trab->addDbField ( "validasiempreasterisktrabajador", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Valida siempre." ) );


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
    l->setMainCompany ( trab->mainCompany() );
    l->setDbTableName ( "valasterisk" );
    l->setDbFieldId ( "idvalasterisk" );
    l->addSubFormHeader ( "fechavalasterisk", BlDbField::DbDate, BlDbField::DbNotNull , BlSubFormHeader::DbNone, _ ( "Fecha" ) );
    l->addSubFormHeader ( "horavalasterisk", BlDbField::DbVarChar, BlDbField::DbNotNull , BlSubFormHeader::DbNone, _ ( "Hora" ) );
    l->addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone , _ ( "Almacen" ) );
    l->addSubFormHeader ( "aperturaalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "Apertura manyanas" ) );
    l->addSubFormHeader ( "cierrealmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "Cierre manyanas" ) );
    l->addSubFormHeader ( "apertura1almacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "Apertura tardes" ) );
    l->addSubFormHeader ( "cierre1almacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "Cierre tardes" ) );
    l->addSubFormHeader ( "archvalasterisk", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite , _ ( "Audicion" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite , _ ( "Id almacen" ) );
    l->addSubFormHeader ( "idvalasterisk", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id validacion" ) );
    l->addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id trabajador" ) );
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );
    trab->dialogChanges_setExcludedObject ( l->mui_list );
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
    BL_FUNC_DEBUG

    alm->addDbField ( "extasteriskalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Extension." ) );

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
    l->setMainCompany ( alm->mainCompany() );
    l->setDbTableName ( "valasterisk" );
    l->setDbFieldId ( "idvalasterisk" );
    l->addSubFormHeader ( "fechavalasterisk", BlDbField::DbDate, BlDbField::DbNotNull , BlSubFormHeader::DbNone, _ ( "Fecha" ) );
    l->addSubFormHeader ( "horavalasterisk", BlDbField::DbVarChar, BlDbField::DbNotNull , BlSubFormHeader::DbNone, _ ( "Hora" ) );
    l->addSubFormHeader ( "nomtrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Trabajador" ) );
    l->addSubFormHeader ( "apellidostrabajador", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Apellidos" ) );
    l->addSubFormHeader ( "archvalasterisk", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "Audicion" ) );
    l->addSubFormHeader ( "idalmacen", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id almacen" ) );
    l->addSubFormHeader ( "idvalasterisk", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id validacion" ) );
    l->addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id trabajador" ) );
    l->setInsert ( true );
    l->setDelete ( true );
    l->setSortingEnabled ( false );

    alm->mui_tab->addTab ( l, "Validaciones Asterisk" );

    
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
        query += "true";
    } else {
        query += "false";
    } // end if
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->mdb_idtrabajador );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->runQuery ( query );

    BfSubForm *l1 = trab->findChild<BfSubForm *> ( "m_validacionestrab" );
    l1->setColumnValue ( "idtrabajador", trab->mdb_idtrabajador );
    l1->save();
    trab->mainCompany() ->commit();
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

    BlDbRecordSet *cur = trab->mainCompany() ->loadQuery ( "SELECT passasterisktrabajador, validasiempreasterisktrabajador FROM trabajador WHERE idtrabajador = " + trab->mdb_idtrabajador );
    if ( !cur->eof() ) {
        l->setText ( cur->value( "passasterisktrabajador" ) );
        l9->setChecked ( ( cur->value( "validasiempreasterisktrabajador" ) == "t" ) );
    } // end if
    delete cur;

    /// Cargamos las validaciones de asterisk.
    BfSubForm *l1 = trab->findChild<BfSubForm *> ( "m_validacionestrab" );
    l1->load ( "SELECT * FROM valasterisk NATURAL LEFT JOIN almacen WHERE idtrabajador = " + trab->mdb_idtrabajador + " ORDER BY fechavalasterisk, horavalasterisk" );

    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_load ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_validacionesalm" );
    if ( form )
        form->load ( "SELECT * FROM valasterisk NATURAL LEFT JOIN trabajador WHERE idalmacen = " + fich->dbValue ( "idalmacen" ) + " ORDER BY fechavalasterisk, horavalasterisk" );
    return 0;
}


int BlForm_guardar_Post ( BlForm *fich )
{
    BlSubForm * form = fich->findChild<BlSubForm *> ( "m_validacionesalm" );
    if ( form ) {
        form->setColumnValue ( "idalmacen", fich->dbValue ( "idalmacen" ) );
        form->save();
    }
    return 0;
}

