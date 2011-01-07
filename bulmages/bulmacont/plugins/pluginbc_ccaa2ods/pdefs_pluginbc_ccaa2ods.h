#ifndef PDEFS_PLUGINBC_CCAA2ODS_H
#define PDEFS_PLUGINBC_CCAA2ODS_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_CCAA2ODS
#define PLUGINBC_CCAA2ODS_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_CCAA2ODS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_CCAA2ODS_EXPORT

#endif


#endif

