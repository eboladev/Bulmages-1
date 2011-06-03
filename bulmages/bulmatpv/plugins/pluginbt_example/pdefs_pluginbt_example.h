#ifndef PDEFS_PLUGINBT_EXAMPLE_H
#define PDEFS_PLUGINBT_EXAMPLE_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_EXAMPLE
#define PLUGINBT_EXAMPLE_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_EXAMPLE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_EXAMPLE_EXPORT

#endif


#endif

