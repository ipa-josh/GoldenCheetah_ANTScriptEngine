#pragma once
#include <QtScript/QtScript>

class RegularEvent : public QObject
{
    Q_OBJECT
    
    QScriptEngine &engine;
    QTimer *timer;
    QString fileName;
    QString contents;
public:
    RegularEvent(QString filename, QScriptEngine &engine, const int interval) :
    engine(engine), fileName(filename)
    {
		QFile scriptFile(fileName);
		scriptFile.open(QIODevice::ReadOnly);
		QTextStream stream(&scriptFile);
		contents = stream.readAll();
		scriptFile.close();
		
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		timer->start(interval);
	}

public slots:
    void update() {		
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
	}
};
