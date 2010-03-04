#ifndef PDEFS_PLUGINBF_CLIENTECOBRO_H
#define PDEFS_PLUGINBF_CLIENTECOBRO_H


#ifdef WIN32

#ifdef PLUGINBF_CLIENTECOBRO
#define PLUGINBF_CLIENTECOBRO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CLIENTECOBRO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CLIENTECOBRO_EXPORT

#endif


#endif

