#ifndef PDEFS_PLUGINBL_SUBFORM2ODS_H
#define PDEFS_PLUGINBL_SUBFORM2ODS_H


#ifdef WIN32

#ifdef PLUGINBL_SUBFORM2ODS
#define PLUGINBL_SUBFORM2ODS_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_SUBFORM2ODS_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_SUBFORM2ODS_EXPORT

#endif


#endif

