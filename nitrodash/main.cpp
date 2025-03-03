#include "homescreen.h"
#include "infoscreen.h"
#include "packetsscreen.h"
#include "wpactrlifc.h"
#include "gpsmonitor.h"
#include <QApplication>
#include <QThread>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;

    WpaCtrlIfc* wpaCtrlIfc = new WpaCtrlIfc();
    QThread wpaCtrlIfcThread;

    GpsMonitor* gpsMonitor = new GpsMonitor();
    QThread gpsMonitorThread;

    QObject::connect(wpaCtrlIfc, &WpaCtrlIfc::scanUpdate, &w, &HomeScreen::onScanUpdate);
    QObject::connect(wpaCtrlIfc, &WpaCtrlIfc::statusUpdate, &w, &HomeScreen::onStatusUpdate);
    QObject::connect(&w, &HomeScreen::requestStatus, wpaCtrlIfc, &WpaCtrlIfc::onRequestStatus);
    QObject::connect(&w, &HomeScreen::requestScan, wpaCtrlIfc, &WpaCtrlIfc::onRequestScan);
    QObject::connect(&w, &HomeScreen::requestConnect, wpaCtrlIfc, &WpaCtrlIfc::onRequestConnect);

    wpaCtrlIfc->moveToThread(&wpaCtrlIfcThread);
    wpaCtrlIfcThread.start();

    QObject::connect(&gpsMonitorThread, SIGNAL(started()), gpsMonitor, SLOT(process()));
    QObject::connect(gpsMonitor, &GpsMonitor::gpsUpdate, &w, &HomeScreen::onGpsUpdate);

    gpsMonitor->moveToThread(&gpsMonitorThread);
    gpsMonitorThread.start();

    w.show();
    return a.exec();
}
