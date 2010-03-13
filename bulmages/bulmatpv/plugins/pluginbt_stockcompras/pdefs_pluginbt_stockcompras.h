#ifndef PDEFS_PLUGINBT_STOCKCOMPRAS_H
#define PDEFS_PLUGINBT_STOCKCOMPRAS_H


#ifdef WIN32

#ifdef PLUGINBT_STOCKCOMPRAS
#define PLUGINBT_STOCKCOMPRAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_STOCKCOMPRAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_STOCKCOMPRAS_EXPORT

#endif


#endif

