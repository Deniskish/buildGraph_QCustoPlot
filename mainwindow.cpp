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
            QString line = inPotok.readLine().trimmed();//.trimmed() убирает пробелы
            //qDebug() << "Считанная строка" << line;
            if (line.isEmpty()) continue;
            QDateTime dateTime;
            if (line.startsWith("[")){
                int end = line.indexOf("]");
                if (end != -1) {
                    QString timeStr = line.mid(1, end -1);
                    dateTime = QDateTime::fromString(timeStr, "yyyy-MM-dd_HH-mm-ss.zzz");
                    qDebug() << "Считанная строка" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
                    line = line.mid(end+1);
                }
            }
        }
        filePotok.close();
    }
}


void MainWindow::on_pushButton_2_clicked()
{

}

