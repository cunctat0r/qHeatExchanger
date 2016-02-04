#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <Qthread>
#include <QDebug>
#include <QTimer>
#include <stdint.h>
#include "modbus.h"
#include "oweninputmodule.h"
#include "output_module.h"


class Controller : public QObject
{
    Q_OBJECT

    enum State {
        READ_INPUTS,
        READ_OUTPUTS,
        WRITE_REGISTER
    };

    State state;


public:
    explicit Controller(QObject *parent = 0);
    ~Controller();

    OwenInputModule *mb110_8a;
    output_module *my110_16p;


signals:
    void sensor0Changed(float value);
    void sensor1Changed(float value);
    void sensor2Changed(float value);
    void sensor3Changed(float value);
    void portReady();
    void maskChanged(uint16_t mask);


public slots:
    void doWork();
    void setOn(int pumpNumber);
    void setOff(int pumpNumber);

private:
    uint16_t mask;
};

#endif // CONTROLLER_H
