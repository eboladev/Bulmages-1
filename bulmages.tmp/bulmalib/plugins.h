#ifndef PLUGINS_H
#define PLUGINS_H

#include "QList"
#include "QLibrary"

class Plugins {
private:
	QList<QLibrary *> m_lista;
public:
	Plugins();
	~Plugins();
	void cargaLibs(QString libs);
	void lanza(QString func, void *clase);
};

extern Plugins *g_plugins;

#endif

