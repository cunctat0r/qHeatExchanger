#ifndef OWENINPUTMODULE_H
#define OWENINPUTMODULE_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <stdint.h>
#include "modbus.h"

class OwenInputModule : public QObject
{
    Q_OBJECT
public:
    explicit OwenInputModule(QObject *parent = 0);
    ~OwenInputModule();

    void init(modbus_t *modbus_context, uint16_t slave);
    void getInputs(uint16_t reg_number);
    float getSensor(uint16_t sensorNumber);

signals:
    void input_ready();

public slots:

private:
    uint16_t* inputs;
    uint16_t slave_id;
    modbus_t *context;
    float sensors[4];
};

#endif // OWENINPUTMODULE_H
