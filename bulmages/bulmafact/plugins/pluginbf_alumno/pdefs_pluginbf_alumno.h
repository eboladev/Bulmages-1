#ifndef PDEFS_PLUGINBF_ALUMNO_H
#define PDEFS_PLUGINBF_ALUMNO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_ALUMNO
#define PLUGINBF_ALUMNO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ALUMNO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ALUMNO_EXPORT

#endif


#endif

