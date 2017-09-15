#pragma once

/* from file TrainSidebar.h */

// Status settings
#define RT_MODE_ERGO        0x0001        // load generation modes
#define RT_MODE_SPIN        0x0002        // spinscan like modes
#define RT_MODE_SLOPE       0x0002        // same as spinscan but not so CT specific
#define RT_MODE_CALIBRATE   0x0004        // calibrate

/* from MainWindow.h */
#if defined(NOWEBKIT) && defined(WIN32)
// Macro to avoid Code Duplication in multiple files
// QtWebEngine puts it's cache into the User directory (only on Windows) - so do not show in list
# define SKIP_QTWE_CACHE \
   QStringList webEngineDirs; \
   webEngineDirs << "QtWebEngine" << "cache"; \
   if (webEngineDirs.contains(name)) continue;
#else
# define SKIP_QTWE_CACHE
#endif


/* from file Settings.h */
extern int OperatingSystem;

#define WINDOWS 1
#define LINUX   2
#define OSX     3
#define OPENBSD 4


#define __GC_HELPER__(x) const QString x = #x;

#if 0
__GC_HELPER__(GC_WHEELSIZE)
__GC_HELPER__(GC_DEV_COUNT)
__GC_HELPER__(GC_DEV_TYPE)
__GC_HELPER__(GC_DEV_NAME)
__GC_HELPER__(GC_DEV_SPEC)
__GC_HELPER__(GC_DEV_WHEEL)
__GC_HELPER__(GC_DEV_STRIDE)
__GC_HELPER__(GC_DEV_PROF)
__GC_HELPER__(GC_DEV_DEF)
__GC_HELPER__(GC_DEV_VIRTUAL)

class AppSettings {
	std::map<QString, QVariant> data_;
	
public:
    // standard access to global config
    QVariant value(const QObject *me, const QString key, const QVariant def = 0) ;
    void setValue(QString key, QVariant value);

    // access to athleteName specific config
    QVariant cvalue(QString athleteName, QString key, QVariant def = 0);

    void setCValue(QString athleteName, QString key, QVariant value);
};

extern AppSettings *appsettings;
#else

#include "Settings.h"
extern GSettings *appsettings;

#endif
