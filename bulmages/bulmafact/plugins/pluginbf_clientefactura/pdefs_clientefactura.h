#ifndef PDEFS_CLIENTEFACTURA_H
#define PDEFS_CLIENTEFACTURA_H


#ifdef Q_WS_WIN

#ifdef pluginbf_clientefactura
#define MY_EXPORT_CLIENTEFACTURA __declspec(dllexport)
#else
#define MY_EXPORT_CLIENTEFACTURA __declspec(dllimport)
#endif

#else
#define MY_EXPORT_CLIENTEFACTURA
#endif



#endif


