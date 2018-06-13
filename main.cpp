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
#include "cardmodelvector.h"
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
    CardModelVector* cardModelVector = new CardModelVector(&file);

    // Get a subset of the models to display
    CardModel* model_1 = cardModelVector->getCardModel(1);
    CardModel* model_2 = cardModelVector->getCardModel(2);
    CardModel* model_3 = cardModelVector->getCardModel(3);

    // connect the models to the editingFinished signal


    // Link the models
    context->setContextProperty("cardModel_1", model_1);
    context->setContextProperty("cardModel_2", model_2);
    context->setContextProperty("cardModel_3", model_3);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
