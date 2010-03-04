#ifndef PDEFS_PLUGINBF_ARTICULO_H
#define PDEFS_PLUGINBF_ARTICULO_H


#ifdef WIN32

#ifdef PLUGINBF_ARTICULO
#define PLUGINBF_ARTICULO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ARTICULO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ARTICULO_EXPORT

#endif


#endif

