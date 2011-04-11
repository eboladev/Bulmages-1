#ifndef PDEFS_PLUGINBC_REGISTROIVA_H
#define PDEFS_PLUGINBC_REGISTROIVA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_REGISTROIVA
#define PLUGINBC_REGISTROIVA_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_REGISTROIVA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_REGISTROIVA_EXPORT

#endif


#endif

