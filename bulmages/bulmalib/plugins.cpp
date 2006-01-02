#include "plugins.h"

#include "funcaux.h"

#include <QString>
#include <QLibrary>
#include <QStringList>


typedef void (*MyPrototype)(void *);
MyPrototype myFunction;

Plugins *g_plugins;


Plugins::Plugins() {}


Plugins::~Plugins() {}

void Plugins::cargaLibs(QString libs) {
    /// Hacemos la carga de los plugins.
    QString cad=libs;
    _depura("Carga de plugins: "+cad,0);
    QStringList plugins = QStringList::split( ";", cad );
    for ( QStringList::Iterator it = plugins.begin(); it != plugins.end(); ++it ) {
        QLibrary *lib= new QLibrary(*it);
        _depura("Resolviendo la libreria: "+*it+"\n", 2);
	lib->load();
        if (!lib->isLoaded()) {
            _depura("No se ha podido cargar la libreria\n",2);
        } else {
            m_lista.append(lib);
        }// end if
    }// end for
}

void Plugins::lanza(QString func, void *clase) {


    for (int i = 0; i < m_lista.size(); ++i) {
        myFunction = (MyPrototype) m_lista.at(i)->resolve( func );
        if ( myFunction ) {
            myFunction(clase);
        } else {
            _depura("No ha entrado la libreria\n",2);
        }// end if
    }


}

