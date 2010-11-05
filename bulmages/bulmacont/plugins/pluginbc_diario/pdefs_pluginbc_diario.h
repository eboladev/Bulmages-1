#ifndef PDEFS_PLUGINBC_DIARIO_H
#define PDEFS_PLUGINBC_DIARIO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_DIARIO
#define PLUGINBC_DIARIO_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_DIARIO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_DIARIO_EXPORT

#endif


#endif

