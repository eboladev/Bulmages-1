#ifndef PDEFS_PLUGINBF_BANCO_H
#define PDEFS_PLUGINBF_BANCO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_BANCO
#define PLUGINBF_BANCO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_BANCO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_BANCO_EXPORT

#endif


#endif

