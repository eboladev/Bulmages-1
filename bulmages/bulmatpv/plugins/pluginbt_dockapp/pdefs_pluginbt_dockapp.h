#ifndef PDEFS_PLUGINBT_DOCKAPP_H
#define PDEFS_PLUGINBT_DOCKAPP_H


#ifdef WIN32

#ifdef PLUGINBT_DOCKAPP
#define PLUGINBT_DOCKAPP_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_DOCKAPP_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_DOCKAPP_EXPORT

#endif


#endif

