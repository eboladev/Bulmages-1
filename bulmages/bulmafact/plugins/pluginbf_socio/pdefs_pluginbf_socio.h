#ifndef PDEFS_PLUGINBF_SOCIO_H
#define PDEFS_PLUGINBF_SOCIO_H


#ifdef WIN32

#ifdef PLUGINBF_SOCIO
#define PLUGINBF_SOCIO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_SOCIO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_SOCIO_EXPORT

#endif


#endif

