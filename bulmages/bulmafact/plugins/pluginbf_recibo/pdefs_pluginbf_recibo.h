#ifndef PDEFS_PLUGINBF_RECIBO_H
#define PDEFS_PLUGINBF_RECIBO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_RECIBO
#define PLUGINBF_RECIBO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_RECIBO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_RECIBO_EXPORT

#endif


#endif

