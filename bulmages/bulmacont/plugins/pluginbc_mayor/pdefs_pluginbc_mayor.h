#ifndef PDEFS_PLUGINBC_MAYOR_H
#define PDEFS_PLUGINBC_MAYOR_H


#ifdef WIN32

#ifdef PLUGINBC_MAYOR
#define PLUGINBC_MAYOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_MAYOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_MAYOR_EXPORT

#endif


#endif

