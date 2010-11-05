#ifndef PDEFS_PLUGINBL_SUBFORM2SXC_H
#define PDEFS_PLUGINBL_SUBFORM2SXC_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_SUBFORM2SXC
#define PLUGINBL_SUBFORM2SXC_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_SUBFORM2SXC_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_SUBFORM2SXC_EXPORT

#endif


#endif

