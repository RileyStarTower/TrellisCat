#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QVector>
#include <QObject>
#include <QQuickWindow>
#include <QStandardPaths>

#include <QCoreApplication>
#include <QString>

//TODO: remove
#include <QtDebug>

#include "cardmodel.h"
#include "spacercard.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    qmlRegisterType<CardModel>("com.startowerstudio.cardmodel", 1, 0, "CardModel");

    // TODO: add file selection
    QString docsPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "TrellCat Documents", QStandardPaths::LocateDirectory) + "/";// + "/TrellCat Documents/"; //"/home/riley/Documents/TrellCat Documents/";
    QString fileName = "Test";
    QString extension = ".🐱";
    QFile file(docsPath + fileName + extension);

    CardModel* flatModel = new CardModel(&file);
//    context->setContextProperty("gridModel", flatModel);
//    flatModel->setCardFile(&file);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject* topLevel = engine.rootObjects().value(0);
    QQuickWindow* appWindow = qobject_cast<QQuickWindow*>(topLevel);
    QObject::connect(appWindow, SIGNAL(addChild(int)), flatModel, SLOT(addChild(int)));

    appWindow->setTitle(fileName + " - TrellCat");

    // update the width of the GridView
//    QObject* gridView = appWindow->findChild<QObject*>("cardGrid");
//    gridView->setProperty("width", (flatModel->getTreeDepth()) * 400);
    flatModel->setAppWindow(appWindow); // TODO: come up with a more elegant solution

    return app.exec();
}
