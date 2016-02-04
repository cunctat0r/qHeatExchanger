#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    mask = 0;
    state = READ_INPUTS;
    modbus_t *ctx = 0;

    ctx = modbus_new_rtu("COM3", 9600, 'N', 8, 1);

    mb110_8a = new OwenInputModule();
    mb110_8a->init(ctx, 1);

    my110_16p = new output_module();
    my110_16p->init(ctx, 2);

    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    if (ctx == NULL) {
        //fprintf(stderr, "Unable to create the libmodbus context\n");
        //return -1;
        exit(1);
    }

    if (modbus_connect(ctx) == -1) {
        //fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        //return -1;
    }

    QTimer* ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(doWork()));
    ptimer->start(50);

}

Controller::~Controller()
{

}

void Controller::setOn(int pumpNumber)
{
    this->mask |= (1 << (pumpNumber - 1));
}

void Controller::setOff(int pumpNumber)
{
    this->mask &= (~(1 << (pumpNumber - 1)));
}


void Controller::doWork()
{
    //qDebug()<<"Sender: "<<QObject::sender();
    switch (state) {
        case READ_INPUTS:
            state = READ_OUTPUTS;
            //mb110_8a->getInputs(18);
            mb110_8a->getInputs(24);
            //QDebug(mb110_8a->getSensor(0));
            emit sensor0Changed(mb110_8a->getSensor(0));
            emit sensor1Changed(mb110_8a->getSensor(1));
            emit sensor2Changed(mb110_8a->getSensor(2));
            emit sensor3Changed(mb110_8a->getSensor(3));

            break;
        case READ_OUTPUTS:
            state = WRITE_REGISTER;
            my110_16p->getOutputs(50);
            emit maskChanged(my110_16p->getRelays());
            break;
        case WRITE_REGISTER:            
            state = READ_INPUTS;
            my110_16p->setOutputs(50, this->mask);
            break;
     }

}

