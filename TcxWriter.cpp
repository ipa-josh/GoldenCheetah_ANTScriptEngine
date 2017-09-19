#include "TcxWriter.h"


bool TcxRecord::write(const QString &activity_name, const QString &athlete, QFile &file) {
	createNewDataPoint();
	if(dataPoints.count()==0) {
		qDebug()<<"no data points captured -> no record created";
		return false;
	}
	
	QDomText text;
	QDomDocument doc;
	QDomProcessingInstruction hdr = doc.createProcessingInstruction("xml","version=\"1.0\"");
	doc.appendChild(hdr);

	// pwx
	QDomElement tcx = doc.createElementNS("http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2", "TrainingCenterDatabase");
	tcx.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	tcx.setAttribute("xsi:schemaLocation", "http://www.garmin.com/xmlschemas/ActivityExtension/v2 http://www.garmin.com/xmlschemas/ActivityExtensionv2.xsd http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2 http://www.garmin.com/xmlschemas/TrainingCenterDatabasev2.xsd");
	//tcx.setAttribute("version", "2.0");
	doc.appendChild(tcx);

	// activities, we just serialise one ride
	QString sport = "Biking";
	QDomElement activities = doc.createElement("Activities");
	tcx.appendChild(activities);
	QDomElement activity = doc.createElement("Activity");
	activity.setAttribute("Sport", sport); // was ride->getTag("Sport", "Biking") but must be Biking, Running or Other
	activities.appendChild(activity);

	// time
	QDomElement id = doc.createElement("Id");
	text = doc.createTextNode(dataPoints.at(0).time.toString(Qt::ISODate));
	id.appendChild(text);
	activity.appendChild(id);
	
	double _total_time = dataPoints.at(0).time.secsTo(dataPoints.at(dataPoints.count()-1).time);
	double _total_distance = dataPoints.at(dataPoints.count()-1).distance;
	double _max_speed = getMaxSpeed();
	double _max_heartrate = getMaxHeartRate();
	double _avg_heartrate = getAvgHeartRate();

    QDomElement lap = doc.createElement("Lap");
    lap.setAttribute("StartTime", dataPoints.at(0).time.toString(Qt::ISODate));
    activity.appendChild(lap);

	QDomElement lap_time = doc.createElement("TotalTimeSeconds");
	text = doc.createTextNode(QString("%1").arg(_total_time));
	//text = doc.createTextNode(ride->dataPoints().last()->secs);
	lap_time.appendChild(text);
	lap.appendChild(lap_time);

	QDomElement lap_distance = doc.createElement("DistanceMeters");
	text = doc.createTextNode(QString("%1").arg(1000*_total_distance));
	//text = doc.createTextNode(ride->dataPoints().last()->km);
	lap_distance.appendChild(text);
	lap.appendChild(lap_distance);

	QDomElement max_speed = doc.createElement("MaximumSpeed");
	text = doc.createTextNode(QString("%1")
		.arg(_max_speed / 3.6));
	max_speed.appendChild(text);
	lap.appendChild(max_speed);

	/*QDomElement lap_calories = doc.createElement("Calories");
	text = doc.createTextNode(QString("%1").arg((int)computed.value("total_work")->value(true)));
	lap_calories.appendChild(text);
	lap.appendChild(lap_calories);*/

	// optional per XSD, so only generate them if the data is to be exported and is present
	if (_max_heartrate>=0)
	{
		QDomElement avg_heartrate = doc.createElement("AverageHeartRateBpm");
		QDomElement value = doc.createElement("Value");
		text = doc.createTextNode(QString("%1").arg((int)_avg_heartrate));
		value.appendChild(text);
		avg_heartrate.appendChild(value);
		lap.appendChild(avg_heartrate);

		QDomElement max_heartrate = doc.createElement("MaximumHeartRateBpm");
		value = doc.createElement("Value");
		text = doc.createTextNode(QString("%1").arg((int)_max_heartrate));
		value.appendChild(text);
		max_heartrate.appendChild(value);
		lap.appendChild(max_heartrate);
	}

	QDomElement lap_intensity = doc.createElement("Intensity");
	text = doc.createTextNode("Active");
	lap_intensity.appendChild(text);
	lap.appendChild(lap_intensity);

	QDomElement lap_triggerMethod = doc.createElement("TriggerMethod");
	text = doc.createTextNode("Manual");
	lap_triggerMethod.appendChild(text);
	lap.appendChild(lap_triggerMethod);
	
	QDomElement track = doc.createElement("Track");
	lap.appendChild(track);
	for(int i=0; i<dataPoints.count(); i++)
		dataPoints.at(i).write(doc, track);
	
	
    if (!file.open(QIODevice::WriteOnly)) return(false);
    file.resize(0);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);
    out << doc.toByteArray(4);
    out.flush();
    file.close();
    
	return true;
}

