#ifndef PDEFS_PLUGINBF_ETIQUETADO_H
#define PDEFS_PLUGINBF_ETIQUETADO_H


#ifdef WIN32

#ifdef PLUGINBF_ETIQUETADO
#define PLUGINBF_ETIQUETADO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ETIQUETADO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ETIQUETADO_EXPORT

#endif


#endif

