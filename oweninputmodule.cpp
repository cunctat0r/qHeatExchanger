#include "oweninputmodule.h"

OwenInputModule::OwenInputModule(QObject *parent) : QObject(parent)
{

}

OwenInputModule::~OwenInputModule()
{

}

void OwenInputModule::init(modbus_t *modbus_context, uint16_t slave)
{
    context = modbus_context;
    slave_id = slave;
    sensors[0] = 0.0;
    sensors[1] = 0.0;
    sensors[2] = 0.0;
    sensors[3] = 0.0;
}

void OwenInputModule::getInputs(uint16_t reg_number)
{
    int rc;
    uint16_t tab_reg[64];
    uint16_t src0[2];
    uint16_t src1[2];
    uint16_t src2[2];
    uint16_t src3[2];

    modbus_set_slave(this->context, this->slave_id);
    rc = modbus_read_registers(this->context, 0, reg_number, tab_reg);
    if (rc == -1) {
        rc = modbus_read_registers(this->context, 0, reg_number, tab_reg);
        if (rc == -1) {
      //      fprintf(stderr, "%s\n", modbus_strerror(errno));
            //return -1;
            //exit(1);
            //qDebug("Ошибка связи при чтении");
            return;
        }
    }
    src0[0] = tab_reg[5];
    src0[1] = tab_reg[4];
    this->sensors[0] = (modbus_get_float(src0) > 0) ? modbus_get_float(src0) : 0;

    src1[0] = tab_reg[11];
    src1[1] = tab_reg[10];
    this->sensors[1] = (modbus_get_float(src1) > 0) ? modbus_get_float(src1) : 0;

    src2[0] = tab_reg[17];
    src2[1] = tab_reg[16];
    this->sensors[2] = (modbus_get_float(src2) > 0) ? modbus_get_float(src2) : 0;    

    src3[0] = tab_reg[23];
    src3[1] = tab_reg[22];
    this->sensors[3] = (modbus_get_float(src3) > 0) ? modbus_get_float(src3) : 0;
    //qDebug("sensor 3 value: %f", this->sensors[3]);

}

float OwenInputModule::getSensor(uint16_t sensorNumber)
{
    return this->sensors[sensorNumber];
}

