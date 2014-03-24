#ifndef PDEFS_PLUGINBL_ALARM_H
#define PDEFS_PLUGINBL_ALARM_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_ALARM
#define PLUGINBL_ALARM_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_ALARM_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_ALARM_EXPORT

#endif


#endif

