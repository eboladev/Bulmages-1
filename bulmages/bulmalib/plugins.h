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
	int lanza(QString func, void *clase);
	int lanza(QString func, void *clase, void **ret);
};

extern Plugins *g_plugins;

#endif

