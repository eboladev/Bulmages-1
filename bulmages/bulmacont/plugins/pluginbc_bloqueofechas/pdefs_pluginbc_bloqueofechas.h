#ifndef PDEFS_PLUGINBC_BLOQUEOFECHAS_H
#define PDEFS_PLUGINBC_BLOQUEOFECHAS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_BLOQUEOFECHAS
#define PLUGINBC_BLOQUEOFECHAS_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_BLOQUEOFECHAS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_BLOQUEOFECHAS_EXPORT

#endif


#endif
