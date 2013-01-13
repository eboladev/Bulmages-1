#ifndef PDEFS_PLUGINBF_Z2Z_H
#define PDEFS_PLUGINBF_Z2Z_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_Z2Z
#define PLUGINBF_Z2Z_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_Z2Z_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_Z2Z_EXPORT

#endif


#endif

