#ifndef PDEFS_PLUGINBF_CORREO_H
#define PDEFS_PLUGINBF_CORREO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CORREO
#define PLUGINBF_CORREO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CORREO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CORREO_EXPORT

#endif


#endif

