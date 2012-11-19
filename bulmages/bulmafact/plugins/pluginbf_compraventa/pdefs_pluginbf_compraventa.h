#ifndef PDEFS_PLUGINBF_COMPRAVENTA_H
#define PDEFS_PLUGINBF_COMPRAVENTA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_COMPRAVENTA
#define PLUGINBF_COMPRAVENTA_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_COMPRAVENTA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_COMPRAVENTA_EXPORT

#endif


#endif

