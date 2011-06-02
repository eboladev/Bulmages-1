#ifndef PDEFS_PLUGINBT_DEBUG_H
#define PDEFS_PLUGINBT_DEBUG_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_DEBUG
#define PLUGINBT_DEBUG_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_DEBUG_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_DEBUG_EXPORT

#endif


#endif

