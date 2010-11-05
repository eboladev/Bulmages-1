#ifndef PDEFS_PLUGINBF_CONTRATO_H
#define PDEFS_PLUGINBF_CONTRATO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_CONTRATO
#define PLUGINBF_CONTRATO_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_CONTRATO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_CONTRATO_EXPORT

#endif


#endif

