#ifndef PDEFS_PLUGINBL_REPORT2ODS_H
#define PDEFS_PLUGINBL_REPORT2ODS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_REPORT2ODS
#define PLUGINBL_REPORT2ODS_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_REPORT2ODS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_REPORT2ODS_EXPORT

#endif


#endif

