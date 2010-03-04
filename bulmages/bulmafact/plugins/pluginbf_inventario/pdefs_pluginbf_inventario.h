#ifndef PDEFS_PLUGINBF_INVENTARIO_H
#define PDEFS_PLUGINBF_INVENTARIO_H


#ifdef WIN32

#ifdef PLUGINBF_INVENTARIO
#define PLUGINBF_INVENTARIO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_INVENTARIO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_INVENTARIO_EXPORT

#endif


#endif

