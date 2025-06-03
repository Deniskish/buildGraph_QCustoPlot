#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->customPlot->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    //ui->customPlot->graph()->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->setMouseTracking(true);
    connect(ui->customPlot, &QCustomPlot::mouseMove,this,&MainWindow::onMouseMove);
    //setupQuadraticDemo(ui->customPlot);
}




// void MainWindow::setupQuadraticDemo(QCustomPlot *customPlot)
// {
//     demoName = "Quadratic Demo";
//     // generate some data:
//     QVector<double> x(101), y(101); // initialize with entries 0..100
//     for (int i=0; i<101; ++i)
//     {
//         x[i] = i/50.0 - 1; // x goes from -1 to 1
//         y[i] = x[i];  // let's plot a quadratic function
//     }
//     // create graph and assign data to it:
//     customPlot->addGraph();
//     customPlot->graph(0)->setData(x, y);
//     // give the axes some labels:
//     customPlot->xAxis->setLabel("x");
//     customPlot->yAxis->setLabel("y");
//     customPlot->xAxis2->setVisible(true);  // верхняя ось(по умолчанию скрыта)
//     customPlot->yAxis2->setVisible(true);  // правая ось(по умолчанию скрыта)
//     ui->customPlot->setInteraction(QCP::iRangeDrag, true);
//     ui->customPlot->setInteraction(QCP::iRangeZoom, true);
//     // ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
//     // ui->customPlot->axisRect()->setRangeZoomFactor(0.99);

//     // set axes ranges, so we see all data:
//     customPlot->xAxis->setRange(-1, 1);
//     customPlot->yAxis->setRange(0, 1);
// }
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
    ui -> lineE_NameFile -> insert(fileContur);

    if (fileContur.isEmpty()) return;//если файл пустой выход из окна
    fileContur_Name = fileContur;

    lines.clear();
    secondsList.clear();
    timeDateList.clear();//очистка данных старого графикаа
    //-------------------

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
                    {
                        startTime = dateTime;
                    }
                    double seconds = startTime.msecsTo(dateTime)/1000;//перевел в милисекунды
                    secondsList.append(seconds);//для сохранния основного
                    timeDateList.append(dateTime);//для использования  в показе маркера и легенды даты для него то ж самое толькол джругой формат, проработать надо(Приоритет)
                    //() << "Считанная строка" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
                    line = line.mid(end+1);//удалить время
                    lines.append(line);

                    //сейчас в line, все содержимое строки, но удлалили время
                    //qDebug() << line;
                }
            }
        }
        filePotok.close();
    }
}


