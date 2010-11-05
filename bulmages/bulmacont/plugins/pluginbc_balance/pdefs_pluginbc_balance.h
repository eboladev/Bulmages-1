#ifndef PDEFS_PLUGINBC_BALANCE_H
#define PDEFS_PLUGINBC_BALANCE_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_BALANCE
#define PLUGINBC_BALANCE_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_BALANCE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_BALANCE_EXPORT

#endif


#endif

