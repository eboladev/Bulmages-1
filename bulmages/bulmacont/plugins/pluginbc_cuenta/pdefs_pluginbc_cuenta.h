#ifndef PDEFS_PLUGINBC_CUENTA_H
#define PDEFS_PLUGINBC_CUENTA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_CUENTA
#define PLUGINBC_CUENTA_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_CUENTA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_CUENTA_EXPORT

#endif


#endif

