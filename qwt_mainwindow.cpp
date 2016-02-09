#include "qwt_mainwindow.h"

Qwt_MainWindow::Qwt_MainWindow(QWidget *parent) : QWidget(parent)
{


    leftSecondsTimer = new QTimer(this);
    connect(leftSecondsTimer, SIGNAL(timeout()), SLOT(leftNextTick()));
    rightSecondsTimer = new QTimer(this);
    connect(rightSecondsTimer, SIGNAL(timeout()), SLOT(rightNextTick()));

    roundingPrecision = 2;
    leftTick = 0;
    rightTick = 0;
    numTicks = 600;

    for (int i = 0; i <= 600; i++) {
        x[i] = 0;
        rightX[i] = 0;
        leftInData[i] = 0.0;
        leftExData[i] = 0.0;
        rightInData[i] = 0.0;
        rightExData[i] = 0.0;
    }

    this->setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "./img/gauge.ico"));
    this->setStyleSheet("QLabel, QPushButton, QLineEdit { font: bold 16px;}");
    // Графики давления в теплообменниках
    // левый
    leftPressurePlot = new QwtPlot;
    leftPressurePlotLegend = new QwtLegend;
    leftPressurePlot->setTitle("Опрессовка теплообменника");
    leftPressurePlot->insertLegend(leftPressurePlotLegend, QwtPlot::RightLegend, 1.0);
    //leftPressurePlot->setAxisAutoScale(QwtPlot::yLeft);
    leftPressurePlot->setAxisScale(QwtPlot::yLeft, 0, 10);
    leftPressurePlot->setAxisScale(QwtPlot::xBottom, 0, 600);
    leftPressurePlot->setAxisTitle( QwtPlot::xBottom, " Время опрессовки, сек" );
    leftPressurePlot->setAxisTitle( QwtPlot::yLeft, " Давление, кгс" );
    leftPressurePlot->setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom );
    leftPressurePlot->setAxisLabelAlignment( QwtPlot::yLeft, Qt::AlignVCenter);

    //QPen leftInPressurePen = QPen(Qt::red);
    QPen leftExchangerPressurePen = QPen(Qt::blue);

    //leftInPressureCurve = new QwtPlotCurve("Входное давление");
    //leftInPressureCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    //leftInPressureCurve->setPen(leftInPressurePen);
    //leftInPressureCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    //leftInPressureCurve->attach(leftPressurePlot);

    leftExchangerPressureCurve = new QwtPlotCurve("Давление в теплообменнике");
    leftExchangerPressureCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    leftExchangerPressureCurve->setPen(leftExchangerPressurePen);
    leftExchangerPressureCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    leftExchangerPressureCurve->attach(leftPressurePlot);
    // правый
    rightPressurePlot = new QwtPlot;
    rightPressurePlotLegend = new QwtLegend;
    rightPressurePlot->setTitle("Опрессовка теплообменника");
    rightPressurePlot->insertLegend(rightPressurePlotLegend, QwtPlot::RightLegend, 1.0);
    //rightPressurePlot->setAxisAutoScale(QwtPlot::yLeft);
    rightPressurePlot->setAxisScale(QwtPlot::yLeft, 0, 10);
    rightPressurePlot->setAxisScale(QwtPlot::xBottom, 0, 600);
    rightPressurePlot->setAxisTitle( QwtPlot::xBottom, " Время опрессовки, сек" );
    rightPressurePlot->setAxisTitle( QwtPlot::yLeft, " Давление, кгс" );
    rightPressurePlot->setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom );
    rightPressurePlot->setAxisLabelAlignment( QwtPlot::yLeft, Qt::AlignVCenter);

    //QPen rightInPressurePen = QPen(Qt::red);
    QPen rightExchangerPressurePen = QPen(Qt::blue);

    //rightInPressureCurve = new QwtPlotCurve("Входное давление");
    //rightInPressureCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    //rightInPressureCurve->setPen(rightInPressurePen);
    //rightInPressureCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    //rightInPressureCurve->attach(rightPressurePlot);

    rightExchangerPressureCurve = new QwtPlotCurve("Давление в теплообменнике");
    rightExchangerPressureCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    rightExchangerPressureCurve->setPen(rightExchangerPressurePen);
    rightExchangerPressureCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    rightExchangerPressureCurve->attach(rightPressurePlot);

    // Кнопки
    leftPressButton = new QPushButton("\nОпрессовка\n");
    leftPressButton->setDisabled(true);
    rightPressButton = new QPushButton("\nОпрессовка\n");
    rightPressButton->setDisabled(true);
    leftStopButton = new QPushButton("\nСТОП\n");
    //leftStopButton->setDisabled(true);
    leftStopButton->setDisabled(false);
    rightStopButton = new QPushButton("\nСТОП\n");
    //rightStopButton->setDisabled(true);
    rightStopButton->setDisabled(false);
    // set layouts
    // controls for left exchanger
    QVBoxLayout *leftControlsLayout = new QVBoxLayout;

    QFrame *leftControlsFrame = new QFrame;
    leftControlsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    leftControlsLayout->addWidget(leftControlsFrame);
    QVBoxLayout *leftControlsLayoutRelief  = new QVBoxLayout(leftControlsFrame);

    QHBoxLayout *leftinPressureLayout = new QHBoxLayout;
    QLabel *leftinPressureLbl = new QLabel;
    leftinPressureLbl->setText("Входное давление, кгс");
    leftinPressureEntry = new QLineEdit;
    leftinPressureEntry->setFixedWidth(120);
    leftinPressureEntry->setAlignment(Qt::AlignHCenter);
    QSpacerItem *leftinPressureSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Minimum);
    leftinPressureLayout->addWidget(leftinPressureLbl);
    leftinPressureLayout->addSpacerItem(leftinPressureSpacer);
    leftinPressureLayout->addWidget(leftinPressureEntry);

    QHBoxLayout *leftExchangerPressureLayout = new QHBoxLayout;
    QLabel *leftExchangerPressureLbl = new QLabel;
    QSpacerItem *leftExchangerPressureSpacer = new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Minimum);
    leftExchangerPressureLbl->setText("Давление в теплообменнике, кгс");
    leftExchangerPressureLayout->addWidget(leftExchangerPressureLbl);
    leftExchangerPressureEntry = new QLineEdit;
    leftExchangerPressureEntry->setFixedWidth(120);
    leftExchangerPressureEntry->setAlignment(Qt::AlignHCenter);
    leftExchangerPressureLayout->addSpacerItem(leftExchangerPressureSpacer);
    leftExchangerPressureLayout->addWidget(leftExchangerPressureEntry);

    leftControlsLayoutRelief->addWidget(leftPressButton);

    QSpacerItem *leftVerticalSpacer1 = new QSpacerItem(1,1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    leftControlsLayoutRelief->addSpacerItem(leftVerticalSpacer1);

    leftControlsLayoutRelief->addLayout(leftinPressureLayout);
    leftControlsLayoutRelief->addLayout(leftExchangerPressureLayout);

    QSpacerItem *leftVerticalSpacer2 = new QSpacerItem(1,1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    leftControlsLayoutRelief->addSpacerItem(leftVerticalSpacer2);

    leftControlsLayoutRelief->addWidget(leftStopButton);

    leftStatusLabel = new QLabel;
    leftStatusLabel->setText("");
    leftStatusLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    leftControlsLayoutRelief->addWidget(leftStatusLabel);

    // controls for right exchanger
    QVBoxLayout *rightControlsLayout = new QVBoxLayout;

    QFrame *rightControlsFrame = new QFrame;
    rightControlsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    rightControlsLayout->addWidget(rightControlsFrame);
    QVBoxLayout *rightControlsLayoutRelief  = new QVBoxLayout(rightControlsFrame);

    rightControlsLayoutRelief->addWidget(rightPressButton);

    QSpacerItem *rightVerticalSpacer1 = new QSpacerItem(1,1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightControlsLayoutRelief->addSpacerItem(rightVerticalSpacer1);

    QHBoxLayout *rightinPressureLayout = new QHBoxLayout;
    QLabel *rightinPressureLbl = new QLabel;
    rightinPressureEntry = new QLineEdit;
    rightinPressureEntry->setFixedWidth(120);
    rightinPressureEntry->setAlignment(Qt::AlignHCenter);
    rightinPressureLbl->setText("Входное давление, кгс");
    rightinPressureLayout->addWidget(rightinPressureLbl);
    rightinPressureLayout->addWidget(rightinPressureEntry);

    QHBoxLayout *rightExchangerPressureLayout = new QHBoxLayout;
    QLabel *rightExchangerPressureLbl = new QLabel;
    rightExchangerPressureEntry = new QLineEdit;
    rightExchangerPressureEntry->setFixedWidth(120);
    rightExchangerPressureEntry->setAlignment(Qt::AlignHCenter);
    rightExchangerPressureLbl->setText("Давление в теплообменнике, кгс");
    rightExchangerPressureLayout->addWidget(rightExchangerPressureLbl);
    rightExchangerPressureLayout->addWidget(rightExchangerPressureEntry);

    rightControlsLayoutRelief->addLayout(rightinPressureLayout);
    rightControlsLayoutRelief->addLayout(rightExchangerPressureLayout);

    QSpacerItem *rightVerticalSpacer2 = new QSpacerItem(1,1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightControlsLayoutRelief->addSpacerItem(rightVerticalSpacer2);

    rightControlsLayoutRelief->addWidget(rightStopButton);

    rightStatusLabel = new QLabel;
    rightStatusLabel->setText("");
    rightStatusLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    rightControlsLayoutRelief->addWidget(rightStatusLabel);

    // layout for image
    QVBoxLayout *imageLayout = new QVBoxLayout;
    QFrame *pictureFrame = new QFrame;
    pictureFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    imageLayout->addWidget(pictureFrame);
    QVBoxLayout *pictureLayout  = new QVBoxLayout(pictureFrame);

    QLabel *pictureLbl = new QLabel;
    QImage heaterImage(QCoreApplication::applicationDirPath() + "./img/sch.png");
    // TODO height = windowHeight / 2, width = windowWidth / 3
    pictureLbl->setFixedHeight(300);
    pictureLbl->setFixedWidth(300);
    pictureLbl->setScaledContents(true);
    pictureLbl->setPixmap(QPixmap::fromImage(heaterImage));
    pictureLayout->addWidget(pictureLbl);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    imageLayout->addLayout(buttonsLayout);
    leftStartButton = new QPushButton("\nЛевый пост\n");
    rightStartButton = new QPushButton("\nПравый пост\n");
    buttonsLayout->addWidget(leftStartButton);
    buttonsLayout->addWidget(rightStartButton);


    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setSpacing(10);
    controlLayout->addLayout(leftControlsLayout);
    controlLayout->addLayout(imageLayout);
    controlLayout->addLayout(rightControlsLayout);

    QHBoxLayout *plotsLayout = new QHBoxLayout;
    plotsLayout->setSpacing(10);

    QFrame *leftPlotFrame = new QFrame;
    leftPlotFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    plotsLayout->addWidget(leftPlotFrame);
    QVBoxLayout *leftPlotLayout = new QVBoxLayout(leftPlotFrame);
    leftPlotLayout->addWidget(leftPressurePlot);

    QFrame *rightPlotFrame = new QFrame;
    rightPlotFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    plotsLayout->addWidget(rightPlotFrame);
    QVBoxLayout *rightPlotLayout = new QVBoxLayout(rightPlotFrame);
    rightPlotLayout->addWidget(rightPressurePlot);

    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addLayout(controlLayout);
    widgetLayout->addLayout(plotsLayout);


    //=====================================================

    connect(leftStartButton, SIGNAL(clicked()), this, SLOT(setLeftStatus()));
    connect(rightStartButton, SIGNAL(clicked()), this, SLOT(setRightStatus()));
    connect(leftStopButton, SIGNAL(clicked()), this, SLOT(stopLeftStatus()));
    connect(rightStopButton, SIGNAL(clicked()), this, SLOT(stopRightStatus()));
    connect(leftPressButton, SIGNAL(clicked()), this, SLOT(lpbClick()));
    connect(rightPressButton, SIGNAL(clicked()), this, SLOT(rpbClick()));


    //qDebug()<<"From main thread: "<<QThread::currentThreadId();

    setLayout(widgetLayout);
    showMaximized();

}

Qwt_MainWindow::~Qwt_MainWindow()
{

}

void Qwt_MainWindow::showLeftInPressure(float pressure)
{
    leftinPressureEntry->setText(QString::number(pressure, 'f', roundingPrecision));
}

void Qwt_MainWindow::showRightInPressure(float pressure)
{
    rightinPressureEntry->setText(QString::number(pressure, 'f', roundingPrecision));
}

void Qwt_MainWindow::showLeftExchangerPressure(float pressure)
{
    leftExchangerPressureEntry->setText(QString::number(pressure, 'f', roundingPrecision));
}

void Qwt_MainWindow::showRightExchangerPressure(float pressure)
{
    rightExchangerPressureEntry->setText(QString::number(pressure, 'f', roundingPrecision));
}

void Qwt_MainWindow::setLeftStatus()
{
    leftStartButton->setDisabled(true);
    int n = QMessageBox::warning(0,
                                 "Левый пост",
                                 "Установите теплообменник на левый пост,"
                                 "\n подключите шланги"
                                 "\n и нажмите ОК",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 );
    if (!n) {
        n = QMessageBox::warning(0,
                                 "Левый пост",
                                 "Убедитесь, что теплообменник установлен верно",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 );
        if (!n) {
            n = QMessageBox::warning(0,
                                     "Левый пост",
                                     "Откройте кран сброса воздуха",
                                     "OK",
                                     "Отмена",
                                     QString(),
                                     0,
                                     1
                                     );
            if (!n) {
                //emit leftPressButtonClicked();
                emit startLeftPost();
                //qDebug("Left press button clicked");
            } else {
                leftStartButton->setDisabled(false);
            }
        } else {
            leftStartButton->setDisabled(false);
        }
    } else {
        leftStartButton->setDisabled(false);
    }

}

void Qwt_MainWindow::setRightStatus()
{
    rightStartButton->setDisabled(true);
    int n = QMessageBox::warning(0,
                                 "Правый пост",
                                 "Установите теплообменник на правый пост,"
                                 "\n подключите шланги"
                                 "\n и нажмите ОК",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 );
    if (!n) {
        n = QMessageBox::warning(0,
                                 "Правый пост",
                                 "Убедитесь, что теплообменник установлен верно",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 );
        if (!n) {
            n = QMessageBox::warning(0,
                                     "Правый пост",
                                     "Откройте кран сброса воздуха",
                                     "OK",
                                     "Отмена",
                                     QString(),
                                     0,
                                     1
                                     );
            if (!n) {
                //emit rightPressButtonClicked();
                emit startRightPost();
            } else {
                rightStartButton->setDisabled(false);
            }
        } else {
            rightStartButton->setDisabled(false);
        }
    } else {
        rightStartButton->setDisabled(false);
    }
}

void Qwt_MainWindow::stopLeftStatus()
{
    //leftStopButton->setDisabled(true);
    if (leftSecondsTimer->isActive()) {
        leftSecondsTimer->stop();
        this->leftTick = 0;
        emit completeEtap(5);
    } else {
        emit completeEtap(5);
    }
    //emit leftStopButtonClicked(1);
}

void Qwt_MainWindow::stopRightStatus()
{
    //rightStopButton->setDisabled(true);
    if (rightSecondsTimer->isActive()) {
        rightSecondsTimer->stop();
        this->rightTick = 0;
        emit completeRightEtap(5);
    } else {
        emit completeRightEtap(5);
    }
    //emit rightStopButtonClicked(1);
}

void Qwt_MainWindow::lpbClick()
{
    //qDebug("emit leftPressButtonClicked()");
    emit leftPressButtonClicked();
}

void Qwt_MainWindow::rpbClick()
{
    emit rightPressButtonClicked();
}

void Qwt_MainWindow::showEtapMessage(int etap)
{    
    switch (etap) {
    case 0:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        emit completeEtap(etap);
        break;
    case 1:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Откройте кран подачи, закройте кран слива."
                                 "\nПосле сброса воздуха закройте кран сброса воздуха",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStopButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(5);
        } else {
            emit completeEtap(etap);
        }
        break;
    case 2:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        emit completeEtap(etap);
        break;
    case 3:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        leftStatusLabel->setText("Подготовка к опрессовке, подождите");
        emit completeEtap(etap);
        break;
    case 4:
        // сделатm активной кнопку опрессовка
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Закройте кран подачи",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(5);
        } else {
            leftStatusLabel->setText("Подготовка завершена");
            leftPressButton->setDisabled(false);
            emit completeEtap(etap);
        }
        break;
    case 5:
    {
        // если нажата кнопка "Опрессовка"
        leftPressButton->setDisabled(true);
        leftStopButton->setDisabled(false);
        leftStatusLabel->setText("Идет опрессовка");


            for (int i = 0; i <= 600; i++) {
                x[i] = 0;
                leftExData[i] = 0.0;
                leftInData[i] = 0.0;
            }

        leftExchangerPressureCurve->setSamples(x, leftExData, 601);
        //leftInPressureCurve->setSamples(x, leftInData, 601);
        leftPressurePlot->replot();

        leftSecondsTimer->start(1000);
        break;
    }

        // Открыть кран слива, открыть кран подачи, когда включился насос -- через 10 сек закрыть кран подачи, открыть сброс
    case 6:
        /*
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Откройте кран слива, затем откройте кран сброса воздуха",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(7);
        } else {
            leftStatusLabel->setText("Идет сброс, подождите");
            leftStopButton->setDisabled(true);
            emit completeEtap(etap);
        }
        */
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Откройте кран слива, затем откройте кран подачи",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(6);
        } else {
            leftStatusLabel->setText("Идет сброс, подождите");
            //leftStopButton->setDisabled(true);
            emit completeEtap(etap);
        }
        break;
    case 61:
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Закройте кран подачи, откройте кран сброса",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(7);
        } else {
            leftStatusLabel->setText("Идет сброс, подождите");
            //leftStopButton->setDisabled(true);
            emit completeEtap(etap);
        }
        break;
    case 7:
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Откройте кран слива, затем откройте кран сброса воздуха",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(7);
        } else {
            leftStatusLabel->setText("Идет сброс, подождите");
            //leftStopButton->setDisabled(true);
            emit completeEtap(etap);
        }
        break;
    case 8:
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "После слива воды закройте"
                                 "\n краны слива, подачи и сброса",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(8);
        } else {
            emit completeEtap(etap);
        }
        break;
    case 9:
        if (QMessageBox::warning(0,
                                 "Левый пост",
                                 "Опрессовка завершена",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            leftStartButton->setDisabled(false);
            leftStatusLabel->setText("");
            emit completeEtap(9);
        } else {
            leftStatusLabel->setText("Опрессовка завершена");

            leftStartButton->setDisabled(false);
            emit completeEtap(etap);
        }
        break;
    case 99:
        break;
    case 10:
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Левый теплообменник"),
                                             tr("Номер левого теплообменника:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            leftExchangerNumber = text;
            emit completeEtap(etap);
        } else {
           emit completeEtap(11);
        }
    }
        break;
    case 101:
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Левый теплообменник"),
                                             tr("Оператор:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            leftExchangerOperator = text;
            emit completeEtap(etap);
        } else {
           emit completeEtap(11);
        }
    }
         break;
    case 11:
    {
        QPrinter printer( QPrinter::HighResolution );
        printer.setDocName( "Опрессовка теплоообменника" );
        printer.setCreator( "Опрессовка теплоообменника");
        printer.setOrientation(QPrinter::Portrait);

        QDateTime dateTime = dateTime.currentDateTime();
        QString dateTimeString = dateTime.toString("dd-MM-yyyy");
        QPainter painter;
        QFont font=painter.font() ;


        QPrintDialog dialog( &printer );
        if ( dialog.exec() )
        {
            QwtPlotRenderer renderer;

            if ( printer.colorMode() == QPrinter::GrayScale )
            {
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
                renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
            }

            double xscale = printer.pageRect().width() * 0.85;
            double yscale = printer.pageRect().height()* 0.3;

            QRectF headerRect(0, 0, xscale, yscale /8);
            QRectF pictureRect(0, yscale /8, xscale, yscale);
            QRectF labelRect(0, yscale + yscale /8, xscale, yscale);

            painter.begin(&printer);
            QTextOption textOption0(Qt::AlignCenter | Qt::AlignTop);
            font.setPointSize (14);
            painter.setFont(font);
            QString txtHeader = "Теплообменник №: " + leftExchangerNumber;
            painter.drawText(headerRect, txtHeader, textOption0);
            renderer.render(leftPressurePlot, &painter, pictureRect);
            QTextOption textOption(Qt::AlignLeft | Qt::AlignTop);
            QString txt = "\n\nИспытан: " + dateTimeString + "\n\nПодпись: " + leftExchangerOperator;
            painter.drawText(labelRect, txt, textOption);
            painter.end();

        }
        emit completeEtap(etap);
    }
        break;
    default:
        break;
    }
}

