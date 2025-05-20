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

    QVector<float> znachenieParametr;

private:
    Ui::MainWindow *ui;
    QString demoName;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};
#endif // MAINWINDOW_H
