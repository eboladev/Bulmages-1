#ifndef PDEFS_PLUGINBF_TRABAJADOR_H
#define PDEFS_PLUGINBF_TRABAJADOR_H


#ifdef WIN32

#ifdef PLUGINBF_TRABAJADOR
#define PLUGINBF_TRABAJADOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_TRABAJADOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_TRABAJADOR_EXPORT

#endif


#endif

