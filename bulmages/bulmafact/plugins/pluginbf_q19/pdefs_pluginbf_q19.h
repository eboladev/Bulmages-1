#ifndef PDEFS_PLUGINBF_Q19_H
#define PDEFS_PLUGINBF_Q19_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_Q19
#define PLUGINBF_Q19_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_Q19_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_Q19_EXPORT

#endif


#endif

