#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include "algorythm.h"

Algorythm::Algorythm(QObject *parent) : QObject(parent)
{
    //this->postNumber = post_id;
    this->sensor[0] = 0;
    this->sensor[1] = 0;
    this->sensor[2] = 0;
    this->sensor[3] = 0;
    this->etap = 0;

}

Algorythm::~Algorythm()
{

}

float Algorythm::getPressure(int numOfSensor)
{
    //qDebug("Getting pressure...");
    return this->sensor[numOfSensor];
}

void Algorythm::init(int post_id)
{
    this->postNumber = post_id;
}


bool Algorythm::pumpIsOn(int pumpNumber)
{
    uint16_t init_value = this->relays;
    int bit_number = pumpNumber - 1;
    int result = (init_value & (1 << bit_number)) >> bit_number;
    return (result == 0) ? false : true;
}

bool Algorythm::pumpIsOff(int pumpNumber)
{
    bool test;
    //qDebug("checking pump is off...");
    test = !(this->pumpIsOn(pumpNumber));
    return test;
}

void Algorythm::setEtap(int etap)
{
    this->etap = etap;
}

void Algorythm::process()
{    
    switch (this->postNumber) {
    case 1:
        this->etap = 1;
        this->pumpNumber = 2;
        this->sensorNumber = 1;
        break;
    case 2:
        this->etap = 1;
        this->pumpNumber = 3;
        this->sensorNumber = 2;
        break;
    case 3:
        this->pumpNumber = 1;
        this->sensorNumber = 0;
        break;
    case 4:
        this->pumpNumber = 4;
        this->sensorNumber = 3;
        break;
    default:
        break;
    }
    ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(watchPumpTimer()));
    ptimer->start(100);
}

void Algorythm::setSensor0(float value)
{
    this->sensor[0] = value;
}

void Algorythm::setSensor1(float value)
{
    this->sensor[1] = value;
}

void Algorythm::setSensor2(float value)
{
    this->sensor[2] = value;
}

void Algorythm::setSensor3(float value)
{
    this->sensor[3] = value;
}

void Algorythm::setRelays(uint16_t mask)
{
    this->relays = mask;
}


void Algorythm::watchPumpTimer()
{
    switch (this->pumpNumber) {
    case 1:
        // нагнетающий насос
        if (this->getPressure(this->sensorNumber) < 6.2) {
            if (this->pumpIsOff(this->pumpNumber)) {
                emit pumpOn(this->pumpNumber);
                //continue;
            }
        }
        if (this->getPressure(this->sensorNumber) > 6.7) {
            if (this->pumpIsOn(this->pumpNumber)) {
                emit pumpOff(this->pumpNumber);
                //continue;
            }
        }
        break;
    case 2:
    case 3:
        // циркуляционные насосы

        switch (this->etap) {
        case 0:
            // начало, ничего не делать
            //qDebug("Etap is %d", this->etap);
            break;
        case 1:
            //qDebug("Etap is %d", this->etap);
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 2:
            //qDebug("Etap is %d, post id %d", this->etap, this->postNumber);
            if (this->getPressure(this->sensorNumber) >= 1) {
                //if (this->pumpIsOff(this->pumpNumber)) {
                //    emit pumpOn(this->pumpNumber);
                    this->etap = 3;
                    //continue;
                //} else {
                    //qDebug("Pump is on...");
                //}
            }
            break;
        case 3:
            //qDebug("Etap is %d", this->etap);
            // Показать сообщение "После сброса воздуха закройте кран сброса"
            if (this->getPressure(sensorNumber) > 6.5) {
                if (this->ptimer->isActive()) {
                    this->ptimer->stop();
                    emit etapStarted(this->etap);
                }
            }
            break;
        case 4:
            //qDebug("Etap is %d", this->etap);
            QThread::msleep(10000);
            //emit etapStarted(this->etap);
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 5:
            //qDebug("Etap is %d", this->etap);
            break;
        case 6:
            //qDebug("Etap is %d", this->etap);
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 61:
            //qDebug("Etap is %d", this->etap);
            QThread::msleep(10000);
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 7:
            //qDebug("Etap is %d", this->etap);
            //if (this->pumpIsOn(this->pumpNumber)) {
            //    emit pumpOff(this->pumpNumber);
                //this->etap = 3;
                //continue;
            //}
            if (this->pumpIsOff(4)) {
                if (this->ptimer->isActive()) {
                    this->ptimer->stop();
                    emit etapStarted(this->etap);
                }
            }
            break;
        case 8:
            //qDebug("Etap is %d", this->etap);
            if (this->pumpIsOff(4)) {
                if (this->ptimer->isActive()) {
                    this->ptimer->stop();
                    emit etapStarted(this->etap);
                }
                //emit pumpOff(this->pumpNumber);
                //this->etap = 3;
                //continue;
            }

            break;
        case 9:
            //qDebug("Etap is %d", this->etap);
            //if (this->pumpIsOn(this->pumpNumber)) {
            //    emit pumpOff(this->pumpNumber);
                //this->etap = 3;
                //continue;
            //}
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 10:
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 101:
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        case 11:
            if (this->ptimer->isActive()) {
                this->ptimer->stop();
                emit etapStarted(this->etap);
            }
            break;
        }

    case 4:
        // насос откачки из котла
        // проверять будем сухие контакты
        //qDebug("Dry contact: %d", qRound(this->getPressure(this->sensorNumber)));
        if (qRound(this->getPressure(this->sensorNumber)) == 2) {
            if (this->pumpIsOff(this->pumpNumber)) {
                emit pumpOn(this->pumpNumber);
                //continue;
            }
        }
        if (qRound(this->getPressure(this->sensorNumber)) == 1) {
            //qDebug("Dry contact: %d", qRound(this->getPressure(this->sensorNumber)));
            if (this->pumpIsOn(this->pumpNumber)) {
                QThread::msleep(3000);
                //qDebug("Dry contact after sleep: %d", qRound(this->getPressure(this->sensorNumber)));
                if (qRound(this->getPressure(this->sensorNumber)) == 1) {
                    emit pumpOff(this->pumpNumber);
                }
                //continue;
            }
        }
        break;
    default:
        break;
    }
}

