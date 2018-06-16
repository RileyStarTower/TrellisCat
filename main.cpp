#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>

//TODO: remove
#include <QtDebug>

#include <QVector>
#include <QListView>
#include <QQuickView>

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
    QFile file("/home/riley/Documents/Writing Tool Sample XML/Generic Sample.txt");

    // The CardModelVector object needs to have a reference to the file so it can read and write
    CardVector* cardVector = new CardVector(&file);

    CardModel* flatModel = new CardModel(cardVector);
    context->setContextProperty("gridModel", flatModel);
    flatModel->setCardFile(&file);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
