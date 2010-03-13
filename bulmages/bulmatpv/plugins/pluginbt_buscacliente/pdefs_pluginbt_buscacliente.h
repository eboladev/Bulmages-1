#ifndef PDEFS_PLUGINBT_BUSCACLIENTE_H
#define PDEFS_PLUGINBT_BUSCACLIENTE_H


#ifdef WIN32

#ifdef PLUGINBT_BUSCACLIENTE
#define PLUGINBT_BUSCACLIENTE_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_BUSCACLIENTE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_BUSCACLIENTE_EXPORT

#endif


#endif

