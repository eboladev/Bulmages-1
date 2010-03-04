#ifndef PDEFS_PLUGINBF_CORREO_H
#define PDEFS_PLUGINBF_CORREO_H


#ifdef WIN32

#ifdef PLUGINBF_CORREO
#define PLUGINBF_CORREO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CORREO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CORREO_EXPORT

#endif


#endif

