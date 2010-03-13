#ifndef PDEFS_PLUGINBC_CAASLODS_H
#define PDEFS_PLUGINBC_CAASLODS_H


#ifdef WIN32

#ifdef PLUGINBC_CAASLODS
#define PLUGINBC_CAASLODS_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_CAASLODS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_CAASLODS_EXPORT

#endif


#endif

