#ifndef PDEFS_PLUGINBL_FEEDS_H
#define PDEFS_PLUGINBL_FEEDS_H


#ifdef WIN32

#ifdef PLUGINBL_FEEDS
#define PLUGINBL_FEEDS_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_FEEDS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_FEEDS_EXPORT

#endif


#endif

