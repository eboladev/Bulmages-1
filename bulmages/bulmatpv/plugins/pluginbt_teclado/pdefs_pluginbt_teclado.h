#ifndef PDEFS_PLUGINBT_TECLADO_H
#define PDEFS_PLUGINBT_TECLADO_H


#ifdef WIN32

#ifdef PLUGINBT_TECLADO
#define PLUGINBT_TECLADO_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_TECLADO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_TECLADO_EXPORT

#endif


#endif

