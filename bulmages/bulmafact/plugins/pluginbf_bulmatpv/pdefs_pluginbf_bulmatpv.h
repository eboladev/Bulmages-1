#ifndef PDEFS_PLUGINBF_BULMATPV_H
#define PDEFS_PLUGINBF_BULMATPV_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_BULMATPV
#define PLUGINBF_BULMATPV_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_BULMATPV_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_BULMATPV_EXPORT

#endif


#endif

