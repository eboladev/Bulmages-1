#ifndef PDEFS_PLUGINBT_MESAS_H
#define PDEFS_PLUGINBT_MESAS_H


#ifdef WIN32

#ifdef PLUGINBT_MESAS
#define PLUGINBT_MESAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_MESAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_MESAS_EXPORT

#endif


#endif

