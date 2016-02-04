#include <stdint.h>
#include <QThread>
#include <QtDebug>
#include "modbus.h"
#include "output_module.h"




output_module::output_module(QObject *parent) : QObject(parent)
{

}

output_module::~output_module()
{
}

void output_module::init(modbus_t *modbus_context, uint16_t slave)
{
    context = modbus_context;
    slave_id = slave;
    relays = 0;
}

void output_module::getOutputs(uint16_t reg_number) {
	/* Получить значения выходов модуля */
    int rc = 0;
    uint16_t tab_reg[1];
	tab_reg[0] = 0;

    modbus_set_slave(this->context, this->slave_id);
    rc = modbus_read_registers(this->context, reg_number, 1, tab_reg);
	if (rc == -1) {
        rc = modbus_read_registers(this->context, reg_number, 1, tab_reg);
		if (rc == -1) {
//			fprintf(stderr, "Error getting outputs\n");
//			fprintf(stderr, "%s\n", modbus_strerror(errno));
		}
	}

    this->relays = tab_reg[0];       
}

void output_module::setOutputs(uint16_t reg_number, uint16_t mask)
{
    uint16_t init_value[1];
    int rc = 0;

    init_value[0] = mask;
    modbus_set_slave(this->context, this->slave_id);
    //init_value[0] |= (1 << bit_number);
    rc = modbus_write_registers(context, reg_number, 1, init_value);
    if (rc == -1) {
        rc = modbus_write_registers(context, reg_number, 1, init_value);
        if (rc == -1) {
        //exit(1);
            qDebug("Ошибка связи при записи");
            return;
        }
    }
}

void output_module::setOn(uint16_t reg_number, uint16_t bit_number) {
    uint16_t init_value[1];
    int rc = 0;

    init_value[0] = this->relays;
    modbus_set_slave(this->context, this->slave_id);
    init_value[0] |= (1 << bit_number);
    rc = modbus_write_registers(context, reg_number, 1, init_value);
    if (rc == -1) {
        //exit(1);
    }
}

bool output_module::isOn(uint16_t reg_number, uint16_t bit_number)
{
    uint16_t init_value[1];
    uint16_t result;

    init_value[0] = 0;
    result = 0;

    this->getOutputs(reg_number);
    //QThread::msleep(25);

    init_value[0] = this->relays;
    result = (init_value[0] & (1 << bit_number)) >> bit_number;
    return (result == 0) ? false : true;
}

void output_module::setOff(uint16_t reg_number, uint16_t bit_number)
{
    uint16_t init_value[1];
    int rc = 0;

    init_value[0] = this->relays;
    modbus_set_slave(this->context, this->slave_id);
    init_value[0] &= (~(1 << bit_number));
    rc = modbus_write_registers(context, reg_number, 1, init_value);
    if (rc == -1) {
        //exit(1);
    }
}

bool output_module::isOff(uint16_t reg_number, uint16_t bit_number)
{
    return !(this->isOn(reg_number, bit_number));
}

uint16_t output_module::getRelays()
{
    return this->relays;
}

