#ifndef PDEFS_PLUGINBF_CLIENTEPEDIDO_H
#define PDEFS_PLUGINBF_CLIENTEPEDIDO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CLIENTEPEDIDO
#define PLUGINBF_CLIENTEPEDIDO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CLIENTEPEDIDO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CLIENTEPEDIDO_EXPORT

#endif


#endif