void MainWindow::on_pushButton_2_clicked()//кнопка Show
{
    ui->customPlot->clearGraphs();//очищает график
    // for (const QString &dt : dataList){//для теста
    //     qDebug() << "Время" << dt;
    // }
    // for (const QDateTime &xt : timeDateList){
    //     qDebug() << "Время" << xt;
    // }
    QVector<double> x, y;

    QString userParametr = ui -> lineE_ParametrName -> text().trimmed();//значение указанное пользователем
    if (!userParametr.isEmpty())// проверка на непустое значение
    {
        bool ok = false;
        userParametr.toDouble(&ok);// перевод значения в double
        for (int i = 0; i < lines.size(); i++)//в lines хранятся строки со значениями, но уже без времени в QVector<String>
        {
            QString line = lines[i];// берем каждую строку по индексу
            double time = secondsList.value(i, -1);// в secondsList хранятся строки со значениями, но уже без времени в QVector<double>
            QStringList listLine = line.split(";", Qt::SkipEmptyParts);// разбиение линии с индексом i по ";", получается много значений
            for (const QString& item : listLine)// берется каждый индекс по очереди, например MO_MB = 1, TA_RTS=1 и т.д.
            {
                QStringList pereborZnachenie = item.split("=", Qt::SkipEmptyParts);//делится на название и значение
                if (pereborZnachenie.size() == 2)//проверка, что в строке было два значения. Например TA_RTS=1, станет TA_RTS и 1.
                {
                    QString key = pereborZnachenie[0].trimmed();//присваивание key первого индекса и удаление пробела к конце
                    QString znachStr = pereborZnachenie[1].trimmed();//присваивание znachStr второго индекса и удаление пробела к конце
                    if (key == userParametr)//провека равно ли значение тому, что задал пользователь
                    {
                        bool ok;
                        double znach = znachStr.toDouble(&ok);//преобразование в double
                        if (ok && time >= 0)
                        {
                            x.append(time);//время
                            y.append(znach);//значение
                            //qDebug() << "x = " << time << ", y = " << znach;
                        }
                    }

                }
            }
        }

    }
    ui->customPlot->clearGraphs();//очищает график
    // ui->customPlot->xAxis2->setVisible(true);  // верхняя ось(по умолчанию скрыта), так и не понял, зачем эти оси
    // ui->customPlot->yAxis2->setVisible(true);  // правая ось(по умолчанию скрыта)
    ui->customPlot->legend->setFont(QFont("System",12));
    ui->customPlot->addGraph();//добавляет график
    ui->customPlot->graph()->setAdaptiveSampling(false);


    QPen pen;
    pen.setWidth(1);
    //pen.setStyle(Qt::DashLine);//будет плавная линия
    pen.setColor(Qt::red);
    ui->customPlot->graph()->setData(x, y);//график строится по значения x, y

    //фигня какая то(ни на что ни на что не повлияло)
    //ui->customPlot->graph(0)->setAntialiased(true);  // сглаживает линии
    //ui->customPlot->setNotAntialiasedElements(QCP::aeAxes | QCP::aeLegend); // отключает сглаживание для только графика и легенды
    //ui->customPlot->graph()->setAdaptiveSampling(true); // Критично для плавности!
    //ui->customPlot->setAntialiasedElements(QCP::aeGraphs); // Включить для линий
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    ui->customPlot->xAxis->setLabel("Время (сек)");//значение по оси X
    ui->customPlot->yAxis->setLabel(userParametr);//значение по оси Y
    //ui->customPlot->graph(0)->setBrush(QBrush(QColor(255,50,30,80)));//заливка области под графиком, послений параметр отвечает за прозрачность
    ui->customPlot->setMouseTracking(true);//включает отслеживание мыши
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);//отвечает за перемещение графика, по нажатию мыши
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);//отвечает за маштабирование графика, по нажатию мыши
    //ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->Axis);//по сути то же перемещение графика
    //ui->customPlot->axisRect()->setRangeZoomFactor(0.1);//параметр указывает на скорость маштабирования
    if (!x.isEmpty()) {
        ui->customPlot->xAxis->setRange(x.first(), x.last());//диапозон оси время
    }
    if (!y.isEmpty()) {
        double minY = *std::min_element(y.begin(), y.end());
        double maxY = *std::max_element(y.begin(), y.end());
        ui->customPlot->yAxis->setRange(minY, maxY);
    }
    ui->customPlot->replot();


    // if( fileContur_Name == "" ){
    //     QMessageBox::information(this,"fail","Сохранить не удалось");
    //     return;
    // }


    // if( fileContur_Name.endsWith(".png") ){
    //     QMessageBox::information(this,"success","Successfully saved as PNG file");
    //     ui->customPlot->savePng( fileContur_Name, ui->customPlot->width(), ui->customPlot->height() );

    // }
    // if( fileContur_Name.endsWith(".jpg")||fileContur_Name.endsWith(".jpeg") ){
    //     QMessageBox::information(this,"success","Successfully saved as JPG file");
    //     ui->customPlot->saveJpg( fileContur_Name, ui->customPlot->width(), ui->customPlot->height() );

    // }
    // if( fileContur_Name.endsWith(".bmp") ){
    //     QMessageBox::information(this,"success","Successfully saved as BMP file");
    //     ui->customPlot->saveBmp( fileContur_Name, ui->customPlot->width(), ui->customPlot->height() );

    // }
    // if( fileContur_Name.endsWith(".txt") ){
    //     QMessageBox::information(this,"success","«Успешно сохранено в виде PDF-файла»");


    //     //Сохранить файл с окончанием на .pdf
    //     QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("text file (*.pdf)"));
    //     qDebug() << "Выбран файл:" << fileName;
    //     fileContur_NameNew = fileName;

    //     if (fileName.isEmpty()) return;

    //     QFile file(fileName);
    //     if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
    //         qDebug() << "Не удалось открыть файл для записи!";
    //         return;
    //     }
    //     file.close();


    //     ui->customPlot->savePdf( fileContur_NameNew, ui->customPlot->width(), ui->customPlot->height());
    // }
    // else{
    //     // В противном случае гиперссылка называется «Сохранить файл в формате .pdf»
    //                        QMessageBox::information(this,"успешно","Успешно сохранено в формате PDF по умолчанию (работает условие else");
    //     ui->customPlot->savePdf(fileContur_Name.append(".pdf"), ui->customPlot->width(), ui->customPlot->height() );
    // }




}

