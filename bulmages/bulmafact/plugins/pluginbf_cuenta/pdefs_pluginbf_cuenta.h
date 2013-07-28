#ifndef PDEFS_PLUGINBF_CUENTA_H
#define PDEFS_PLUGINBF_CUENTA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CUENTA
#define PLUGINBF_CUENTA_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CUENTA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CUENTA_EXPORT

#endif


#endif

