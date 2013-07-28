#ifndef PDEFS_PLUGINBC_AMORTIZACION_H
#define PDEFS_PLUGINBC_AMORTIZACION_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_AMORTIZACION
#define PLUGINBC_AMORTIZACION_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_AMORTIZACION_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_AMORTIZACION_EXPORT

#endif


#endif

