#ifndef PDEFS_PLUGINBC_ASIENTO_H
#define PDEFS_PLUGINBC_ASIENTO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_ASIENTO
#define PLUGINBC_ASIENTO_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_ASIENTO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_ASIENTO_EXPORT

#endif


#endif

