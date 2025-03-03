#ifndef GPSMONITOR_H
#define GPSMONITOR_H

#include <QObject>
#include <QElapsedTimer>
#include "config.h"

#define MAX_LONGITUDE    180
#define MAX_LATITUDE     90
#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(6)

class GpsMonitor : public QObject
{
    Q_OBJECT
public:
    GpsMonitor();
    ~GpsMonitor();
    double nmea2dec(char *nmea, char type, char *dir);
    void writeGpsFile(unsigned int fix, double lat, double lon, double timestamp);

private:
    int fixState;
    QElapsedTimer timer;

protected:

signals:
    void finished();
    void gpsUpdate(QString const &gpslocation);

public slots:
    void process();

};

#endif // GPSMONITOR_H
