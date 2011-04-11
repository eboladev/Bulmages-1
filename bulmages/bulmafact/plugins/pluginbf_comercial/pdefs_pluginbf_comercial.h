#ifndef PDEFS_PLUGINBF_COMERCIAL_H
#define PDEFS_PLUGINBF_COMERCIAL_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_COMERCIAL
#define PLUGINBF_COMERCIAL_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_COMERCIAL_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_COMERCIAL_EXPORT

#endif


#endif

