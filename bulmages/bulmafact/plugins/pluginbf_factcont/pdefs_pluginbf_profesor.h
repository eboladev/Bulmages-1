#ifndef PDEFS_PLUGINBF_PROFESOR_H
#define PDEFS_PLUGINBF_PROFESOR_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_PROFESOR
#define PLUGINBF_PROFESOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_PROFESOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_PROFESOR_EXPORT

#endif


#endif

