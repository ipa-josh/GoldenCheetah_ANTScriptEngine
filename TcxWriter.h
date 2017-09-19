#pragma once

#include <QtXml>
#include <QTime>
#include "RealtimeData.h"


struct DataPoint {
	QTime time;
	double distance;
	double cadence;
	double watts;
	double hr;
	double speed;
	
	DataPoint() :
		time(QTime::currentTime()),
		distance(-1),
		cadence(-1),
		watts(-1),
		hr(-1),
		speed(-1)
	{}
	
	bool valid() {
		return
			distance>=0 ||
			cadence>=0 ||
			watts>=0 ||
			speed>=0 ||
			hr>=0;
	}
	
	void write(QDomDocument &doc, QDomElement &track) const {
		QDomText text;
		QDomElement trackpoint = doc.createElement("Trackpoint");
		track.appendChild(trackpoint);
            
		// time
		QDomElement time = doc.createElement("Time");
		text = doc.createTextNode(this->time.toString(Qt::ISODate));
		time.appendChild(text);
		trackpoint.appendChild(time);
            
		// distance - meters
		if (distance>=0) {
			QDomElement dist = doc.createElement("DistanceMeters");
			text = doc.createTextNode(QString("%1").arg((distance*1000)));
			dist.appendChild(text);
			trackpoint.appendChild(dist);
		}
            
		// cadence - rpm
		if (cadence>=0) {
			QDomElement dist = doc.createElement("Cadence");
			text = doc.createTextNode(QString("%1").arg(cadence));
			dist.appendChild(text);
			trackpoint.appendChild(dist);
		}
		
		QDomElement Extensions = doc.createElement("Extensions");
		trackpoint.appendChild(Extensions);
		QDomElement ns2TPX = doc.createElement("ns2:TPX");
		Extensions.appendChild(ns2TPX);
            
		// watts
		if (watts>=0) {
			QDomElement dist = doc.createElement("ns2:Watts");
			text = doc.createTextNode(QString("%1").arg(watts));
			dist.appendChild(text);
			trackpoint.appendChild(dist);
		}
            
		// watts
		if (speed>=0) {
			QDomElement dist = doc.createElement("ns2:Speed");
			text = doc.createTextNode(QString("%1").arg(speed/3.6));
			dist.appendChild(text);
			trackpoint.appendChild(dist);
		}
	}
};


class TcxRecord : public AbstractRealtimeDataLogger {
	DataPoint lastDataPoint;
	QList<DataPoint> dataPoints;
	
	void createNewDataPoint() {
		if(!lastDataPoint.valid()) return;
		
		lastDataPoint.time = QTime::currentTime();
		dataPoints.append(lastDataPoint);
		
		lastDataPoint = DataPoint();
	}
	
public:

	bool write(const QString &activity_name, const QString &athlete, QFile &file);

    void setWatts(double watts) {
		if(lastDataPoint.watts>=0) createNewDataPoint();
		lastDataPoint.watts = watts;
	}
    void setHr(double hr) {
		if(lastDataPoint.hr>=0) createNewDataPoint();
		lastDataPoint.hr = hr;
	}
    void setSpeed(double speed) {
		if(lastDataPoint.speed>=0) createNewDataPoint();
		lastDataPoint.speed = speed;
	}
    void setCadence(double aCadence) {
		if(lastDataPoint.cadence>=0) createNewDataPoint();
		lastDataPoint.cadence = aCadence;
	}
    void setAltDistance(double distance) {
		if(lastDataPoint.distance>=0) createNewDataPoint();
		lastDataPoint.distance = distance;
	}
    
    double getMaxSpeed() const {
		double v=-1;
		foreach(DataPoint d, dataPoints) v=std::max(v, d.speed);
		return v;
	}
    
    double getMaxHeartRate() const {
		double v=-1;
		foreach(DataPoint d, dataPoints) v=std::max(v, d.hr);
		return v;
	}
    
    double getAvgHeartRate() const {
		double v=0;
		int n=0;
		foreach(DataPoint d, dataPoints) if(d.hr>=0) {++n;v+=d.hr;}
		return v/std::max(1,n);
	}
};
