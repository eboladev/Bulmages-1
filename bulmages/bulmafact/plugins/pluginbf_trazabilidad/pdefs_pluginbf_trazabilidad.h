#ifndef PDEFS_PLUGINBF_TRAZABILIDAD_H
#define PDEFS_PLUGINBF_TRAZABILIDAD_H


#ifdef WIN32

#ifdef PLUGINBF_TRAZABILIDAD
#define PLUGINBF_TRAZABILIDAD_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_TRAZABILIDAD_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_TRAZABILIDAD_EXPORT

#endif


#endif

