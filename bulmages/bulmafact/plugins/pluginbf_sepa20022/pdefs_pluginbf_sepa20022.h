#ifndef PDEFS_PLUGINBF_SEPA20022_H
#define PDEFS_PLUGINBF_SEPA20022_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_SEPA20022
#define PLUGINBF_SEPA20022_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_SEPA20022_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_SEPA20022_EXPORT

#endif


#endif