#if 0
/*
<TrainingCenterDatabase>
	<Activities>
		<Activity Sport="Biking">
			<Id>2017-08-11T19:30:26.469Z</Id>
			<Lap StartTime="2017-08-11T19:17:30.000Z">
				<TotalTimeSeconds>776.827</TotalTimeSeconds>
				<DistanceMeters>8014.0</DistanceMeters>
				<MaximumSpeed>10.715604</MaximumSpeed>
				<Calories>194</Calories>
				<Intensity>Active</Intensity>
				<TriggerMethod>Time</TriggerMethod>
				<Track>
					<Trackpoint>
						<Time>2017-08-11T19:17:30.000Z</Time>
						<DistanceMeters>7.2</DistanceMeters>
						<Cadence>35</Cadence>
						<Extensions>
							<ns2:TPX>
								<ns2:Speed>7.206988</ns2:Speed>
								<ns2:Watts>188</ns2:Watts>
							</ns2:TPX>
						</Extensions>
					</Trackpoint>
				</Track>
			</Lap>
			<Notes/>
			<Training VirtualPartner="false">
				<Plan Type="Course" IntervalWorkout="false">
					<Name>filename</Name>
				</Plan>
			</Training>
		</Activity>
	</Activities>
	<Author xsi:type="Application_t">
		<Name>athlete</Name>
		<Build>
			<Version>
				<VersionMajor>0</VersionMajor>
				<VersionMinor>0</VersionMinor>
				<BuildMajor>0</BuildMajor>
				<BuildMinor>2</BuildMinor>
			</Version>
			<Builder>tacx.data.xml.tcx.TCXJAXBCreatorImpl</Builder>
		</Build>
		<LangID>en</LangID>
		<PartNumber>000-00000-00</PartNumber>
	</Author>
</TrainingCenterDatabase>
*/

