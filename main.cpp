#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QQuickWindow>

#include "imageproc.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setOrganizationName("Piyush");
    app.setOrganizationDomain("Aggarwal");

    /* create custom QObjects before engine so engine can destroy correctly
     */

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [&](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qDebug() << "couldn't load object!";
            QCoreApplication::exit(-1);
        } else {
            qDebug() << "QML Loaded sucessfully! Starting application now!";
        }
    }, Qt::QueuedConnection);

    /* add custom QObjects to the QML as property before loading the QML
     */
    engine.rootContext()->setContextProperty("imageProc", ImageProc::getInstance());

    qmlRegisterUncreatableMetaObject(
                ImageProcUtil::staticMetaObject,          // static meta object
                "ImageProc.ImageProcUtil",                // import statement (can be any string)
                1, 0,                                     // major and minor version of the import
                "ImageProcUtil",                          // name in QML (does not have to match C++ name)
                "Error: only enums"                       // error in case someone tries to create a ImageProcUtil object
                );

    QQuickWindow::setTextRenderType(QQuickWindow::QtTextRendering);

    engine.load(url);

    return app.exec();
}
