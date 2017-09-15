#include "ANT/ANT.h"
#include "ANT/ANTLogger.h"
#include "DeviceTypes.h"
#include "Deps.h"
#include <QDir>
#include <QtScript/QtScript>

/*Q_DECLARE_METATYPE(RealtimeData)

QScriptValue toScriptValue_RealtimeData(QScriptEngine *engine, const RealtimeData &s)
{
  QScriptValue obj = engine->newObject();
  obj.setProperty("name", s.getName());
  obj.setProperty("watts", s.getWatts());
  return obj;
}

void fromScriptValue_RealtimeData(const QScriptValue &obj, RealtimeData &s)
{
  s.setName( obj.property("name").toString().toLatin1().data() );
  s.setWatts( obj.property("watts").toNumber() );
}*/

void getDeviceConfiguration(QList<DeviceConfiguration> &supported_devices) {
	DeviceConfiguration *dc = NULL;
    DeviceConfigurations all;
    QList<DeviceConfiguration> Devices = all.getList();
    for (int i=0; i<Devices.count(); i++) {

        qDebug()<<"Device: "<<Devices.at(i).name;

        // Create the controllers for each device
        // we can call upon each of these when we need
        // to interact with the device
        if (Devices.at(i).type == DEV_ANTLOCAL) {
			supported_devices.append(Devices[i]);
        }
        else
			qDebug()<<"device type "<<Devices.at(i).name<<" NOT supported!";
    }
}

void setup(QString cyclist="") {
	//this is the path within the current directory where GC will look for
	//files to allow USB stick support
	QString localLibraryPath="Library/GoldenCheetah";
    QString oldLibraryPath=QDir::home().canonicalPath()+"/Library/GoldenCheetah";

	//these are the new platform-dependent library paths
#if defined(Q_OS_MACX)
	QString libraryPath="Library/GoldenCheetah";
#elif defined(Q_OS_WIN)
#if QT_VERSION > 0x050000 // windows and qt5
	QStringList paths=QStandardPaths::standardLocations(QStandardPaths::DataLocation);
	QString libraryPath = paths.at(0); 
#else // windows not qt5
	QString libraryPath=QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/GoldenCheetah";
#endif // qt5
#else // not windows or osx (must be Linux or OpenBSD)
	// Q_OS_LINUX et al
	QString libraryPath=".goldencheetah";
#endif //
        
	QString gcroot;
	
	// set global root directory
	// lets try the local library we've worked out...
	QDir home = QDir();
	if(QDir(localLibraryPath).exists() || home.exists(localLibraryPath)) {

		home.cd(localLibraryPath);

	} else {

		// YIKES !! The directory we should be using doesn't exist!
		home = QDir::home();
		if (home.exists(oldLibraryPath)) { // there is an old style path, lets fo there
			home.cd(oldLibraryPath);
		} else {

			if (!home.exists(libraryPath)) {
				if (!home.mkpath(libraryPath)) {

					// tell user why we aborted !
					qDebug()<<"Cannot create library directory "<<libraryPath;
					exit(2);
				}
			}
			home.cd(libraryPath);
		}
	}
	
	gcroot = home.canonicalPath();
	appsettings->initializeQSettingsGlobal(gcroot);
	
	QString homeDir = home.canonicalPath();
	QStringListIterator i(home.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    while (i.hasNext()) {
        QString name = i.next();
        SKIP_QTWE_CACHE  // skip Folder Names created by QTWebEngine on Windows
        
        qDebug()<<"Athlete: "<<name;
        if(cyclist.count()==0) cyclist = name;
	}
    
	if (home.cd(cyclist))
		appsettings->initializeQSettingsAthlete(homeDir, cyclist);
	else {
		qDebug()<<"Cannot read athlete information for "<<cyclist;
		exit(3);
	}
}


int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
    
	setup();
        
    // for Device Pairing the controller is called with parent = NULL
    QString cyclist;
    QString athletePath;
    QObject *parent = NULL;
    DeviceConfiguration *dc = NULL;
    
    QList<DeviceConfiguration> supported_devices;
    getDeviceConfiguration(supported_devices);
    
    if(supported_devices.count()) {
		// select the first device
		dc = &supported_devices[0];
	}
	else {
		qDebug()<<"no setting for any ANT device found!";
		return 1;
	}

    /*if (parent) {
        cyclist = parent->context->athlete->cyclist;
        athletePath = parent->context->athlete->home->root().canonicalPath();
    } else {
        cyclist = QString();
        athletePath = QDir::tempPath();
    }*/

    ANT *myANTlocal = new ANT (parent, dc, cyclist);
    ANTLogger *logger = new ANTLogger(parent, athletePath);
    
   /* 
    //start
    logger->open();
    myANTlocal->start();
    myANTlocal->setup();
    
    //myANTlocal->restart();
    
    // get latest telemetry
    RealtimeData rtData;
    myANTlocal->getRealtimeData(rtData);
    
    //int rc =  myANTlocal->stop();
    //logger->close();
    */
    RealtimeData rtData;
    
    QScriptEngine engine;
    //qScriptRegisterMetaType(engine, toScriptValue_RealtimeData, fromScriptValue_RealtimeData);
    
    QScriptValue scriptDevice = engine.newQObject(myANTlocal);
    engine.globalObject().setProperty("device", scriptDevice);
    
    QScriptValue scriptrtData = engine.newQObject(&myANTlocal->getRealtimeData());
    engine.globalObject().setProperty("rtData", scriptrtData);
    
    QString fileName("scripts/helloscript.js");
    QFile scriptFile(fileName);
    scriptFile.open(QIODevice::ReadOnly);
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
    
    QScriptValue result = engine.evaluate(contents, fileName);
    
    if (result.isError()) {
        qDebug()<<
                              QString::fromLatin1("%0:%1: %2")
                              .arg(fileName)
                              .arg(result.property("lineNumber").toInt32())
                              .arg(result.toString());
        return -1;
    }
    
    return a.exec();
}
