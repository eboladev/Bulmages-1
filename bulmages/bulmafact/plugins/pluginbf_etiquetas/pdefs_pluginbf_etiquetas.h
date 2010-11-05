#ifndef PDEFS_PLUGINBF_ETIQUETAS_H
#define PDEFS_PLUGINBF_ETIQUETAS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_ETIQUETAS
#define PLUGINBF_ETIQUETAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ETIQUETAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ETIQUETAS_EXPORT

#endif


#endif