void Qwt_MainWindow::showRightEtapMessage(int etap)
{
    switch (etap) {
    case 0:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        emit completeRightEtap(etap);
        break;
    case 1:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "Откройте кран подачи, закройте кран слива."
                                 "\nПосле сброса воздуха закройте кран сброса воздуха",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStartButton->setDisabled(false);
            rightStopButton->setDisabled(false);
            rightStatusLabel->setText("Отмена испытания");
            emit completeRightEtap(5);
        } else {
            emit completeRightEtap(etap);
        }
        break;
    case 2:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        emit completeRightEtap(etap);
        break;
    case 3:
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        rightStatusLabel->setText("Подготовка к опрессовке, подождите");
        emit completeRightEtap(etap);
        break;
    case 4:
        // сделатm активной кнопку опрессовка
        //qDebug("ShowEtapMessage-> etap is %d", etap);
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "Закройте кран подачи",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStartButton->setDisabled(false);
            rightStatusLabel->setText("");
            emit completeRightEtap(5);
        } else {
            rightStatusLabel->setText("Подготовка завершена");
            rightPressButton->setDisabled(false);
            emit completeRightEtap(etap);
        }
        break;
    case 5:
    {
        // если нажата кнопка "Опрессовка"
        rightPressButton->setDisabled(true);
        rightStopButton->setDisabled(false);
        rightStatusLabel->setText("Идет опрессовка");


            for (int i = 0; i <= 600; i++) {
                rightX[i] = 0;
                rightExData[i] = 0.0;
                rightInData[i] = 0.0;
            }

        rightExchangerPressureCurve->setSamples(rightX, rightExData, 601);
        //rightInPressureCurve->setSamples(rightX, rightInData, 601);
        rightPressurePlot->replot();

        rightSecondsTimer->start(1000);
        break;
    }
    case 6:
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "Откройте кран слива, затем откройте кран подачи",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStartButton->setDisabled(false);
            rightStatusLabel->setText("");
            emit completeRightEtap(6);
        } else {
            rightStatusLabel->setText("Идет сброс, подождите");
            //rightStopButton->setDisabled(true);
            emit completeRightEtap(etap);
        }
        break;
    case 61:
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "Закройте кран подачи, откройте кран сброса",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStatusLabel->setDisabled(false);
            rightStatusLabel->setText("");
            emit completeRightEtap(7);
        } else {
            rightStatusLabel->setText("Идет сброс, подождите");
            rightStatusLabel->setDisabled(true);
            emit completeRightEtap(etap);
        }
        break;
    case 7:
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "Откройте кран слива, затем откройте кран сброса воздуха",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStartButton->setDisabled(false);
            rightStatusLabel->setText("");
            emit completeRightEtap(7);
        } else {
            rightStatusLabel->setText("Идет сброс, подождите");
            //rightStopButton->setDisabled(true);
            emit completeRightEtap(etap);
        }
        break;
    case 8:
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "После слива воды закройте"
                                 "\n краны слива, подачи и сброса",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStartButton->setDisabled(false);
            rightStatusLabel->setText("");
            emit completeRightEtap(8);
        } else {
            emit completeRightEtap(etap);
        }
        break;
    case 9:
        if (QMessageBox::warning(0,
                                 "Правый пост",
                                 "Опрессовка завершена",
                                 "OK",
                                 "Отмена",
                                 QString(),
                                 0,
                                 1
                                 )) {
            // TODO завершение испытания
            //emit finished();
            rightStartButton->setDisabled(false);
            rightStatusLabel->setText("");
            emit completeRightEtap(9);
        } else {
            rightStatusLabel->setText("Опрессовка завершена");

            rightStartButton->setDisabled(false);
            emit completeRightEtap(etap);
        }
        break;
    case 99:
        break;
    case 10:
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Правый теплообменник"),
                                             tr("Номер правого теплообменника:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            rightExchangerNumber = text;
            emit completeRightEtap(etap);
        } else {
            emit completeRightEtap(11);
         }
    }
        break;
    case 101:
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Правый теплообменник"),
                                             tr("Оператор:"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty()) {
            rightExchangerOperator = text;
            emit completeRightEtap(etap);
        } else {
           emit completeRightEtap(11);
        }
    }
         break;
    case 11:
    {
        QPrinter printer( QPrinter::HighResolution );
        printer.setDocName( "Опрессовка теплообменника" );
        printer.setCreator( "Опрессовка теплообменника");
        printer.setOrientation(QPrinter::Portrait);

        QDateTime dateTime = dateTime.currentDateTime();
        QString dateTimeString = dateTime.toString("dd-MM-yyyy");
        QPainter painter;



        QPrintDialog dialog( &printer );
        if ( dialog.exec() )
        {
            QwtPlotRenderer renderer;

            if ( printer.colorMode() == QPrinter::GrayScale )
            {
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
                renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
            }

            double xscale = printer.pageRect().width() * 0.85;
            double yscale = printer.pageRect().height()* 0.3;

            QRectF headerRect(0, 0, xscale, yscale /8);
            QRectF pictureRect(0, yscale /8, xscale, yscale);
            QRectF labelRect(0, yscale + yscale /8, xscale, yscale);

            painter.begin(&printer);
            QFont font=painter.font();
            QTextOption textOption0(Qt::AlignCenter | Qt::AlignTop);
            font.setPointSize (14);
            painter.setFont(font);
            QString txtHeader = "Теплообменник №: " + rightExchangerNumber;
            painter.drawText(headerRect, txtHeader, textOption0);
            renderer.render(rightPressurePlot, &painter, pictureRect);
            QTextOption textOption(Qt::AlignLeft | Qt::AlignTop);
            QString txt = "\n\nИспытан: " + dateTimeString + "\n\nОператор: " + rightExchangerOperator;
            painter.drawText(labelRect, txt, textOption);
            painter.end();

        }
        emit completeRightEtap(etap);
    }
        break;
    default:
        break;
    }
}

