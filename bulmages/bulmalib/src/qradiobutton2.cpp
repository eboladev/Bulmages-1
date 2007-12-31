#include "qradiobutton2.h"
#include "funcaux.h"

/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/**
\param parent
**/
QRadioButton2::QRadioButton2 ( QWidget *parent )
        : QRadioButton ( parent )
{
    _depura ( "QQRadioButton2::QRadioButton2", 0 );
    _depura ( "END QRadioButton2::QRadioButton2", 0 );
}


/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/**
\param text
\param parent
**/
QRadioButton2::QRadioButton2 ( const QString & text, QWidget * parent )
        : QRadioButton ( text, parent )
{
    _depura ( "QQRadioButton2::QRadioButton2", 0 );
    _depura ( "END QRadioButton2::QRadioButton2", 0 );
}


/// Se libera memoria destruyendo el cursor que almacena los tipos de IVA si es que esta creado.
/**
**/
QRadioButton2::~QRadioButton2()
{
    _depura ( "QRadioButton2::~QRadioButton2", 0 );
    _depura ( "END QRadioButton2::~QRadioButton2", 0 );
}
