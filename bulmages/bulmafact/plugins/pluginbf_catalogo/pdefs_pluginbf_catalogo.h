#ifndef PDEFS_PLUGINBF_CATALOGO_H
#define PDEFS_PLUGINBF_CATALOGO_H


#ifdef WIN32

#ifdef PLUGINBF_CATALOGO
#define PLUGINBF_CATALOGO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CATALOGO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CATALOGO_EXPORT

#endif


#endif

