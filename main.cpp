#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tablemodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("Gomsil LTD");
    app.setOrganizationDomain("gomsil.eu");
    app.setApplicationName("QGomsil");

    qmlRegisterType<TableModel>("TableModel", 1, 0, "TableModel");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
