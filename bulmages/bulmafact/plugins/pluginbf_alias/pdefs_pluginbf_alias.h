#ifndef PDEFS_PLUGINBF_ALIAS_H
#define PDEFS_PLUGINBF_ALIAS_H


#ifdef WIN32

#ifdef PLUGINBF_ALIAS
#define PLUGINBF_ALIAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ALIAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ALIAS_EXPORT

#endif


#endif

