#ifndef PDEFS_PLUGINBC_BALANCEARBOL_H
#define PDEFS_PLUGINBC_BALANCEARBOL_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_BALANCEARBOL
#define PLUGINBC_BALANCEARBOL_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_BALANCEARBOL_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_BALANCEARBOL_EXPORT

#endif


#endif

