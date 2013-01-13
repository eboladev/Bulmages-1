#ifndef PDEFS_PLUGINBF_QWEBCAM_H
#define PDEFS_PLUGINBF_QWEBCAM_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_QWEBCAM
#define PLUGINBF_QWEBCAM_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_QWEBCAM_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_QWEBCAM_EXPORT

#endif


#endif

