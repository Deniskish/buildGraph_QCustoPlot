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
        y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QVector<structPoints> vectorPoints;
    QVector<structPoints> vectorPointsLineEdit;
    /*Данные считываются с LineEdit и добавляются в QVector*/
    /*Данные читаются из файла выбранного пользователем*/
    QString fileContur = QFileDialog::getOpenFileName(nullptr, tr("Open file"), "", tr("text file (*.txt)"));
    qDebug() << "Выбран файл" << fileContur;

    if (fileContur.isEmpty()) return;//если файл пустой выход из окна

    QFile filePotok(fileContur);
    if (filePotok.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream inPotok(&filePotok);
        while (!inPotok.atEnd()){
            QString line = inPotok.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList listLine = line.split(" ", Qt::SkipEmptyParts);
            if (listLine.size() == 2){
                structPoints points;
                points.x = listLine[0].toFloat();
                points.y = listLine[1].toFloat();
                vectorPoints.append(points);
            }
        }
        filePotok.close();
    }
}

