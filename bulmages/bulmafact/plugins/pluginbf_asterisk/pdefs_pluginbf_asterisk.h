#ifndef PDEFS_PLUGINBF_ASTERISK_H
#define PDEFS_PLUGINBF_ASTERISK_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_ASTERISK
#define PLUGINBF_ASTERISK_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_ASTERISK_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_ASTERISK_EXPORT

#endif


#endif

