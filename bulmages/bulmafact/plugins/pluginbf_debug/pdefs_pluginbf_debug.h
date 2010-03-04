#ifndef PDEFS_PLUGINBF_DEBUG_H
#define PDEFS_PLUGINBF_DEBUG_H


#ifdef WIN32

#ifdef PLUGINBF_DEBUG
#define PLUGINBF_DEBUG_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_DEBUG_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_DEBUG_EXPORT

#endif


#endif

