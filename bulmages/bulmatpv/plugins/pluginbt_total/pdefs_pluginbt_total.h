#ifndef PDEFS_PLUGINBT_TOTAL_H
#define PDEFS_PLUGINBT_TOTAL_H


#ifdef WIN32

#ifdef PLUGINBT_TOTAL
#define PLUGINBT_TOTAL_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_TOTAL_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_TOTAL_EXPORT

#endif


#endif

