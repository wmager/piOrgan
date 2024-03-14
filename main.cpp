#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <piorganInterface.h>
#include <QQmlContext>
#include <QtWidgets>
#include <iostream>
#include <cstdlib>

PiOrganInterface piorgan;  // A class containing my functions

bool locked = false;

int main(int argc, char *argv[])
{
    locked = true;
    piorgan.setEventsEnabled(false);

    const char* display = std::getenv("DISPLAY");
    if (display == nullptr) {
        std::cout << "Die Umgebungsvariable existiert nicht." << std::endl;
        if (setenv("DISPLAY", ":0.0", 1) != 0) {
            std::cerr << "Fehler beim Setzen der Umgebungsvariable." << std::endl;
            return 1;
        }
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/untitled6/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QSize screenSize = primaryScreen->size();
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();
    
    engine.rootContext()->setContextProperty("screenWidth",screenWidth);
    engine.rootContext()->setContextProperty("screenHeight",screenHeight);
    engine.rootContext()->setContextProperty("_myClass", &piorgan);


    engine.load(url);



    QObject *rootObject = engine.rootObjects().first();

    piorgan.setRootObject(rootObject);

    locked = false;
    piorgan.setEventsEnabled(true);

    return app.exec();
}


