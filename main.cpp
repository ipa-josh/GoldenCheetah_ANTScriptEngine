#include "ANT/ANT.h"
#include "ANT/ANTLogger.h"
#include "DeviceTypes.h"
#include "Deps.h"
#include "RegularEvent.h"
#include <QDir>
#include <QtScript/QtScript>
#include <QCommandLineParser>
#include <qmqtt/qmqtt_client.h>
#include "TcxWriter.h"


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

QString setup(QString &cyclist) {
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
	
	return home.absolutePath();
}


int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	
	QCommandLineParser parser;
    parser.setApplicationDescription("simplified version of GoldenCheetah allowing to script trainings");
    parser.addHelpOption();
    
    QCommandLineOption scriptInitOption(QStringList() << "i" << "script-init",
            "Script file run at startup.",
            "script-init",
            "scripts/init.js");
    parser.addOption(scriptInitOption);
    
    QCommandLineOption scriptRegularOption(QStringList() << "r" << "script-regular",
            "Script file run at regular events [interval].",
            "script-regular",
            "scripts/regular.js");
    parser.addOption(scriptRegularOption);
    
    QCommandLineOption athleteOption(QStringList() << "a" << "athlete",
            "Name of athlete",
            "athlete");
    parser.addOption(athleteOption);
    
    QCommandLineOption trainingFileOption(QStringList() << "t" << "trainingfile",
            "Filepath to training file (*.erg, *.crs or *.mrc)",
            "trainingfile");
    parser.addOption(trainingFileOption);
    
    parser.process(app);
    
    QString cyclist = parser.value(athleteOption);
	QString athletePath = setup(cyclist);
        
    // for Device Pairing the controller is called with parent = NULL
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

    ANT *myANTlocal = new ANT (parent, dc, cyclist);
    ANTLogger *logger = new ANTLogger(parent, athletePath);
    
    // Connect a logger
    app.connect(myANTlocal, SIGNAL(receivedAntMessage(const unsigned char, const ANTMessage ,const timeval )), logger, SLOT(logRawAntMessage(const unsigned char, const ANTMessage ,const timeval)));
    app.connect(myANTlocal, SIGNAL(sentAntMessage(const unsigned char, const ANTMessage ,const timeval )), logger, SLOT(logRawAntMessage(const unsigned char, const ANTMessage ,const timeval)));
    
    logger->open();
    
   /* 
    //start
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
    int interval=1000;
    
    QScriptEngine engine;
    
    TcxRecord recorder;
    myANTlocal->getRealtimeData().setDataLogger(&recorder);
    
    QScriptValue scriptDevice = engine.newQObject(myANTlocal);
    engine.globalObject().setProperty("device", scriptDevice);
    
    QScriptValue scriptrtData = engine.newQObject(&myANTlocal->getRealtimeData());
    engine.globalObject().setProperty("rtData", scriptrtData);
    
    engine.globalObject().setProperty("interval", interval);
    
    if(parser.isSet(trainingFileOption))
		engine.globalObject().setProperty("erg_path", parser.value(trainingFileOption));
    
    QString fileName(parser.value(scriptInitOption));
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
    
    QMQTT::Client *mqtt_client = NULL;
    if(engine.globalObject().property("mqtt_host").isString()) {
		quint32 port = 1883;
		if(engine.globalObject().property("mqtt_port").isNumber())
			port = (quint32)engine.globalObject().property("mqtt_port").toUInt32();
			
		mqtt_client = new QMQTT::Client(engine.globalObject().property("mqtt_host").toString(), port);
		mqtt_client->connect();
		
		QScriptValue scriptMQTTClient = engine.newQObject(mqtt_client);
		engine.globalObject().setProperty("mqtt", scriptMQTTClient);
	}
    
    interval = engine.globalObject().property("interval").toInt32();
    
    ErgFile *ergFile = NULL;
    QString name = parser.value(scriptRegularOption);
    if(engine.globalObject().property("erg_path").isString()) {
		QString filename = engine.globalObject().property("erg_path").toString();
		name = filename;
		Context *ctxt = new Context();
		if(engine.globalObject().property("CP").isNumber()) ctxt->setCP(engine.globalObject().property("CP").toInt32());
		ergFile = new ErgFile(filename, ERG, ctxt);
	}
    
    RegularEvent regularEvent(parser.value(scriptRegularOption), engine, interval, ergFile);
    myANTlocal->connect(&regularEvent, SIGNAL(setLoad(double)), myANTlocal, SLOT(setLoad(double)));
    
    int ret = app.exec();
    
    myANTlocal->stop();
    logger->close();
    
    if(engine.globalObject().property("recorder_path").isString()) {
		QString filename = engine.globalObject().property("recorder_path").toString();
		QFile file(filename);
		if (file.open(QIODevice::ReadWrite))
			recorder.write(name, cyclist, file);
		else
			qDebug()<<QString::fromLatin1("failed to open file for recording: %0").arg(filename);
	}
    
    return ret;
}
