#ifndef PDEFS_PLUGINBF_IVAINCLUIDO_H
#define PDEFS_PLUGINBF_IVAINCLUIDO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_IVAINCLUIDO
#define PLUGINBF_IVAINCLUIDO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_IVAINCLUIDO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_IVAINCLUIDO_EXPORT

#endif


#endif

