
#include "qcombobox2.h"

/// Existe una instancia del objeto global theApp.
extern QApplication2 *theApp;


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
    _depura ( "QComboBox2::QComboBox2", 0 );
    m_cursorcombo = NULL;
    connect ( this, SIGNAL ( activated ( int ) ), this, SLOT ( m_activated ( int ) ) );
    connect(theApp, SIGNAL(fichaGuardada(Ficha *)), this, SLOT(onFichaGuardada(Ficha*)));

    m_query = "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia";
    m_tabla = "provincia";
    m_id = "idprovincia";
    m_valores["nomprovincia"] = "";
    m_valores["descpais"] = "";

    _depura ( "END QComboBox2::QComboBox2", 0 );
}


/// Se libera memoria destruyendo el cursor que almacena los tipos de IVA si es que esta creado.
/**
**/
QComboBox2::~QComboBox2()
{
    _depura ( "QComboBox2::~QComboBox2", 0 );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    _depura ( "END QComboBox2::~QComboBox2", 0 );
}

void QComboBox2::onFichaGuardada(Ficha *f) {
	mensajeInfo("Combo cambia ficha");
	if (m_tabla == f->tableName()) {
		setId(id());
	} // end if
}




/** Este metodo sirve para indicar al Widget cual es la provincia por defecto.
    Recarga todo el query de provincias y compone de nuevo el comboBox estableciendo como provincia
    por defecto aquella cuyo identificador coincide con el que se ha pasado por parametro.
*/
/**
\param idprovincia
**/
void QComboBox2::setId ( QString id )
{
    _depura ( "BusquedaProvincia::setIdProvincia", 0, id );
    if ( m_cursorcombo != NULL )
        delete m_cursorcombo;
    m_cursorcombo = empresaBase() ->cargacursor ( m_query );
    /// Si ha habido un problema con la base de datos salimos.
    if ( !m_cursorcombo ) return;

    int i = 0;
    int i1 = 0;
    clear();
    addItem ( "--" );

    while ( !m_cursorcombo->eof() ) {
        i ++;
        if ( m_cursorcombo->valor ( m_id ) == id )
            i1 = i;

	/// Inicializamos los valores de vuelta a ""
	QMapIterator<QString, QString> i(m_valores);
	QString cad;
	while (i.hasNext()) {
		i.next();
		cad = cad +" "+ m_valores.value(i.key());
	} // end while

        addItem ( cad );
        m_cursorcombo->siguienteregistro();
    } // end while

    setCurrentIndex ( i1 );
    _depura ( "END BusquedaProvincia::setIdProvincia", 0 );
}


///
/**
\param idprovincia
**/
void QComboBox2::setValorCampo ( QString id )
{
    _depura ( "BusquedaProvincia::setValorCampo", 0 );
    setId ( id );
    _depura ( "END BusquedaProvincia::setValorCampo", 0 );
}


/// SLOT que se activa al activar un elemento del desplegable
/**
\param index
**/
void QComboBox2::m_activated ( int index )
{
    _depura ( "BusquedaProvincia::m_activated", 0 );
    if ( index > 0 ) {
        emit ( valueChanged ( m_cursorcombo->valor ( m_id, index - 1 ) ) );
    } else {
        emit ( valueChanged ( "" ) );
    }
    _depura ( "END BusquedaProvincia::m_activated", 0 );
}


///
/**
\return
**/
QString QComboBox2::id()
{
    _depura ( "BusquedaProvincia::idProvincia", 0 );
    if ( currentIndex() > 0 ) {
        return m_cursorcombo->valor ( m_id, currentIndex() - 1 );
    } else {
        return "";
    } // end if
    _depura ( "END BusquedaProvincia::idProvincia", 0 );
}


///
/**
\return
**/
QString QComboBox2::valorCampo()
{
    _depura ( "BusquedaProvincia::valorCampo", 0 );
    _depura ( "END BusquedaProvincia::valorCampo", 0 );
    return id();
}

void QComboBox2::setQuery(QString q) {
	m_query = q;
}

void QComboBox2::setTableName(QString tableName) {
	m_tabla = tableName;
}

void QComboBox2::setCampoId(QString cid) {
	m_id = cid;
}
