#ifndef PDEFS_PLUGINBT_CLIENTSYNC_H
#define PDEFS_PLUGINBT_CLIENTSYNC_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_CLIENTSYNC
#define PLUGINBT_CLIENTSYNC_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_CLIENTSYNC_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_CLIENTSYNC_EXPORT

#endif


#endif

