#ifndef PDEFS_PLUGINBF_BULMACONT_H
#define PDEFS_PLUGINBF_BULMACONT_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_BULMACONT
#define PLUGINBF_BULMACONT_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_BULMACONT_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_BULMACONT_EXPORT

#endif


#endif

