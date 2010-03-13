#ifndef PDEFS_PLUGINBT_IVAINCLUIDO_H
#define PDEFS_PLUGINBT_IVAINCLUIDO_H


#ifdef WIN32

#ifdef PLUGINBT_IVAINCLUIDO
#define PLUGINBT_IVAINCLUIDO_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_IVAINCLUIDO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_IVAINCLUIDO_EXPORT

#endif


#endif