/*
void Qwt_MainWindow::nextTick()
{
    if (this->tick < 600) {
    //if (this->tick < 10) {

        x[this->tick] = this->tick;
        leftExData[this->tick] = leftExchangerPressureEntry->text().toDouble();
        leftInData[this->tick] = leftinPressureEntry->text().toDouble();

        leftExchangerPressureCurve->setSamples(x, leftExData, this->numTicks + 1);
        leftInPressureCurve->setSamples(x, leftInData, this->numTicks + 1);
        leftPressurePlot->replot();

        qDebug("Tick %d", this->tick);
        this->tick += 1;
    } else {
        secondsTimer->stop();
        this->tick = 0;
        emit completeEtap(5);
    }

}
*/

void Qwt_MainWindow::leftNextTick()
{
    if (this->leftTick < 600) {


        x[this->leftTick] = this->leftTick;
        leftExData[this->leftTick] = leftExchangerPressureEntry->text().toDouble();
        leftInData[this->leftTick] = leftinPressureEntry->text().toDouble();

        leftExchangerPressureCurve->setSamples(x, leftExData, this->leftTick + 1);
        //leftInPressureCurve->setSamples(x, leftInData, this->leftTick + 1);
        leftPressurePlot->replot();
        leftStatusLabel->setText("Идет опрессовка, осталось " + QString::number(600 - this->leftTick) + " сек.");
        this->leftTick += 1;
    } else {
        leftSecondsTimer->stop();
        this->leftTick = 0;
        emit completeEtap(5);
    }
}

void Qwt_MainWindow::rightNextTick()
{
    if (this->rightTick < 600) {


        rightX[this->rightTick] = this->rightTick;
        rightExData[this->rightTick] = rightExchangerPressureEntry->text().toDouble();
        rightInData[this->rightTick] = rightinPressureEntry->text().toDouble();

        rightExchangerPressureCurve->setSamples(rightX, rightExData, this->rightTick + 1);
        //rightInPressureCurve->setSamples(rightX, rightInData, this->rightTick + 1);
        rightPressurePlot->replot();
        rightStatusLabel->setText("Идет опрессовка, осталось " + QString::number(600 - this->rightTick) + " сек.");
        this->rightTick += 1;
    } else {
        rightSecondsTimer->stop();
        this->rightTick = 0;
        emit completeRightEtap(5);
    }
}

