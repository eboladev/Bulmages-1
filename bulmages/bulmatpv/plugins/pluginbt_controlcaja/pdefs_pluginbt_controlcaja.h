#ifndef PDEFS_PLUGINBT_CONTROLCAJA_H
#define PDEFS_PLUGINBT_CONTROLCAJA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_CONTROLCAJA
#define PLUGINBT_CONTROLCAJA_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_CONTROLCAJA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_CONTROLCAJA_EXPORT

#endif


#endif

