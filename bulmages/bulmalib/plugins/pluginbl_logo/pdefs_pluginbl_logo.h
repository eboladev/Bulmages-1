#ifndef PDEFS_PLUGINBL_LOGO_H
#define PDEFS_PLUGINBL_LOGO_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_LOGO
#define PLUGINBL_LOGO_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_LOGO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_LOGO_EXPORT

#endif


#endif

