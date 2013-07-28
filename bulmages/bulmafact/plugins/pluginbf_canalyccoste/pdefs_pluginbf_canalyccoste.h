#ifndef PDEFS_PLUGINBC_CANALYCCOSTE_H
#define PDEFS_PLUGINBC_CANALYCCOSTE_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBC_CANALYCCOSTE
#define PLUGINBC_CANALYCCOSTE_EXPORT __declspec(dllexport)
#else
#define PLUGINBC_CANALYCCOSTE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBC_CANALYCCOSTE_EXPORT

#endif


#endif

