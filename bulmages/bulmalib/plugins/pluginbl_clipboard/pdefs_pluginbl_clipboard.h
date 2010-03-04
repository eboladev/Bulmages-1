#ifndef PDEFS_PLUGINBL_CLIPBOARD_H
#define PDEFS_PLUGINBL_CLIPBOARD_H


#ifdef WIN32

#ifdef PLUGINBL_CLIPBOARD
#define PLUGINBL_CLIPBOARD_EXPORT __declspec(dllexport)
#else
#define PLUGINBL_CLIPBOARD_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBL_CLIPBOARD_EXPORT

#endif


#endif