void Algorythm::etapCompleted(int etap)
{
    switch (etap) {
    case 0:
        break;
    case 1:
        if (! this->ptimer->isActive()) {
            this->etap = 2;            
            this->ptimer->start();
        }
        break;
    case 2:
        if (! this->ptimer->isActive()) {
            this->etap = 3;
            this->ptimer->start();
        } else {
           this->etap = 3;
        }
        break;
    case 3:
        if (! this->ptimer->isActive()) {
            this->etap = 4;
            this->ptimer->start();
        }
    case 4:
        //this->etap = 5;
        if (! this->ptimer->isActive()) {
            this->etap = 5;
            this->ptimer->start();
        }
        break;
    case 5:
        if (! this->ptimer->isActive()) {
           this->etap = 6;
            this->ptimer->start();
        } else {
            this->etap = 6;
        }
        break;
    case 6:
        if (! this->ptimer->isActive()) {
            //this->etap = 7;
            // // this->etap = 8;
            this->etap = 61;
            this->ptimer->start();
        }
        break;
    case 61:
        if (! this->ptimer->isActive()) {
            //this->etap = 7;
            // // this->etap = 8;
            this->etap = 8;
            this->ptimer->start();
        }
        break;
    case 7:
        if (! this->ptimer->isActive()) {
            this->etap = 8;
            this->ptimer->start();
        }
        break;
    case 8:
        if (! this->ptimer->isActive()) {
            this->etap = 9;
            this->ptimer->start();
        }
        break;
    case 9:
        if (! this->ptimer->isActive()) {
            this->etap = 10;
            this->ptimer->start();
        }
        break;
    case 10:
        if (! this->ptimer->isActive()) {
            this->etap = 101;
            this->ptimer->start();
        }
        break;
    case 101:
        if (! this->ptimer->isActive()) {
            this->etap = 11;
            this->ptimer->start();
        }
        break;
    case 11:
        if (! this->ptimer->isActive()) {
            //qDebug("Запуск печати ветка 1");
            this->etap = 0;
            this->ptimer->start();
        } else {
            //qDebug("Запуск печати ветка 2");
            this->etap = 0;
        }
        break;
    }
}

void Algorythm::startPressing()
{
    // нажата кнопка "Опрессовка"
    //qDebug("Нажата кнопка Опрессовка");
    this->etap = 5;
    emit etapStarted(5);
}

void Algorythm::setPostNumber(QString postNumber)
{
    this->setExchangerNumber(postNumber);
}

QString Algorythm::getExchangerNumber() const
{
    return exchangerNumber;
}

void Algorythm::setExchangerNumber(const QString &value)
{
    exchangerNumber = value;
}


