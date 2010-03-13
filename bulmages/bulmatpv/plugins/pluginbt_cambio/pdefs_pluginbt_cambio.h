#ifndef PDEFS_PLUGINBT_CAMBIO_H
#define PDEFS_PLUGINBT_CAMBIO_H


#ifdef WIN32

#ifdef PLUGINBT_CAMBIO
#define PLUGINBT_CAMBIO_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_CAMBIO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_CAMBIO_EXPORT

#endif


#endif

