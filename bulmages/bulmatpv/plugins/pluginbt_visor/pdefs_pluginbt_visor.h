#ifndef PDEFS_PLUGINBT_VISOR_H
#define PDEFS_PLUGINBT_VISOR_H


#ifdef WIN32

#ifdef PLUGINBT_VISOR
#define PLUGINBT_VISOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_VISOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_VISOR_EXPORT

#endif


#endif

