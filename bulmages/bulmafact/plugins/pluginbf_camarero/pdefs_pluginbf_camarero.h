#ifndef PDEFS_PLUGINBF_CAMARERO_H
#define PDEFS_PLUGINBF_CAMARERO_H


#ifdef WIN32

#ifdef PLUGINBF_CAMARERO
#define PLUGINBF_CAMARERO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CAMARERO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CAMARERO_EXPORT

#endif


#endif

