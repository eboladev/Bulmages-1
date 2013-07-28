#ifndef PDEFS_PLUGINBF_ASIENTO_H
#define PDEFS_PLUGINBF_ASIENTO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_ASIENTO
#define PLUGINBF_ASIENTO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ASIENTO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ASIENTO_EXPORT

#endif


#endif

