#ifndef OUTPUTMODULE_H
#define OUTPUTMODULE_H

#include <QObject>
#include <QDebug>
#include <stdint.h>
#include "modbus.h"


class output_module: public QObject
{
    Q_OBJECT
public:
    //output_module();
    //output_module(modbus_t *modbus_context, uint16_t slave);
    explicit output_module(QObject *parent = 0);
	~output_module();

    void init(modbus_t *modbus_context, uint16_t slave);
    void getOutputs(uint16_t reg_number);
    void setOutputs(uint16_t reg_number, uint16_t mask);
    void setOn(uint16_t reg_number, uint16_t bit_number);
    bool isOn(uint16_t reg_number, uint16_t bit_number);
    void setOff(uint16_t reg_number, uint16_t bit_number);
    bool isOff(uint16_t reg_number, uint16_t bit_number);
    uint16_t getRelays();

signals:
    void ready();

private:
    uint16_t slave_id;
    modbus_t *context;
    uint16_t relays;


};
#endif
