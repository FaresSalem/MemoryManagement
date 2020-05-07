#include "memorymangement.h"
#include "ui_memorymangement.h"
#include <QMessageBox>

// here is global variables to be shared with whole classes and functions
QVector<int>memorySize(0);
int holesCount,holesTotalCount, HoleStartAdress, holeSize,
processesCount,processesTotalCount,
segmentsCount,segmentTotalCount,
segmentSize;


MemoryMangement::MemoryMangement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MemoryMangement)
{
    ui->setupUi(this);
    ui->label_numberOfHoles->hide();
    ui->spinBox_HolesCount->hide();
    ui->label_numberOfProcess->hide();
    ui->spinBox_ProcessesCount->hide();
    ui->label_holesStart->hide();
    ui->spinBox_holeStart->hide();
    ui->label_HoleSize->hide();
    ui->spinBox_holeSize->hide();
    ui->label_segmentCount->hide();
    ui->spinBox_segmentsCount->hide();
    ui->label_segmentSize->hide();
    ui->spinBox_segmentSize->hide();
    ui->label_memoryFit->hide();
    ui->radioButton_BestFit->hide();
    ui->radioButton_FirstFit->hide();

}

MemoryMangement::~MemoryMangement()
{
    delete ui;
}


void MemoryMangement::on_spinBox_memorySize_editingFinished()
{
    bool assigned=false;
    if (!assigned)
    {
        int size=ui->spinBox_memorySize->value();
        memorySize.resize(size); // resize memory size
        //QString text ="size="+QString::number(size);
        //QString text2 ="first="+QString::number(memorySize[0]);
        //QMessageBox::information(this,text,text2);

        for(int var=0;var<memorySize.size();var++) {
            ui->listWidget_processes->addItem(QString::number(var));
            ui->listWidget_processes->item(var)->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget_processes->item(var)->setBackgroundColor(Qt::gray);
        }
        assigned=true;
        ui->label_numberOfHoles->show();
        ui->spinBox_HolesCount->show();
    }
    if (assigned)
    {
        ui->listWidget_processes->clear();
        int size=ui->spinBox_memorySize->value();
        memorySize.resize(size); // resize memory size
        //QString text ="size="+QString::number(size);
        //QString text2 ="first="+QString::number(memorySize[0]);
        //QMessageBox::information(this,text,text2);

        for(int var=0;var<memorySize.size();var++) {
            ui->listWidget_processes->addItem(QString::number(var));
            ui->listWidget_processes->item(var)->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget_processes->item(var)->setBackgroundColor(Qt::gray);
        }
        assigned=true;
    }
}

void MemoryMangement::on_spinBox_HolesCount_editingFinished()
{
    holesTotalCount=ui->spinBox_HolesCount->value();
    ui->label_holesStart->setText("Enter 1 hole start adress");
    ui->label_HoleSize->setText("Enter 1 hole size");
    holesCount=1;
    ui->label_holesStart->show();
    ui->spinBox_holeStart->show();
    ui->label_HoleSize->show();
    ui->spinBox_holeSize->show();
}

void MemoryMangement::on_spinBox_holeStart_editingFinished()
{
    HoleStartAdress=ui->spinBox_holeStart->value();
}

void MemoryMangement::on_spinBox_holeSize_editingFinished()
{
    if (holesCount<=holesTotalCount)
    {
        holeSize=ui->spinBox_holeSize->value();
        // here we would pass it to the the functin hole.assign
        for (int i=HoleStartAdress;i<HoleStartAdress+holeSize;i++) {
           ui->listWidget_processes->item(i)->setText("hole "+QString::number(holesCount));
           ui->listWidget_processes->item(i)->setBackgroundColor(Qt::black);
           ui->listWidget_processes->item(i)->setForeground(Qt::white);
        }
        holesCount++;
        if (holesCount<=holesTotalCount)
        {
        ui->label_holesStart->setText("Enter "+QString::number(holesCount) +" hole start adress");
        ui->label_HoleSize->setText("Enter "+QString::number(holesCount) +" hole size");}
    }
    if (holesCount>holesTotalCount)
    {
        ui->label_holesStart->hide();
        ui->spinBox_holeStart->hide();
        ui->label_HoleSize->hide();
        ui->spinBox_holeSize->hide();
        ui->label_numberOfProcess->show();
        ui->spinBox_ProcessesCount->show();
    }

}



void MemoryMangement::on_radioButton_BestFit_clicked()
{
    // use best fit function
}

void MemoryMangement::on_radioButton_FirstFit_clicked()
{
    // use first fit function
}


void MemoryMangement::on_listWidget_processes_doubleClicked(const QModelIndex &index)
{
    //here to free process with in this index
}

void MemoryMangement::on_listWidget_processes_clicked(const QModelIndex &index)
{
    //here we can show segmentation table
}

void MemoryMangement::on_spinBox_ProcessesCount_editingFinished()
{
    processesTotalCount=ui->spinBox_ProcessesCount->value();
    ui->label_segmentCount->setText("Enter 1 process segments Count");
    processesCount=1;
    ui->label_segmentCount->show();
    ui->spinBox_segmentsCount->show();
    ui->label_segmentSize->show();
    ui->spinBox_segmentSize->show();
    //initialize vector of processes here
}

void MemoryMangement::on_spinBox_segmentsCount_editingFinished()
{
    segmentTotalCount=ui->spinBox_segmentsCount->value();
    segmentsCount=1;
    ui->label_segmentSize->setText("Enter process "+QString::number(processesCount)+" segment "+QString::number(segmentsCount) +" segment size");

}

void MemoryMangement::on_spinBox_segmentSize_editingFinished()
{
    if (segmentsCount<segmentTotalCount)
    {
        segmentsCount++;
        ui->label_segmentSize->setText("Enter process "+QString::number(processesCount)+" segment "+QString::number(segmentsCount) +" segment size");
        segmentSize=ui->spinBox_segmentSize->value();
        ui->statusbar->showMessage("process "+QString::number(processesCount)+" segment "+QString::number(segmentsCount-1)+"is "+QString::number(segmentSize));
        // here we would pass it to the the functin segment.assign
    }
    else if (processesCount<processesTotalCount)
    {
        processesCount++;
        segmentsCount=1;
        ui->label_segmentSize->setText("Enter process "+QString::number(processesCount)+" segment "+QString::number(segmentsCount) +" segment size");
        ui->label_segmentCount->setText("Enter "+QString::number(processesCount) +" process segments Count");
        segmentTotalCount=ui->spinBox_segmentsCount->value();
    }
    else {
        ui->label_segmentCount->hide();
        ui->spinBox_segmentsCount->hide();
        ui->label_segmentSize->hide();
        ui->spinBox_segmentSize->hide();
        ui->label_memoryFit->show();
        ui->radioButton_BestFit->show();
        ui->radioButton_FirstFit->show();
    }
}
