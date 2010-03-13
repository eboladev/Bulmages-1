#ifndef PDEFS_PLUGINBT_SERVERSYNC_H
#define PDEFS_PLUGINBT_SERVERSYNC_H


#ifdef WIN32

#ifdef PLUGINBT_SERVERSYNC
#define PLUGINBT_SERVERSYNC_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_SERVERSYNC_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_SERVERSYNC_EXPORT

#endif


#endif

