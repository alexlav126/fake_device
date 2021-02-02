#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVariant>
#include <QQuickWindow>

#include "channelhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject *root = engine.rootObjects()[0];
    QObject *mainLayoutObj = root->findChild<QObject*>("mainLayout");
    QQuickItem *mainLayout = qobject_cast<QQuickItem*>(mainLayoutObj);

    QQmlComponent component(&engine,
                            QUrl("qrc:/channel.qml"),
                            QQmlComponent::PreferSynchronous);

    const int NUM_CHANNELS = 2;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        auto *view = qobject_cast<QQuickItem*>(component.create());
        // To avoid the Javascript garbage collector to kill it,
        // tell QML that C++ takes care of it:
        QQmlEngine::setObjectOwnership(view, QQmlEngine::CppOwnership);
        view->setParentItem(mainLayout);
        view->setParent(&engine);

        auto channel = new ChannelHandler(i, view, root);
        if(channel == nullptr) return -1;
    }




    return app.exec();
}
