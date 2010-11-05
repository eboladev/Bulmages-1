#ifndef PDEFS_PLUGINBF_PROMEDIOS_H
#define PDEFS_PLUGINBF_PROMEDIOS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_PROMEDIOS
#define PLUGINBF_PROMEDIOS_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_PROMEDIOS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_PROMEDIOS_EXPORT

#endif


#endif

