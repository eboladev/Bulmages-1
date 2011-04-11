#ifndef PDEFS_PLUGINBF_EFACTURA_H
#define PDEFS_PLUGINBF_EFACTURA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_EFACTURA
#define PLUGINBF_EFACTURA_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_EFACTURA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_EFACTURA_EXPORT

#endif


#endif

