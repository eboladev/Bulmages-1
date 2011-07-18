#ifndef PDEFS_PLUGINBL_EXARO_H
#define PDEFS_PLUGINBL_EXARO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_EXARO
#define PLUGINBL_EXARO_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_EXARO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_EXARO_EXPORT

#endif


#endif

