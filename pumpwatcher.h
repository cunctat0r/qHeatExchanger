#ifndef PUMPWATCHER_H
#define PUMPWATCHER_H

#include <QObject>

class PumpWatcher : public QObject
{
    Q_OBJECT
public:
    explicit PumpWatcher(QObject *parent = 0);
    ~PumpWatcher();

signals:

public slots:
};

#endif // PUMPWATCHER_H
