#ifndef PDEFS_PLUGINBF_IVA_H
#define PDEFS_PLUGINBF_IVA_H


#ifdef WIN32

#ifdef PLUGINBF_IVA
#define PLUGINBF_IVA_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_IVA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_IVA_EXPORT

#endif


#endif

