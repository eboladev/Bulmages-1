#ifndef PDEFS_PLUGINBT_TICKET_H
#define PDEFS_PLUGINBT_TICKET_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBT_TICKET
#define PLUGINBT_TICKET_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_TICKET_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_TICKET_EXPORT

#endif


#endif

