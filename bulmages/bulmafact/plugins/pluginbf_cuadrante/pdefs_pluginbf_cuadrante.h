#ifndef PDEFS_PLUGINBF_CUADRANTE_H
#define PDEFS_PLUGINBF_CUADRANTE_H


#ifdef WIN32

#ifdef PLUGINBF_CUADRANTE
#define PLUGINBF_CUADRANTE_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CUADRANTE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CUADRANTE_EXPORT

#endif


#endif

