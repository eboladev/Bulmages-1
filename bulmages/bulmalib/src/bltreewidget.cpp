#include <QMenu>

#include "blfunctions.h"
#include "bltreewidget.h"
#include "blplugins.h"
#include "local_blI18n.h"

BlTreeWidget::BlTreeWidget ( QWidget * parent ) : QTreeWidget(parent) {

    /// Disparamos los plugins.
    g_plugins->lanza ( "BlTreeWidget_BlTreeWidget_Post", this );
}


BlTreeWidget::~BlTreeWidget () {
}



///
/**
**/
void BlTreeWidget::creaMenu ( QMenu * )
{
    blDebug ( "BlSubForm:: CreaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
**/
void BlTreeWidget::procesaMenu ( QAction * )
{
    blDebug ( "BlSubForm:: procesaMenu", 0, "funcion para ser sobreescrita" );
}


///
/**
\return
**/
void BlTreeWidget::contextMenuEvent ( QContextMenuEvent * )
{
    blDebug ( "BlTreeWidget::contextMenuEvent", 0 );
//    QAction *del = NULL;
/*
    int row = currentRow();
    if ( row < 0 )
        return;
*/
    int col = currentColumn();
    if ( col < 0 )
        return;

    QMenu *popup = new QMenu ( this );

    /// Lanzamos el evento para que pueda ser capturado por terceros.
    emit pintaMenu ( popup );

    /// Lanzamos la propagacion del menu a traves de las clases derivadas.
    creaMenu ( popup );


/*
    if ( m_delete ) {
        popup->addSeparator();
        del = popup->addAction ( _ ( "Borrar registro" ) );
    } // end if
    popup->addSeparator();
*/
    QAction *ajustc = popup->addAction ( _ ( "Ajustar columa" ) );
//    QAction *ajustac = popup->addAction ( _ ( "Ajustar altura" ) );

    QAction *ajust = popup->addAction ( _ ( "Ajustar columnas" ) );
//    QAction *ajusta = popup->addAction ( _ ( "Ajustar alturas" ) );

//    popup->addSeparator();
//    QAction *verconfig = popup->addAction ( _ ( "Ver/Ocultar configurador de subformulario" ) );

    QAction *opcion = popup->exec ( QCursor::pos() );

    /// Si no hay ninguna opcion pulsada se sale sin hacer nada
    if ( !opcion ) {
	blDebug ( "END BlTreeWidget::contextMenuEvent", 0 );
	return;
    } // end if

//    if ( opcion == del )
//        remove ( row );
    if ( opcion == ajust ) {
	for (int i=0; i<columnCount(); i++) {
	    resizeColumnToContents (i);
	} // end for
    } // end if
//    if ( opcion == ajusta )
//        resizeRowsToContents();
    if ( opcion == ajustc )  {
        resizeColumnToContents ( col );
    } // end if
  //  if ( opcion == ajustac )
 //       resizeRowToContents ( row );
//    if ( opcion == verconfig )
//        toogleConfig();

    emit trataMenu ( opcion );

    /// Activamos las herederas.
    procesaMenu ( opcion );

    delete popup;
    blDebug ( "END BlTreeWidget::contextMenuEvent", 0 );
}

