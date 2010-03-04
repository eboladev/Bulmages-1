#ifndef PDEFS_PLUGINBF_ASOCIACION_H
#define PDEFS_PLUGINBF_ASOCIACION_H


#ifdef WIN32

#ifdef PLUGINBF_ASOCIACION
#define PLUGINBF_ASOCIACION_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ASOCIACION_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ASOCIACION_EXPORT

#endif


#endif

