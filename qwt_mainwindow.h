#ifndef QWT_MAINWINDOW_H
#define QWT_MAINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <QCoreApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QSpacerItem>
#include <QLineEdit>
#include <QFont>
#include <QThread>
#include <QProgressDialog>
#include <QTimer>
#include <QInputDialog>
#include <QDir>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprintdialog.h>
#include <QPainter>
#include <QDateTime>
#include <QRectF>
#include <QFont>
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include <qwt_legend.h>
#include <qwt_plot_renderer.h>

class Qwt_MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit Qwt_MainWindow(QWidget *parent = 0);
    ~Qwt_MainWindow();

public:
    QMessageBox *initMessageBox;


private:
    //QProgressBar *leftProgressBar;
    QwtPlot *leftPressurePlot, *rightPressurePlot;
    QwtLegend *leftPressurePlotLegend, *rightPressurePlotLegend;
    QPushButton *leftPressButton, *rightPressButton;
    QPushButton *leftStopButton, *rightStopButton;
    QPushButton *leftStartButton, *rightStartButton;
    QLineEdit *leftinPressureEntry, *leftExchangerPressureEntry;
    QLineEdit *rightinPressureEntry, *rightExchangerPressureEntry;
    QwtPlotCurve *leftInPressureCurve, *leftExchangerPressureCurve;
    QwtPlotCurve *rightInPressureCurve, *rightExchangerPressureCurve;
    QLabel *leftStatusLabel, *rightStatusLabel;
    int roundingPrecision;
    int leftTick, rightTick;
    QTimer *leftSecondsTimer, *rightSecondsTimer;
    int numTicks;
    double x[601], rightX[601];
    double leftInData[601], leftExData[601];
    double rightInData[601], rightExData[601];
    QString rightExchangerNumber, leftExchangerNumber;
    QString rightExchangerOperator, leftExchangerOperator;

signals:
    void leftPressButtonClicked();
    void rightPressButtonClicked();
    void leftStopButtonClicked(int pump_number);
    void rightStopButtonClicked(int pump_number);
    void completeEtap(int etap);
    void completeLeftEtap(int etap);
    void completeRightEtap(int etap);
    void startLeftPost();
    void startRightPost();
    void setLeftPostNumber(QString postNumber);
    void setRightPostNumber(QString postNumber);

public slots:
    void showLeftInPressure(float pressure);
    void showRightInPressure(float pressure);
    void showLeftExchangerPressure(float pressure);
    void showRightExchangerPressure(float pressure);
    void setLeftStatus();
    void setRightStatus();
    void stopLeftStatus();
    void stopRightStatus();
    void lpbClick();
    void rpbClick();
    void showEtapMessage(int etap);
    void showRightEtapMessage(int etap);

    //void nextTick();
    void leftNextTick();
    void rightNextTick();
};

#endif // QWT_MAINWINDOW_H
