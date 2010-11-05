#ifndef PDEFS_PLUGINBF_EXAMPLE_H
#define PDEFS_PLUGINBF_EXAMPLE_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_EXAMPLE
#define PLUGINBF_EXAMPLE_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_EXAMPLE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_EXAMPLE_EXPORT

#endif


#endif

