#include "RegularEvent.h"
    
void RegularEvent::update() {		
	engine.globalObject().setProperty("elapsed", start.secsTo(QTime::currentTime()));
	QScriptValue result = engine.evaluate(contents, fileName);
	
	if (result.isError()) {
		qDebug()<<
							  QString::fromLatin1("%0:%1: %2")
							  .arg(fileName)
							  .arg(result.property("lineNumber").toInt32())
							  .arg(result.toString());
		timer->stop();
		return;
	}
	else if(result.isBool() && result.toBool())
		stop();
}
	
void RegularEvent::startWorkout() {
	if(load_timer && load_timer->isActive())
		return;
		
	load_period.restart();
	if(ergFile)
		load_timer->start(LOADRATE);
}

int RegularEvent::loatAtRelative(long msecs) {
    int curLap=0;
    return ergFile->wattsAt(load_msecs+msecs, curLap);
}
	
void RegularEvent::loadUpdate()
{

    // the period between loadUpdate calls is not constant, and not exactly LOADRATE,
    // therefore, use a QTime timer to measure the load period
    load_msecs += load_period.restart();

    //if (status&RT_MODE_ERGO) {
        long load = loatAtRelative(0);

        // we got to the end!
        if (load == -100) {
            stop();
        } else {
            emit setLoad(load);
        }
    /*} else {
        slope = ergFile->gradientAt(displayWorkoutDistance*1000, curLap);

        if(displayWorkoutLap != curLap)
        {
            context->notifyNewLap();
        }
        displayWorkoutLap = curLap;

        // we got to the end!
        if (slope == -100) {
            Stop(DEVICE_OK);
        } else {
            foreach(int dev, activeDevices) Devices[dev].controller->setGradient(slope);
            context->notifySetNow(displayWorkoutDistance * 1000);
        }
    }*/
}
