#ifndef PDEFS_PLUGINBF_PROVEEDOR_H
#define PDEFS_PLUGINBF_PROVEEDOR_H


#ifdef WIN32

#ifdef PLUGINBF_PROVEEDOR
#define PLUGINBF_PROVEEDOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_PROVEEDOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_PROVEEDOR_EXPORT

#endif


#endif

