#ifndef PDEFS_PLUGINBF_FORMASDEPAGO_H
#define PDEFS_PLUGINBF_FORMASDEPAGO_H


#ifdef WIN32

#ifdef PLUGINBF_FORMASDEPAGO
#define PLUGINBF_FORMASDEPAGO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_FORMASDEPAGO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_FORMASDEPAGO_EXPORT

#endif


#endif

