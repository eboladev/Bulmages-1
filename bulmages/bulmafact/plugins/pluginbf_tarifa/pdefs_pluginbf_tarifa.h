#ifndef PDEFS_PLUGINBF_TARIFA_H
#define PDEFS_PLUGINBF_TARIFA_H


#ifdef WIN32

#ifdef PLUGINBF_TARIFA
#define PLUGINBF_TARIFA_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_TARIFA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_TARIFA_EXPORT

#endif


#endif

