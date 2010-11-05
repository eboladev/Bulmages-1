#ifndef PDEFS_PLUGINBF_CARTERAPAGOS_H
#define PDEFS_PLUGINBF_CARTERAPAGOS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CARTERAPAGOS
#define PLUGINBF_CARTERAPAGOS_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CARTERAPAGOS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CARTERAPAGOS_EXPORT

#endif


#endif

