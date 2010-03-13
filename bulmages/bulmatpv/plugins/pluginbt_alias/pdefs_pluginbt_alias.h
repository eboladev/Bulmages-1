#ifndef PDEFS_PLUGINBT_ALIAS_H
#define PDEFS_PLUGINBT_ALIAS_H


#ifdef WIN32

#ifdef PLUGINBT_ALIAS
#define PLUGINBT_ALIAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_ALIAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_ALIAS_EXPORT

#endif


#endif

