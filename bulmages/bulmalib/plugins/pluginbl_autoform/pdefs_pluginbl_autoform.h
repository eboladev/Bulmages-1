#ifndef PDEFS_PLUGINBL_AUTOFORM_H
#define PDEFS_PLUGINBL_AUTOFORM_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_AUTOFORM
#define PLUGINBL_AUTOFORM_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_AUTOFORM_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_AUTOFORM_EXPORT

#endif


#endif

