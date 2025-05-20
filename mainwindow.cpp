#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupQuadraticDemo(ui->customPlot);
}
void MainWindow::setupQuadraticDemo(QCustomPlot *customPlot)
{
    demoName = "Quadratic Demo";
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
        x[i] = i/50.0 - 1; // x goes from -1 to 1
        y[i] = x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    customPlot->xAxis2->setVisible(true);  // верхняя ось(по умолчанию скрыта)
    customPlot->yAxis2->setVisible(true);  // правая ось(по умолчанию скрыта)
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);
    // ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
    // ui->customPlot->axisRect()->setRangeZoomFactor(0.99);

    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()//кнопка Open file
{
    /*Данные считываются с LineEdit и добавляются в QVector*/
    /*Данные читаются из файла выбранного пользователем*/
    QString fileContur = QFileDialog::getOpenFileName(nullptr, tr("Open file"), "", tr("text file (*.txt)"));
    qDebug() << "Выбран файл" << fileContur;

    if (fileContur.isEmpty()) return;//если файл пустой выход из окна

    QVector<double> x, y;
    QDateTime startTime;

    QFile filePotok(fileContur);
    if (filePotok.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream inPotok(&filePotok);
        while (!inPotok.atEnd()){
            QString line = inPotok.readLine().trimmed();//.trimmed() убирает пробелы
            //qDebug() << "Считанная строка" << line;
            if (line.isEmpty()) continue;

            //отделение QDateTime
            QDateTime dateTime;
            if (line.startsWith("[")){
                int end = line.indexOf("]");
                if (end != -1) {//Проверяем, нашли ли мы скобку ]. Если indexOf вернёт -1, значит ] не найден, и дальше идти нельзя.


                    QString timeStr = line.mid(1, end-1);//извлекает end-1 символов, ничная с 1(то есть, получится знаение между [ и ], это и есть время
                    dateTime = QDateTime::fromString(timeStr, "yyyy-MM-dd_HH-mm-ss.zzz");//определил QDateTime
                    if (!dateTime.isValid()) continue;

                    if (!startTime.isValid())//startTime не установлен, значит будет ошибка valid, если ошибка(в начале всегда) приравниваю к dateTime это первое значение времени
                        startTime = dateTime;

                    float seconds = startTime.msecsTo(dateTime)/1000;//перевел в милисекунды
                    secondsList.append(seconds);
                    qDebug() << "Считанная строка" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
                    line = line.mid(end);//удалить время
                    lines.append(line);

                    //сейчас в line, все содержимое строки, но удлалили время
                    qDebug() << line;
                }
            }
        }
        filePotok.close();
    }
}


void MainWindow::on_pushButton_2_clicked()//кнопка Show
{
    QVector<double> x, y;

    QString userParametr = ui -> lineE_ParametrName -> text().trimmed();
    if (!userParametr.isEmpty())
    {
        bool ok = false;
        userParametr.toFloat(&ok);
        for (int i = 0; i < lines.size(); i++)
        {
            QString line = lines[i];
            double time = secondsList.value(i, -1);
            QStringList listLine = line.split(";", Qt::SkipEmptyParts);
            for (const QString& item : listLine)
            {
                QStringList pereborZnachenie = item.split("=", Qt::SkipEmptyParts);
                if (pereborZnachenie.size() == 2)//проверка, что в строке было два значения. Например TA_RTS=1, станет TA_RTS и 1.
                {
                    QString key = pereborZnachenie[0].trimmed();
                    QString znachStr = pereborZnachenie[1].trimmed();
                    if (key == userParametr)
                    {
                        bool ok;
                        float znach = znachStr.toDouble(&ok);
                        if (ok && time >= 0)
                        {
                            x.append(time);
                            y.append(znach);
                            qDebug() << "x = " << time << ", y = " << znach;
                        }
                    }

                }
            }
        }

    }
    ui->customPlot->clearGraphs();
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->xAxis->setLabel("Время (сек)");
    ui->customPlot->yAxis->setLabel(userParametr);
    ui->customPlot->xAxis2->setVisible(true);  // верхняя ось(по умолчанию скрыта)
    ui->customPlot->yAxis2->setVisible(true);  // правая ось(по умолчанию скрыта)
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);
    // ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
    // ui->customPlot->axisRect()->setRangeZoomFactor(0.1);


    if (!x.isEmpty()) {
        ui->customPlot->xAxis->setRange(x.first(), x.last());
    }
    if (!y.isEmpty()) {
        auto [minIt, maxIt] = std::minmax_element(y.begin(), y.end());
        ui->customPlot->yAxis->setRange(*minIt, *maxIt);
    }

    ui->customPlot->replot();
}

