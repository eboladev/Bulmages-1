#ifndef PDEFS_PLUGINBC_DEPURACION_H
#define PDEFS_PLUGINBC_DEPURACION_H


#ifdef WIN32

#ifdef PLUGINBC_DEPURACION
#define PLUGINBC_DEPURACION_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_DEPURACION_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_DEPURACION_EXPORT

#endif


#endif

