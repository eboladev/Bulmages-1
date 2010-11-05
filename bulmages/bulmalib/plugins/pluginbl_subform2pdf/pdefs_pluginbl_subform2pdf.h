#ifndef PDEFS_PLUGINBL_SUBFORM2PDF_H
#define PDEFS_PLUGINBL_SUBFORM2PDF_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBL_SUBFORM2PDF
#define PLUGINBL_SUBFORM2PDF_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_SUBFORM2PDF_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_SUBFORM2PDF_EXPORT

#endif


#endif

