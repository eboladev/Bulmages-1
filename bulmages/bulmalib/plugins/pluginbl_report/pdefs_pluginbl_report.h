#ifndef PDEFS_PLUGINBL_REPORT_H
#define PDEFS_PLUGINBL_REPORT_H


#ifdef WIN32

#ifdef PLUGINBL_REPORT
#define PLUGINBL_REPORT_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_REPORT_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_REPORT_EXPORT

#endif


#endif

