#ifndef PDEFS_PLUGINBL_BLOQCONF_H
#define PDEFS_PLUGINBL_BLOQCONF_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_BLOQCONF
#define PLUGINBL_BLOQCONF_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_BLOQCONF_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_BLOQCONF_EXPORT

#endif


#endif

