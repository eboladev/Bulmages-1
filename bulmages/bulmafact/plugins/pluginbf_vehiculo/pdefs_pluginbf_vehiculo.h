#ifndef PDEFS_PLUGINBF_VEHICULO_H
#define PDEFS_PLUGINBF_VEHICULO_H


#ifdef WIN32

#ifdef PLUGINBF_VEHICULO
#define PLUGINBF_VEHICULO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_VEHICULO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_VEHICULO_EXPORT

#endif


#endif

