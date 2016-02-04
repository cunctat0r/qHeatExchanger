#include <stdint.h>
#include "mainwindow.h"
#include "qwt_mainwindow.h"
#include "algorythm.h"
#include "controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qwt_MainWindow *wnd = new Qwt_MainWindow;
    wnd->setWindowTitle("Опрессовка теплообменников");
    wnd->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    wnd->show();

    QThread* controllerThread = new QThread;
    Controller* controller = new Controller();
    controller->moveToThread(controllerThread);

    QObject::connect(controllerThread, SIGNAL(started()), controller, SLOT(doWork()));
    QObject::connect(controller, SIGNAL(sensor0Changed(float)), wnd, SLOT(showLeftInPressure(float)));
    QObject::connect(controller, SIGNAL(sensor0Changed(float)), wnd, SLOT(showRightInPressure(float)));
    QObject::connect(controller, SIGNAL(sensor1Changed(float)), wnd, SLOT(showLeftExchangerPressure(float)));
    QObject::connect(controller, SIGNAL(sensor2Changed(float)), wnd, SLOT(showRightExchangerPressure(float)));

    QThread* leftPostThread = new QThread;
    QThread* rightPostThread = new QThread;
    Algorythm *leftPost = new Algorythm(); leftPost->init(1);
    Algorythm *rightPost = new Algorythm(); rightPost->init(2);
    leftPost->moveToThread(leftPostThread);
    rightPost->moveToThread(rightPostThread);

    QThread* mainPumpThread = new QThread;
    Algorythm *mainPumpWatcher = new Algorythm(); mainPumpWatcher->init(3);
    mainPumpWatcher->moveToThread(mainPumpThread);

    QThread* boilerPumpThread = new QThread;
    Algorythm *boilerPumpWatcher = new Algorythm(); boilerPumpWatcher->init(4);
    boilerPumpWatcher->moveToThread(boilerPumpThread);

    //QObject::connect(wnd, SIGNAL(rightPressButtonClicked()), rightPost, SLOT(process()));
    //QObject::connect(wnd, SIGNAL(leftPressButtonClicked()), leftPost, SLOT(process()));
    QObject::connect(wnd, SIGNAL(startLeftPost()), leftPost, SLOT(process()));
    QObject::connect(wnd, SIGNAL(startRightPost()), rightPost, SLOT(process()));

    QObject::connect(wnd, SIGNAL(rightPressButtonClicked()), rightPost, SLOT(startPressing()), Qt::DirectConnection);
    QObject::connect(wnd, SIGNAL(leftPressButtonClicked()), leftPost, SLOT(startPressing()), Qt::DirectConnection);

    QObject::connect(mainPumpThread, SIGNAL(started()), mainPumpWatcher, SLOT(process()));
    QObject::connect(boilerPumpThread, SIGNAL(started()), boilerPumpWatcher, SLOT(process()));


    QObject::connect(leftPost, SIGNAL(pumpOn(int)), controller, SLOT(setOn(int)), Qt::DirectConnection);
    QObject::connect(leftPost, SIGNAL(pumpOff(int)), controller, SLOT(setOff(int)), Qt::DirectConnection);
    QObject::connect(rightPost, SIGNAL(pumpOn(int)), controller, SLOT(setOn(int)), Qt::DirectConnection);
    QObject::connect(rightPost, SIGNAL(pumpOff(int)), controller, SLOT(setOff(int)), Qt::DirectConnection);
    QObject::connect(mainPumpWatcher, SIGNAL(pumpOn(int)), controller, SLOT(setOn(int)), Qt::DirectConnection);
    QObject::connect(mainPumpWatcher, SIGNAL(pumpOff(int)), controller, SLOT(setOff(int)), Qt::DirectConnection);
    QObject::connect(boilerPumpWatcher, SIGNAL(pumpOn(int)), controller, SLOT(setOn(int)), Qt::DirectConnection);
    QObject::connect(boilerPumpWatcher, SIGNAL(pumpOff(int)), controller, SLOT(setOff(int)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor0Changed(float)), rightPost, SLOT(setSensor0(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor1Changed(float)), rightPost, SLOT(setSensor1(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor2Changed(float)), rightPost, SLOT(setSensor2(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor3Changed(float)), rightPost, SLOT(setSensor3(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor0Changed(float)), leftPost, SLOT(setSensor0(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor1Changed(float)), leftPost, SLOT(setSensor1(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor2Changed(float)), leftPost, SLOT(setSensor2(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor3Changed(float)), leftPost, SLOT(setSensor3(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor0Changed(float)), mainPumpWatcher, SLOT(setSensor0(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor1Changed(float)), mainPumpWatcher, SLOT(setSensor1(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor2Changed(float)), mainPumpWatcher, SLOT(setSensor2(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor3Changed(float)), mainPumpWatcher, SLOT(setSensor3(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor0Changed(float)), boilerPumpWatcher, SLOT(setSensor0(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor1Changed(float)), boilerPumpWatcher, SLOT(setSensor1(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor2Changed(float)), boilerPumpWatcher, SLOT(setSensor2(float)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(sensor3Changed(float)), boilerPumpWatcher, SLOT(setSensor3(float)), Qt::DirectConnection);


    QObject::connect(controller, SIGNAL(maskChanged(uint16_t)), leftPost, SLOT(setRelays(uint16_t)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(maskChanged(uint16_t)), rightPost, SLOT(setRelays(uint16_t)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(maskChanged(uint16_t)), mainPumpWatcher, SLOT(setRelays(uint16_t)), Qt::DirectConnection);
    QObject::connect(controller, SIGNAL(maskChanged(uint16_t)), boilerPumpWatcher, SLOT(setRelays(uint16_t)), Qt::DirectConnection);

    QObject::connect(leftPost, SIGNAL(etapStarted(int)), wnd, SLOT(showEtapMessage(int)));
    QObject::connect(wnd, SIGNAL(completeEtap(int)), leftPost, SLOT(etapCompleted(int)));
    QObject::connect(rightPost, SIGNAL(etapStarted(int)), wnd, SLOT(showRightEtapMessage(int)));
    QObject::connect(wnd, SIGNAL(completeRightEtap(int)), rightPost, SLOT(etapCompleted(int)));


    leftPostThread->start();
    rightPostThread->start();
    mainPumpThread->start();
    boilerPumpThread->start();
    controllerThread->start();


    return a.exec();
}
