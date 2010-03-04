#ifndef PDEFS_PLUGINBL_TEMPLATE_H
#define PDEFS_PLUGINBL_TEMPLATE_H


#ifdef WIN32

#ifdef PLUGINBL_TEMPLATE
#define PLUGINBL_TEMPLATE_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_TEMPLATE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_TEMPLATE_EXPORT

#endif


#endif

