#ifndef PDEFS_PLUGINBC_EXAMPLE_H
#define PDEFS_PLUGINBC_EXAMPLE_H


#ifdef WIN32

#ifdef PLUGINBC_EXAMPLE
#define PLUGINBC_EXAMPLE_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_EXAMPLE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_EXAMPLE_EXPORT

#endif


#endif

