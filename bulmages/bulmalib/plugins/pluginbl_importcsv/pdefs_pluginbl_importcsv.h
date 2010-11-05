#ifndef PDEFS_PLUGINBL_IMPORTCSV_H
#define PDEFS_PLUGINBL_IMPORTCSV_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_IMPORTCSV
#define PLUGINBL_IMPORTCSV_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_IMPORTCSV_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_IMPORTCSV_EXPORT

#endif


#endif

