#ifndef PDEFS_PLUGINBF_CORRECTOR_H
#define PDEFS_PLUGINBF_CORRECTOR_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CORRECTOR
#define PLUGINBF_CORRECTOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CORRECTOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CORRECTOR_EXPORT

#endif


#endif

