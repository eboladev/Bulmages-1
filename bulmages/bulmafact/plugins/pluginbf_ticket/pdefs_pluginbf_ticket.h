#ifndef PDEFS_PLUGINBF_TICKET_H
#define PDEFS_PLUGINBF_TICKET_H


#ifdef WIN32

#ifdef PLUGINBF_TICKET
#define PLUGINBF_TICKET_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_TICKET_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_TICKET_EXPORT

#endif


#endif

