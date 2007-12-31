
#include "qcombobox2.h"

/** Inicializa todos los componenetes a NULL para que no se produzcan confusiones
    sobre si un elemento ha sido creado o no.
    Conecta el activar un tipo con m_activated.
*/
/// \TODO: No deberia usarse m_activated como signal ya que confunde con una variable de clase.
/**
\param parent
**/
QComboBox2::QComboBox2 ( QWidget *parent )
        : QComboBox ( parent ), PEmpresaBase()
{
    _depura ( "QQComboBox2::QComboBox2", 0 );
    _depura ( "END QComboBox2::QComboBox2", 0 );
}


/// Se libera memoria destruyendo el cursor que almacena los tipos de IVA si es que esta creado.
/**
**/
QComboBox2::~QComboBox2()
{
    _depura ( "QComboBox2::~QComboBox2", 0 );
    _depura ( "END QComboBox2::~QComboBox2", 0 );
}


