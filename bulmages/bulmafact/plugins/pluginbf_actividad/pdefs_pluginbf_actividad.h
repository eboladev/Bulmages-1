#ifndef PDEFS_PLUGINBF_ACTIVIDAD_H
#define PDEFS_PLUGINBF_ACTIVIDAD_H


#ifdef WIN32

#ifdef PLUGINBF_ACTIVIDAD
#define PLUGINBF_ACTIVIDAD_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ACTIVIDAD_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ACTIVIDAD_EXPORT

#endif


#endif

