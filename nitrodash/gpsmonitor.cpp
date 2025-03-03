#include "gpsmonitor.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QElapsedTimer>
#include "config.h"

GpsMonitor::GpsMonitor()
{
    fixState = 0; // 0 = searching
}

GpsMonitor::~GpsMonitor() {
}

void GpsMonitor::process()
{
    QSerialPort serial;
    serial.setPortName(UBLOXPORT);
    if(!serial.setBaudRate(QSerialPort::Baud9600))
        qDebug() << serial.errorString();
    if(!serial.setDataBits(QSerialPort::Data8))
        qDebug() << serial.errorString();
    if(!serial.setParity(QSerialPort::NoParity))
        qDebug() << serial.errorString();
    if(!serial.setFlowControl(QSerialPort::NoFlowControl))
        qDebug() << serial.errorString();
    if(!serial.setStopBits(QSerialPort::OneStop))
        qDebug() << serial.errorString();
    if(!serial.open(QIODevice::ReadOnly))
        qDebug() << serial.errorString();
    if(!serial.clear(QSerialPort::Input))
        qDebug() << serial.errorString();

    double lon = 0;
    double lat = 0;
    double timestamp = 0;
    while(serial.isOpen())
    {
        if (!serial.waitForReadyRead(-1))
            qDebug() << "error: " << serial.errorString();
        else {
            while (serial.canReadLine())
            {
                QByteArray datas = serial.readLine();
                if (datas.startsWith("$GNRMC"))
                {
                    //qDebug() << datas;
                    QList<QByteArray> fields = datas.split(',');
                    if (fields[2].data()[0] == 'A')
                    {
                        // lock acheived
                        timestamp = atof(fields[1].data());
                        lat = nmea2dec(fields[3].data(), 1, fields[4].data());
                        lon = nmea2dec(fields[5].data(), 2, fields[6].data());
                        emit gpsUpdate("GPS: Locked");
                        if ((fixState != 1) || (timer.elapsed() > 3600000))
                        // 3,600,000 milliseconds = 1 hour
                        {
                            // transition from search to fixed.
                            fixState = 1;
                            writeGpsFile(1, lat, lon, timestamp);

                            // start (or restart) a one hour timer
                            if (timer.isValid())
                            {
                                timer.restart();
                                //qDebug() << "timer.restart()";
                            }
                            else
                            {
                                timer.start();
                                //qDebug() << "timer.start()";
                            }
                        }
                        emit gpsUpdate("GPS: Locked");
                    }
                    else
                    {
                        // no lock
                        //qDebug() << "no lock.";
                        if (fixState != 0)
                        {
                            // transition from fixed to searching.
                            fixState = 0;
                            writeGpsFile(0, 0, 0, 0);
                            timer.invalidate();
                            //qDebug() << "timer.invalidate()";
                        }
                        emit gpsUpdate("GPS: Searching");
                    }
                }
            }
        }
    }
}

void GpsMonitor::writeGpsFile(unsigned int fix, double lat, double lon, double timestamp)
{
    //qDebug() << "writeGpsFile()";
    // gpsLocation.json in /root
    QFile file(GPSFILE);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << qSetRealNumberPrecision(8);
    out << "{\n";
    out << "   \"fix\": " << fix << ",\n";
    out << "   \"gpsLocation\":{\n";
    out << "     \"lat\":" << lat << ",\n";
    out << "     \"lon\":" << lon << "\n";
    out << "   }\n";
    out << "},\n";
    out << "\"timestamp\": " << timestamp;
    file.close();
}

/**
 * Convert latitude,longitude from nmea to decimal
 * @param type lat = 1, lon = 2
 */
double GpsMonitor::nmea2dec(char *nmea, char type, char *dir)
{
    unsigned int idx, dot = 0;
    double dec = 0;
    for (idx=0; idx<strlen(nmea);idx++){
        if (nmea[idx] == '.'){
            dot = idx;
            break;
        }
    }

    if (dot < 3)
        return 0;

    int dd;
    double mm;
    char cdd[5], cmm[10];
    memset(&cdd, 0, 5);
    memset(&cmm, 0, 10);
    strncpy(cdd, nmea, dot-2);
    strcpy(cmm, nmea+dot-2);
    dd = atoi(cdd);
    mm = atof(cmm);

    dec = dd + (mm/60);

    if (type == 1 && dec > MAX_LATITUDE)
    {
        qDebug() << "error: exceeded MAX_LATITUDE";
        return 0;
    }
    else if (type == 2 && dec > MAX_LONGITUDE)
    {
        qDebug() << "error: exceeded MAX_LONGITUDE";
        return 0;
    }
    if ((dir[0] == 'S') || (dir[0] == 'W'))
        dec = -1 * dec;

    return dec;
}

