#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QVector>
#include <QListView>
#include <QQuickView>
#include <QObject>
#include <QQuickWindow>
#include <QStandardPaths>

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
    QString docsPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "TrellCat Documents", QStandardPaths::LocateDirectory) + "/";// + "/TrellCat Documents/"; //"/home/riley/Documents/TrellCat Documents/";
    QString fileName = "Basic Template";
    QString extension = ".tcpro";
//    QString test = "." + QString::fromUtf8("\xF0\x9F\x90\xB1");
//    QString extension = ".🐱";
//    QString extension = "\u1F431";
    QFile file(docsPath + fileName + extension);

    // The CardModelVector object needs to have a reference to the file so it can read and write
    CardVector* cardVector = new CardVector(&file);

    CardModel* flatModel = new CardModel(cardVector);
    context->setContextProperty("gridModel", flatModel);
    flatModel->setCardFile(&file);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject* topLevel = engine.rootObjects().value(0);
    QQuickWindow* appWindow = qobject_cast<QQuickWindow*>(topLevel);
    QObject::connect(appWindow, SIGNAL(addChild(int)), flatModel, SLOT(addChild(int)));

    appWindow->setTitle(fileName + " - TrellCat");
    // update the width of the GridView
    QObject* gridView = appWindow->findChild<QObject*>("cardGrid");
    gridView->setProperty("width", (cardVector->getColumnCount() - 1) * 400);
    flatModel->setAppWindow(appWindow); // TODO: come up with a more elegant solution

    return app.exec();
}
