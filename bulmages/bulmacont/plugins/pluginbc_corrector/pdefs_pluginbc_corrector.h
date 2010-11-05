#ifndef PDEFS_PLUGINBC_CORRECTOR_H
#define PDEFS_PLUGINBC_CORRECTOR_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_CORRECTOR
#define PLUGINBC_CORRECTOR_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_CORRECTOR_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_CORRECTOR_EXPORT

#endif


#endif

