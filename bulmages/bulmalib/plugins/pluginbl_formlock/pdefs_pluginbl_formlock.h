#ifndef PDEFS_PLUGINBL_FORMLOCK_H
#define PDEFS_PLUGINBL_FORMLOCK_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_FORMLOCK
#define PLUGINBL_FORMLOCK_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_FORMLOCK_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_FORMLOCK_EXPORT

#endif


#endif

