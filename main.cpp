#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QVector>
#include <QListView>
#include <QQuickView>
#include <QObject>
#include <QQuickWindow>

//TODO: remove
#include <QtDebug>

#include "cardmodel.h"
#include "cardvector.h"
#include "spacercard.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();

    // TODO: add file selection
    QFile file("/home/riley/Documents/Writing Tool Sample XML/Generic Sample (copy 1).🐱");

    // The CardModelVector object needs to have a reference to the file so it can read and write
    CardVector* cardVector = new CardVector(&file);

    CardModel* flatModel = new CardModel(cardVector);
    context->setContextProperty("gridModel", flatModel);
    flatModel->setCardFile(&file);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject* topLevel = engine.rootObjects().value(0);
    QQuickWindow* appWindow = qobject_cast<QQuickWindow*>(topLevel);
    QObject::connect(appWindow, SIGNAL(addChild(int)), flatModel, SLOT(addChild(int)));

    return app.exec();
}