void MainWindow::onMouseMove(QMouseEvent* event) {
    if (!ui->customPlot->graphCount()) return;
    // Получить координаты мыши в координатах графика
    int x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());//pixelToCoord преобразует координату из пикселей окна в координаты данных графика (число с плавающей точкой).
    int y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    // Нахождение ближайшей точкки данных (пример с QCPGraph)
    double minDist = std::numeric_limits<double>::max();
    int closestIndex = -1;//индекс точки, которая сейчас считается ближайшей к курсору. Изначально -1, то есть "нет подходящей точки".
    QCPGraph *graph = ui->customPlot->graph(0);


    int graphSize = graph->data()->size();
    if (graphSize == 0) return;

    for (int i = 0; i < graphSize; ++i)
    {
        double dataX = ui->customPlot->graph(0)->data()->at(i)->key;//at(i) возвращает идентификатор по индексу i в списке контекста.
        double dataY = ui->customPlot->graph(0)->data()->at(i)->value;
        double dist = std::sqrt(std::pow(x - dataX, 2) + std::pow(y - dataY, 2));
        if (dist < minDist) {
            minDist = dist;
            closestIndex = i;
        }
    }

    //Если найдена близкая точка данных, отображается маркер
    if (closestIndex != -1 && minDist < 10) { //нашли хоть какую-то точку (индекс обновился)&&точка достаточно близко к мыши;

        if (tracer)//это условие нужно для того, чтобы при открытии другого файла, произошла привязка маркера к другому графику
        {
            tracer->setGraph(ui->customPlot->graph(0));
        }

        if (!tracer) {//если маркера не было(в начале его и не будет)
            tracer = new QCPItemTracer(ui->customPlot);//создается новый QCPItemTracer (маркер)
            tracer->setGraph(ui->customPlot->graph(0));//привязка к графу
            tracer->setPen(QPen(Qt::red));//обводка
            tracer->setBrush(Qt::red);//заливка
            tracer->setStyle(QCPItemTracer::tsCircle);//форма
            tracer->setSize(7);//размер

            textWithTracer = new QCPItemText(ui->customPlot);
            //textWithTracer->setText(timeDateList(closestIndex));
            textWithTracer->position->setParentAnchor(tracer->position);
        }

        tracer->setGraphKey(ui->customPlot->graph(0)->data()->at(closestIndex)->key);//Устанавливаем положение маркера строго на координату X найденной ближайшей точки.
        tracer->setVisible(true);//маркер видим
        textWithTracer->setVisible(true);//маркер видим

        QDateTime dateTimeForGraph = timeDateList[closestIndex];
        if (!dateTimeForGraph.isValid()) return;

        if (closestIndex >= 0 && closestIndex < timeDateList.size())
        {
            dateTimeForGraph = timeDateList[closestIndex];
        }
        else
        {
            dateTimeForGraph = QDateTime();
        }
        QString dateTimeForGraphString = dateTimeForGraph.toString("yyyy-MM-dd_HH-mm-ss.zzz");
        //qDebug() << "что же будет дальше" <<dateTimeForGraphString;
        QString valueGraph = QString::number(graph->data()->at(closestIndex)->value);
        QString keyGraph = QString::number(graph->data()->at(closestIndex)->key);
        QString finalText = dateTimeForGraphString + "\n" + valueGraph + "\n" +  keyGraph;
        textWithTracer->setText(finalText);
        textWithTracer->setVisible(true);

    } else if (tracer) {
        tracer->setVisible(false);//не видим
        textWithTracer->setVisible(false);//не видим
    }
    ui->customPlot->replot();
}


void MainWindow::on_pushButton_3_clicked()
{
    close();
}


void MainWindow::on_pushButton_4_clicked()
{
    if( fileContur_Name == "" ){
        QMessageBox::information(this,"fail","Сохранить не удалось");
        return;
    }


    // if( fileContur_Name.endsWith(".png") ){
    //     QMessageBox::information(this,"success","Successfully saved as PNG file");
    //     ui->customPlot->savePng( fileContur_Name, ui->customPlot->width(), ui->customPlot->height() );

    // }
    // if( fileContur_Name.endsWith(".jpg")||fileContur_Name.endsWith(".jpeg") ){
    //     QMessageBox::information(this,"success","Successfully saved as JPG file");
    //     ui->customPlot->saveJpg( fileContur_Name, ui->customPlot->width(), ui->customPlot->height() );

    // }
    // if( fileContur_Name.endsWith(".bmp") ){
    //     QMessageBox::information(this,"success","Successfully saved as BMP file");
    //     ui->customPlot->saveBmp( fileContur_Name, ui->customPlot->width(), ui->customPlot->height() );

    // }
    if( fileContur_Name.endsWith(".txt") ){
        QMessageBox::information(this,"success","«Успешно сохранено в виде PDF-файла»");


        //Сохранить файл с окончанием на .pdf
        QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("text file (*.pdf)"));
        qDebug() << "Выбран файл:" << fileName;
        fileContur_NameNew = fileName;

        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Не удалось открыть файл для записи!";
            return;
        }
        file.close();


        ui->customPlot->savePdf( fileContur_NameNew, ui->customPlot->width(), ui->customPlot->height());
    }
    else{
        // В противном случае гиперссылка называется «Сохранить файл в формате .pdf»
        QMessageBox::information(this,"успешно","Успешно сохранено в формате PDF по умолчанию (работает условие else");
        ui->customPlot->savePdf(fileContur_Name.append(".pdf"), ui->customPlot->width(), ui->customPlot->height() );
    }
}

