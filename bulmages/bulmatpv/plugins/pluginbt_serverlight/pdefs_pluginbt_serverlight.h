#ifndef PDEFS_PLUGINBT_SERVERLIGHT_H
#define PDEFS_PLUGINBT_SERVERLIGHT_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_SERVERLIGHT
#define PLUGINBT_SERVERLIGHT_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_SERVERLIGHT_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_SERVERLIGHT_EXPORT

#endif


#endif

