#ifndef ALGORYTHM_H
#define ALGORYTHM_H

#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include <stdint.h>

class Algorythm : public QObject
{
    Q_OBJECT
public:
    explicit Algorythm(QObject *parent = 0);
    ~Algorythm();
    float getPressure(int numOfSensor);
    void init(int post_id);
    //int watchPump(int pumpNumber, int sensorNumber);
    bool pumpIsOn(int pumpNumber);
    bool pumpIsOff(int pumpNumber);
    void setEtap(int etap);


    QString getExchangerNumber() const;
    void setExchangerNumber(const QString &value);

signals:

public slots:
    void process();
    void setSensor0(float value);
    void setSensor1(float value);
    void setSensor2(float value);
    void setSensor3(float value);
    void setRelays(uint16_t mask);
    void watchPumpTimer();
    void etapCompleted(int etap);
    void startPressing();
    void setPostNumber(QString postNumber);

signals:
    void finished();
    void pumpOn(int pumpNumber);
    void pumpOff(int pumpNumber);
    void etapStarted(int etap);

private:
    int postNumber; // 1 -- левый, 2 -- правый
    float sensor[4];
    uint16_t relays;
    int etap;

    int pumpNumber;
    int sensorNumber;

    QTimer* ptimer;
    QString exchangerNumber;

};

#endif // ALGORYTHM_H
