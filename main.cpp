#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QUrl>


#include "controlmodel.h"


int main(int argc, char *argv[])
{
    qmlRegisterType<cell>("cell", 1, 0, "Cell");
    qmlRegisterType<task>("task", 1, 0, "Task");
    qmlRegisterType<ControlModel>("controlmodel", 1, 0, "Control");

    //auto date = QDateTime::currentDateTime();

    QGuiApplication a(argc, argv);

    //sender.setSO(count.objectBrightness(count.sunAzimuthAndAltitude(date,53.9,27.56667,3,true),220));

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return a.exec();
}