QByteArray
TcxFileReader::toByteArray(const RideFile *ride, bool withAlt, bool withWatts, bool withHr, bool withCad) const
{
    QDomText text;
    QDomDocument doc;
    QDomProcessingInstruction hdr = doc.createProcessingInstruction("xml","version=\"1.0\"");
    doc.appendChild(hdr);

    // pwx
    QDomElement tcx = doc.createElementNS("http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2", "TrainingCenterDatabase");
    tcx.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    tcx.setAttribute("xsi:schemaLocation", "http://www.garmin.com/xmlschemas/ActivityExtension/v2 http://www.garmin.com/xmlschemas/ActivityExtensionv2.xsd http://www.garmin.com/xmlschemas/TrainingCenterDatabase/v2 http://www.garmin.com/xmlschemas/TrainingCenterDatabasev2.xsd");
    //tcx.setAttribute("version", "2.0");
    doc.appendChild(tcx);

    // activities, we just serialise one ride
    QString sport = ride->getTag("Sport", "Biking");
    if (sport == "Bike" || sport == tr("Bike") || sport == "Biking" || sport == "Cycling" || sport == "Cycle") {
        sport = "Biking";
    } else if (sport == "Run" || sport == tr("Run") || sport == "Running") {
        sport = "Running";
    } else {
        sport = "Other";
    }
    QDomElement activities = doc.createElement("Activities");
    tcx.appendChild(activities);
    QDomElement activity = doc.createElement("Activity");
    activity.setAttribute("Sport", sport); // was ride->getTag("Sport", "Biking") but must be Biking, Running or Other
    activities.appendChild(activity);

    // time
    QDomElement id = doc.createElement("Id");
    text = doc.createTextNode(ride->startTime().toUTC().toString(Qt::ISODate));
    id.appendChild(text);
    activity.appendChild(id);

    // notes if present
    if (ride->getTag("Notes","") != "") {
        QDomElement notes = doc.createElement("Notes");
        text = doc.createTextNode(ride->getTag("Notes",""));
        notes.appendChild(text);
        activity.appendChild(notes);
    }

    // device type if present
    if (ride->deviceType() != "") {
        QDomElement creator = doc.createElement("Creator");
        QDomElement creatorName = doc.createElement("Name");
        text = doc.createTextNode(ride->deviceType());
        creatorName.appendChild(text);
        creator.appendChild(creatorName);
        activity.appendChild(creator);
    }

    QDomElement lap = doc.createElement("Lap");
    lap.setAttribute("StartTime", ride->startTime().toUTC().toString(Qt::ISODate));
    activity.appendChild(lap);

    const char *metrics[] = {
        "total_distance",
        "workout_time",
        "total_work",
        "average_hr",
        "max_heartrate",
        "average_cad",
        "max_cadence",
        "average_power",
        "max_power",
        "max_speed",
        "average_speed",
        NULL
    };

    QStringList worklist = QStringList();
    for (int i=0; metrics[i];i++) worklist << metrics[i];

    if (context) { // can't do this standalone
        RideItem *tempItem = new RideItem(const_cast<RideFile*>(ride), context);
        QHash<QString,RideMetricPtr> computed = RideMetric::computeMetrics(tempItem, Specification(), worklist);

        QDomElement lap_time = doc.createElement("TotalTimeSeconds");
        text = doc.createTextNode(QString("%1").arg(computed.value("workout_time")->value(true)));
        //text = doc.createTextNode(ride->dataPoints().last()->secs);
        lap_time.appendChild(text);
        lap.appendChild(lap_time);

        QDomElement lap_distance = doc.createElement("DistanceMeters");
        text = doc.createTextNode(QString("%1").arg(1000*computed.value("total_distance")->value(true)));
        //text = doc.createTextNode(ride->dataPoints().last()->km);
        lap_distance.appendChild(text);
        lap.appendChild(lap_distance);

        QDomElement max_speed = doc.createElement("MaximumSpeed");
        text = doc.createTextNode(QString("%1")
            .arg(computed.value("max_speed")->value(true) / 3.6));
        max_speed.appendChild(text);
        lap.appendChild(max_speed);

        QDomElement lap_calories = doc.createElement("Calories");
        text = doc.createTextNode(QString("%1").arg((int)computed.value("total_work")->value(true)));
        lap_calories.appendChild(text);
        lap.appendChild(lap_calories);

        // optional per XSD, so only generate them if the data is to be exported and is present
        if (withHr && ride->areDataPresent()->hr)
        {
            QDomElement avg_heartrate = doc.createElement("AverageHeartRateBpm");
            QDomElement value = doc.createElement("Value");
            text = doc.createTextNode(QString("%1").arg((int)computed.value("average_hr")->value(true)));
            value.appendChild(text);
            avg_heartrate.appendChild(value);
            lap.appendChild(avg_heartrate);

            QDomElement max_heartrate = doc.createElement("MaximumHeartRateBpm");
            value = doc.createElement("Value");
            text = doc.createTextNode(QString("%1").arg((int)computed.value("max_heartrate")->value(true)));
            value.appendChild(text);
            max_heartrate.appendChild(value);
            lap.appendChild(max_heartrate);
        }

        QDomElement lap_intensity = doc.createElement("Intensity");
        text = doc.createTextNode("Active");
        lap_intensity.appendChild(text);
        lap.appendChild(lap_intensity);

        QDomElement lap_triggerMethod = doc.createElement("TriggerMethod");
        text = doc.createTextNode("Manual");
        lap_triggerMethod.appendChild(text);
        lap.appendChild(lap_triggerMethod);
    }

    // samples
    // data points: timeoffset, dist, hr, spd, pwr, torq, cad, lat, lon, alt
    if (!ride->dataPoints().empty()) {
        QDomElement track = doc.createElement("Track");
        lap.appendChild(track);

        foreach (const RideFilePoint *point, ride->dataPoints()) {
            QDomElement trackpoint = doc.createElement("Trackpoint");
            track.appendChild(trackpoint);

            // time
            QDomElement time = doc.createElement("Time");
            text = doc.createTextNode(ride->startTime().toUTC().addSecs(point->secs).toString(Qt::ISODate));
            time.appendChild(text);
            trackpoint.appendChild(time);

            // position
            if (ride->areDataPresent()->lat && point->lat > -90.0 && point->lat < 90.0 && point->lat != 0.0 &&
                ride->areDataPresent()->lon && point->lon > -180.00 && point->lon < 180.00 && point->lon != 0.0 ) {
                QDomElement position = doc.createElement("Position");
                trackpoint.appendChild(position);

                // lat
                QDomElement lat = doc.createElement("LatitudeDegrees");
                text = doc.createTextNode(QString("%1").arg(point->lat, 0, 'g', 11));
                lat.appendChild(text);
                position.appendChild(lat);

                // lon
                QDomElement lon = doc.createElement("LongitudeDegrees");
                text = doc.createTextNode(QString("%1").arg(point->lon, 0, 'g', 11));
                lon.appendChild(text);
                position.appendChild(lon);
            }


            // alt
            if (withAlt && ride->areDataPresent()->alt && point->alt != 0.0) {
                QDomElement alt = doc.createElement("AltitudeMeters");
                text = doc.createTextNode(QString("%1").arg(point->alt));
                alt.appendChild(text);
                trackpoint.appendChild(alt);
            }

            // distance - meters
            if (ride->areDataPresent()->km) {
                QDomElement dist = doc.createElement("DistanceMeters");
                text = doc.createTextNode(QString("%1").arg((point->km*1000)));
                dist.appendChild(text);
                trackpoint.appendChild(dist);
            }

            if (withHr && ride->areDataPresent()->hr)  {
                // HeartRate hack for Garmin Training Center
                // It needs an hr datapoint for every trackpoint or else the
                // hr graph in TC won't display. Schema defines the datapoint
                // as a positive int (> 0)

                int tHr = 1;
                if (ride->areDataPresent()->hr && point->hr >0.00) {
                    tHr = (int)point->hr;
                }
                QDomElement hr = doc.createElement("HeartRateBpm");
                hr.setAttribute("xsi:type", "HeartRateInBeatsPerMinute_t");
                QDomElement value = doc.createElement("Value");
                text = doc.createTextNode(QString("%1").arg(tHr));
                value.appendChild(text);
                hr.appendChild(value);
                trackpoint.appendChild(hr);
            }

            // cad
            if (withCad && ride->areDataPresent()->cad && point->cad < 255) { //xsd maxInclusive value="254"
                QDomElement cad = doc.createElement("Cadence");
                text = doc.createTextNode(QString("%1").arg((int)(point->cad)));
                cad.appendChild(text);
                trackpoint.appendChild(cad);
            }

            if (ride->areDataPresent()->kph || ride->areDataPresent()->watts) {
                QDomElement extension = doc.createElement("Extensions");
                trackpoint.appendChild(extension);
                QDomElement tpx = doc.createElement("TPX");
                tpx.setAttribute("xmlns", "http://www.garmin.com/xmlschemas/ActivityExtension/v2");
                extension.appendChild(tpx);

                // spd - meters per second
                if (ride->areDataPresent()->kph) {
                    QDomElement spd = doc.createElement("Speed");
                    text = doc.createTextNode(QString("%1").arg(point->kph / 3.6));
                    spd.appendChild(text);
                    tpx.appendChild(spd);
                }
                // pwr
                if (withWatts && ride->areDataPresent()->watts) {
                    QDomElement pwr = doc.createElement("Watts");
                    text = doc.createTextNode(QString("%1").arg((int)point->watts));
                    pwr.appendChild(text);
                    tpx.appendChild(pwr);
                }
            }
        }
    }

#if 0 // REFACTOR METRICS
    if (context) {
        // Activity Extensions
        QDomElement extensions = doc.createElement("Extensions");
        lap.appendChild(extensions);
        QDomElement lx = doc.createElement("LX");
        lx.setAttribute("xmlns", "http://www.garmin.com/xmlschemas/ActivityExtension/v2");
        extensions.appendChild(lx);

        QDomElement max_cad = doc.createElement("MaxBikeCadence");
        text = doc.createTextNode(QString("%1").arg(computed.value("max_cadence")->value(true)));
        max_cad.appendChild(text);
        lx.appendChild(max_cad);

        lx = doc.createElement("LX");
        lx.setAttribute("xmlns", "http://www.garmin.com/xmlschemas/ActivityExtension/v2");
        extensions.appendChild(lx);

        QDomElement avg_speed = doc.createElement("AvgSpeed");
        text = doc.createTextNode(QString("%1")
            .arg(computed.value("average_speed")->value(true) / 3.6));
        avg_speed.appendChild(text);
        lx.appendChild(avg_speed);

        lx = doc.createElement("LX");
        lx.setAttribute("xmlns", "http://www.garmin.com/xmlschemas/ActivityExtension/v2");
        extensions.appendChild(lx);

        QDomElement avg_power = doc.createElement("AvgWatts");
        text = doc.createTextNode(QString("%1").arg((int)computed.value("average_power")->value(true)));
        avg_power.appendChild(text);
        lx.appendChild(avg_power);

        lx = doc.createElement("LX");
        lx.setAttribute("xmlns", "http://www.garmin.com/xmlschemas/ActivityExtension/v2");
        extensions.appendChild(lx);

        QDomElement max_power = doc.createElement("MaxWatts");
        text = doc.createTextNode(QString("%1").arg((int)computed.value("max_power")->value(true)));
        max_power.appendChild(text);
        lx.appendChild(max_power);

        // Creator - Device
        QDomElement creator = doc.createElement("Creator");
        creator.setAttribute("xsi:type", "Device_t");
        activity.appendChild(creator);

        QDomElement creator_name = doc.createElement("Name");
        if (ride->deviceType() != "")
            text = doc.createTextNode(ride->deviceType());
        else
            text = doc.createTextNode("Unknown");
        creator_name.appendChild(text);
        creator.appendChild(creator_name);

        QDomElement creator_unitId = doc.createElement("UnitId");
        text = doc.createTextNode("0");
        creator_unitId.appendChild(text);
        creator.appendChild(creator_unitId);

        QDomElement creator_productId = doc.createElement("ProductID");
        text = doc.createTextNode("0");
        creator_productId.appendChild(text);
        creator.appendChild(creator_productId);

        QDomElement creator_version = doc.createElement("Version");
        creator.appendChild(creator_version);

        QDomElement creator_version_version_major = doc.createElement("VersionMajor");
        text = doc.createTextNode("3");
        creator_version_version_major.appendChild(text);
        creator_version.appendChild(creator_version_version_major);

        QDomElement creator_version_version_minor = doc.createElement("VersionMinor");
        text = doc.createTextNode("0");
        creator_version_version_minor.appendChild(text);
        creator_version.appendChild(creator_version_version_minor);

        QDomElement creator_version_build_major = doc.createElement("BuildMajor");
        text = doc.createTextNode("0");
        creator_version_build_major.appendChild(text);
        creator_version.appendChild(creator_version_build_major);

        QDomElement creator_version_build_minor = doc.createElement("BuildMinor");
        text = doc.createTextNode("0");
        creator_version_build_minor.appendChild(text);
        creator_version.appendChild(creator_version_build_minor);

        // Author - Application
        QDomElement author = doc.createElement("Author");
        author.setAttribute("xsi:type", "Application_t");
        tcx.appendChild(author);

        QDomElement author_name = doc.createElement("Name");
        text = doc.createTextNode("GoldenCheetah");
        author_name.appendChild(text);
        author.appendChild(author_name);

        QDomElement author_build = doc.createElement("Build");
        author.appendChild(author_build);

        QDomElement author_version = doc.createElement("Version");
        author_build.appendChild(author_version);

        QDomElement author_version_version_major = doc.createElement("VersionMajor");
        text = doc.createTextNode("3");
        author_version_version_major.appendChild(text);
        author_version.appendChild(author_version_version_major);

        QDomElement author_version_version_minor = doc.createElement("VersionMinor");
        text = doc.createTextNode("0");
        author_version_version_minor.appendChild(text);
        author_version.appendChild(author_version_version_minor);

        QDomElement author_version_build_major = doc.createElement("BuildMajor");
        text = doc.createTextNode("0");
        author_version_build_major.appendChild(text);
        author_version.appendChild(author_version_build_major);

        QDomElement author_version_build_minor = doc.createElement("BuildMinor");
        text = doc.createTextNode("0");
        author_version_build_minor.appendChild(text);
        author_version.appendChild(author_version_build_minor);

        QDomElement author_type = doc.createElement("Type");
        text = doc.createTextNode("Beta"); // was GC_VERSION but should be Interna | Alpha | Beta | Release
        author_type.appendChild(text);
        author_build.appendChild(author_type);

        QDomElement author_lang = doc.createElement("LangID");
        QVariant lang = appsettings->value(NULL, GC_LANG, QLocale::system().name());
        text = doc.createTextNode(lang.toString());
        author_lang.appendChild(text);
        author.appendChild(author_lang);

        QDomElement author_part_number = doc.createElement("PartNumber");
        text = doc.createTextNode("000-00000-00");
        author_part_number.appendChild(text);
        author.appendChild(author_part_number);
    }
#endif

    return doc.toByteArray(4);
}

bool
TcxFileReader::writeRideFile(Context *context, const RideFile *ride, QFile &file) const
{
    QByteArray xml = toByteArray(context, ride, true, true, true, true);

    if (!file.open(QIODevice::WriteOnly)) return(false);
    file.resize(0);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(true);
    out << xml;
    out.flush();
    file.close();
    return(true);
}

#endif
