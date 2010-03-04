#ifndef PDEFS_PLUGINBF_TIPOTRABAJO_H
#define PDEFS_PLUGINBF_TIPOTRABAJO_H


#ifdef WIN32

#ifdef PLUGINBF_TIPOTRABAJO
#define PLUGINBF_TIPOTRABAJO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_TIPOTRABAJO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_TIPOTRABAJO_EXPORT

#endif


#endif

