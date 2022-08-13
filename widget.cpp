#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QHeaderView>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Notepad");
    this->setWindowIcon(QIcon(":/twotone_edit_note_black_36dp.png"));
    QString path = "./res.txt";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QStringList arrayList;
    while(!file.atEnd()){
        // 默认UTF-8
        QString s = file.readLine();
        arrayList << s;
    }
    file.close();
    qDebug() << arrayList;
    int rowNum = arrayList.length();
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"priority"<<"event"<<"remarks"<<"operation");
    ui->tableWidget->setRowCount(rowNum);
    //QTableWidgetItem *item = ui->tableWidget->horizontalHeaderItem(0);
    QHeaderView *q = ui->tableWidget->horizontalHeader();
    QFont s =  QFont("Microsoft YaHei");
    s.setBold(true);
    q->setFont(s);

    for(int i=0;i<rowNum;i++){
        QString s = arrayList[i];
        QStringList array = s.split(" ");
        int len = array.length();
        qDebug() << array;
        for (int j=0;j<len;j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(array[j]));
        }
        QPushButton* button =  new QPushButton("",this);
        button->setIcon(QIcon(":/twotone_delete_sweep_black_24dp.png"));
        ui->tableWidget->setCellWidget(i,3,button);
        connect(button,&QPushButton::clicked,[=]{
            int row = ui->tableWidget->currentRow();
            qDebug()<< row;
            ui->tableWidget->removeRow(row);
        });
    }
    file.close();
    connect(ui->add_button,&QPushButton::clicked,[=]{
        QString s = ui->textEdit->toPlainText();
        QStringList array = s.split(" ");
        int len = array.length();
        qDebug() << s;
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);
        for (int j=0;j<len;j++){
            ui->tableWidget->setItem(rowCount,j,new QTableWidgetItem(array[j]));
        }
        QPushButton* button =  new QPushButton("",this);
        button->setIcon(QIcon(":/twotone_delete_sweep_black_24dp.png"));
        ui->tableWidget->setCellWidget(rowCount,3,button);
        connect(button,&QPushButton::clicked,[=]{
            int row = ui->tableWidget->currentRow();
            qDebug()<< row;
            ui->tableWidget->removeRow(row);
        });
        ui->tableWidget->resizeColumnsToContents();
    });
    connect(ui->put_disk,&QPushButton::clicked,[=]{
        QStringList resList;
        int rowCount = ui->tableWidget->rowCount();
        for(int i=0;i<rowCount;i++){
            QString s;
            for(int j=0;j<3;j++){
                if(ui->tableWidget->item(i,j)!=NULL){
                    s += ui->tableWidget->item(i,j)->text();
                }else{
                    s+=" ";
                }
                if(j!=2){
                    s += " ";
                }
            }
            resList << s;
        }
        resList.sort();
        qDebug()<<resList;
        QString wPath = "./res.txt";
        QFile wFile(wPath);
        wFile.open(QIODevice::Truncate|QIODevice::ReadWrite);
        for(int i=0;i<resList.length();i++){
            wFile.write(resList[i].toUtf8().data());
            if(resList[i].indexOf("\r\n")==-1){
                wFile.write("\r\n");
        }
        }
        wFile.close();
    });
    ui->tableWidget->resizeColumnsToContents();
}

Widget::~Widget()
{
    delete ui;
}

