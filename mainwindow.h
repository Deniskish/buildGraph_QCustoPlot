#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"
#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTableWidget>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QGridLayout>
#include <QList>
#include <QString>
#include <QVector>
#include <QFileDialog>
#include <cmath>

struct structPoints{
    float x;
    float y;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupQuadraticDemo(QCustomPlot *customPlot);
    void plotFromFile(QCustomPlot *customPlot);


    // QVector<QVector<float>> znacheniyaParametrs;
    // QVector<float> znachenieParametr;
    // QVector<float> timeGraph;

    QVector<QString>lines;
    QString listznacheniy;
    QString fileContur_Name;
    QString fileContur_NameNew;

    QVector<double> secondsList;
    QVector<QDateTime> timeDateList;
    QVector<QString> dataString;
    QStringList dataList;
    double AvarageHeader;
    QVector<double> ValueHeader;
    QVector<QString> ValueHeaderStr;
    QString userParametrGlobal;//параметр для отрисовки графика, вводит пользователь

    int xAxis;
    int yAxis;
protected:
    //void mouseMoveEvent(QMouseEvent *event) override;


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_auto_scale_clicked();

    void on_export_graph_and_data_clicked();

private:
    Ui::MainWindow *ui;
    QString demoName;


    QCPItemTracer *tracer = nullptr;//для маркера
    QCPItemText *textWithTracer = nullptr;//для текста возле маркера
    void onMouseMove(QMouseEvent* event);
    void onMouseClicked(QMouseEvent* event);

};
#endif // MAINWINDOW_H
//30.05.2025
