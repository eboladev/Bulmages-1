#ifndef PDEFS_PLUGINBF_DUPLICARASIENTO_H
#define PDEFS_PLUGINBF_DUPLICARASIENTO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_DUPLICARASIENTO
#define PLUGINBF_DUPLICARASIENTO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_DUPLICARASIENTO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_DUPLICARASIENTO_EXPORT

#endif


#endif
