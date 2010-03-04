#ifndef PDEFS_PLUGINBF_TUTOR_H
#define PDEFS_PLUGINBF_TUTOR_H


#ifdef WIN32

#ifdef PLUGINBF_TUTOR
#define PLUGINBF_TUTOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_TUTOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_TUTOR_EXPORT

#endif


#endif

