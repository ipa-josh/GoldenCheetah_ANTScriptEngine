#pragma once
#include <QtScript/QtScript>
#include "Train/ErgFile.h"

#define LOADRATE       1000 // rate at which load is adjusted


class RegularEvent : public QObject
{
    Q_OBJECT
    
    QScriptEngine &engine;
    QTimer *timer, *load_timer;
    QString fileName;
    QString contents;
    QTime start;
    QTime load_period;
    ErgFile *ergFile;
    long load_msecs;
public:
    RegularEvent(QString filename, QScriptEngine &engine, const int interval, ErgFile *erg=NULL) :
    engine(engine), fileName(filename), start(QTime::currentTime()), ergFile(erg), load_msecs(0)
    {
		QFile scriptFile(fileName);
		scriptFile.open(QIODevice::ReadOnly);
		QTextStream stream(&scriptFile);
		contents = stream.readAll();
		scriptFile.close();
		
		timer = new QTimer(this);
		load_timer = new QTimer(this);
		
		connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		connect(load_timer, SIGNAL(timeout()), this, SLOT(loadUpdate()));
		
		timer->start(interval);
		if(ergFile)
			load_timer->start(LOADRATE);
	}
	
	void stop() {
		timer->stop();
		load_timer->stop();
		QCoreApplication::exit(0);
	}

signals:
	void setLoad(double);
        
public slots:
    void update();
    void loadUpdate();
};
