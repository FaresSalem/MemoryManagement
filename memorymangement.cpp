#include "memorymangement.h"
#include "ui_memorymangement.h"
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <QDebug>
#include <utility>

// here is global variables to be shared with whole classes and functions
int holesCount,holesTotalCount, HoleStartAdress, holeSize,Size,
processesCount=1,processesTotalCount,
segmentsCount,segmentTotalCount,segmentSize;
bool bestfit=false;
using namespace std;



MemoryMangement::MemoryMangement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MemoryMangement)
{
    ui->setupUi(this);
    ui->statusbar->showMessage("single click to show segmentation table           double click to delete process      defult mood is first fit");
//    ui->label_numberOfHoles->hide();
//    ui->spinBox_HolesCount->hide();
//    ui->label_numberOfProcess->hide();
//    ui->spinBox_ProcessesCount->hide();
//    ui->label_holesStart->hide();
//    ui->spinBox_holeStart->hide();
//    ui->label_HoleSize->hide();
//    ui->spinBox_holeSize->hide();
//    ui->label_segmentCount->hide();
//    ui->spinBox_segmentsCount->hide();
//    ui->label_segmentSize->hide();
//    ui->spinBox_segmentSize->hide();
//    ui->label_memoryFit->hide();
//    ui->radioButton_BestFit->hide();
//    ui->radioButton_FirstFit->hide();
    ui->tableWidget->hide();

}
typedef struct hole
{
    int base;
    int limit;
}Hole;

typedef struct segment
{
    QString segment_name;
    int limit;
    int base;
    int seg_end;
}Segment;

typedef struct process
{
    int process_id;
    vector <Segment> seg_list;
}Process;

vector <hole> hole_list;                           //data structure to store information of holes in memory
vector <Process> process_list;                     //data structure to store information of processes in memory
vector <int> memory;//was only of integers
vector<process>pending_process;
vector<Segment> seg_list;
vector <int> sec_mem(0);



void initialize_memory(int mem_size)
{
    for (int i = 0; i < mem_size;i++)
    {
        memory[i]=-1;//
    }
}
// assigning holes additionally assigns 2 processes at maximum
// if hole assigned to the start of the memory the next total memory will be assigned as a process
// if hole assigned to the end of the memory, the previous total memory will be assigned as a process
// if there is no holes before this hole, the memory the previous total memory will be assigned as a process
// if there is no holes after this hole, the memory the previous total memory will be assigned as a process
// if there is hole before or after this hole, assign process only to the opposite direction.


void assign_hole(int base, int limit)
{

    hole_list.push_back({
                       base,
                       limit });
    int counter=0;
    for (int j = base;j < base+limit;j++)
            memory[j] = 0;
    if (memory[base-1]==-1)
    {
        counter=0;
        for (int i=base-1;i>=0&&memory[i]==-1;i--)
        {
            counter++;
            memory[i]=processesCount;
        }
        Segment s1 ={"segment "+QString::number(1),counter,base-counter,base-1};
        seg_list.push_back(s1);
        process_list.push_back({ processesCount,seg_list });
        processesCount++;
        seg_list.erase(seg_list.begin(),seg_list.end());

    }
     if (memory[base+limit]==-1||memory[base+limit]>0)
    {
        if (memory[base+limit]>0)
        {
            process_list[memory[base+limit]-1].seg_list[0].seg_end=base-1;
        }
         counter=0;
        for (size_t i=base+limit;i<memory.size()&&(memory[i]>0||memory[i]==-1);i++)
          {
            counter++;
            memory[i]=processesCount;
          }
        Segment s1 ={"segment "+QString::number(1),counter,base+limit,base+limit+counter-1};
        seg_list.push_back(s1);
        process_list.push_back({ processesCount,seg_list });
        processesCount++;
        seg_list.erase(seg_list.begin(),seg_list.end());
    }
}


MemoryMangement::~MemoryMangement()
{
    delete ui;
}
void MemoryMangement::reInit()
{
    holesCount=1;
    processesCount=1;
    segmentsCount=1;
    process_list.clear();
    seg_list.clear();
    pending_process.clear();
    hole_list.clear();
    sec_mem.clear();
    ui->tableWidget->hide();
    bestfit=false;
}

void MemoryMangement::on_spinBox_memorySize_editingFinished()
{
    bool assigned=false;
    if (!assigned)
    {
        reInit();
        Size=ui->spinBox_memorySize->value();// resize memory size
        memory.resize(Size);
        sec_mem.resize(Size);
        initialize_memory(Size);
        //QString text ="size="+QString::number(size);
        //QString text2 ="first="+QString::number(memorySize[0]);
        //QMessageBox::information(this,text,text2);

        for(size_t var=0;var<memory.size();var++) {
            ui->listWidget_processes->addItem(" ");
            ui->listWidget_processes->item(var)->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget_processes->item(var)->setBackgroundColor(Qt::gray);
        }
        assigned=true;
//        ui->label_numberOfHoles->show();
//        ui->spinBox_HolesCount->show();
    }
    if (assigned)
    {
        reInit();
        ui->listWidget_processes->clear();
        int size=ui->spinBox_memorySize->value();
        memory.resize(size);
        initialize_memory(size);
        for(size_t var=0;var<memory.size();var++) {
            ui->listWidget_processes->addItem("");
            ui->listWidget_processes->item(var)->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget_processes->item(var)->setBackgroundColor(Qt::gray);
        }
    }
}

void MemoryMangement::on_spinBox_HolesCount_editingFinished()
{
    holesTotalCount=ui->spinBox_HolesCount->value();
    ui->label_holesStart->setText("Enter hole 1 start adress");
    ui->label_HoleSize->setText("Enter hole 1 size");
//    ui->label_numberOfHoles->setText("number of holes =   "+QString::number(holesTotalCount));
    holesCount=1;
//    ui->label_holesStart->show();
//    ui->spinBox_holeStart->show();
//    ui->label_HoleSize->show();
//    ui->spinBox_holeSize->show();
//    ui->spinBox_HolesCount->hide();
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
        assign_hole(HoleStartAdress,holeSize); // assigning hole function
        // here we would pass it to the the functin hole.assign

        for (int i=HoleStartAdress;i<HoleStartAdress+holeSize;i++) {
           ui->listWidget_processes->item(i)->setText("hole ");
           ui->listWidget_processes->item(i)->setBackgroundColor(Qt::black);
           ui->listWidget_processes->item(i)->setForeground(Qt::white);
        }
        holesCount++;
        if (holesCount<=holesTotalCount)
        {
        ui->label_holesStart->setText("Enter hole "+QString::number(holesCount) +" start adress");
        ui->label_HoleSize->setText("Enter hole "+QString::number(holesCount) +" size");

        }
        }
//    if (holesCount>holesTotalCount)
//    {
//        ui->label_holesStart->hide();
//        ui->spinBox_holeStart->hide();
//        ui->label_HoleSize->hide();
//        ui->spinBox_holeSize->hide();
//        ui->label_numberOfProcess->show();
//        ui->spinBox_ProcessesCount->show();
//    }
    for(size_t var=0;var<memory.size();var++) {
        if (memory[var]!=-1 && memory[var]!=0)
        {
            ui->listWidget_processes->item(var)->setText("process "+QString::number(memory[var]));
            ui->listWidget_processes->item(var)->setTextAlignment(Qt::AlignHCenter);
            ui->listWidget_processes->item(var)->setBackgroundColor(Qt::blue);
        }
    }

}
void MemoryMangement::firstFit()
{
        sec_mem.resize(memory.size());
        int hole_index,flag=0;
        for(size_t i =0 ; i< memory.size();i++)
        {
            sec_mem[i]=memory[i];
        }
        int is_fit=0;
        int i = processesCount-1;
        for (int j = 0;j<process_list[i].seg_list.size();j++)
        {
           is_fit =0;

           for(size_t k = 0;k<hole_list.size();k++)
           {
               if (process_list[i].seg_list[j].limit <= hole_list[k].limit)
                  {
                    is_fit=1;
                    hole_index = k;
                    break;
                  }
           }

           if(is_fit == 1)
           {
               // add to best fit
              process_list[i].seg_list[j].base = hole_list[hole_index].base;
              process_list[i].seg_list[j].seg_end =  process_list[i].seg_list[j].base +  process_list[i].seg_list[j].limit -1;

              flag ++;
              for ( int s = hole_list[hole_index].base; s<hole_list[hole_index].base+process_list[i].seg_list[j].limit;s++)
              {
                  sec_mem[s] = process_list[i].process_id;
              }
               hole_list[hole_index].limit -= process_list[i].seg_list[j].limit;
               hole_list[hole_index].base += process_list[i].seg_list[j].limit;
           }
           else
           {
               break;
           }
        }

      if (flag == process_list[i].seg_list.size())
      {
        for (size_t m = 0;m<memory.size();m++)
        {
            if (memory[m] != sec_mem[m])
            {
                ui->listWidget_processes->item(m)->setText("process "+QString::number(sec_mem[m]));
                ui->listWidget_processes->item(m)->setBackgroundColor(Qt::white);
                ui->listWidget_processes->item(m)->setForeground(Qt::black);
                memory[m]=sec_mem[m];
            }
        }
      }
      else // add to best fit
        {
            for (size_t j=0; j < process_list[i].seg_list.size();j++)
            {
                process_list[i].seg_list[j].base = NULL;
                process_list[i].seg_list[j].seg_end = NULL;
            }

            QMessageBox::StandardButton replay= QMessageBox::question(this,"Process does not fit","Process does not fit delete?",QMessageBox::Yes|QMessageBox::Save);
            if (replay==QMessageBox::Save)
            {
                qDebug() <<" save";
                // move process to pending if not fit
                pending_process.push_back(process_list[i]);
            }

            vector<Segment>::iterator itseg;
            size_t size_seg = process_list[i].seg_list.size();
            for (size_t j=0; j < size_seg;j++)
            {
                itseg = process_list[i].seg_list.begin();
                itseg += j;
                process_list[i].seg_list.erase(itseg);
            }

            vector<Process>::iterator itprocess;
            itprocess = process_list.begin();
            itprocess+= i;
            process_list.erase(itprocess);
            processesCount--;
            processesTotalCount--;
        }
      processesCount++;

}

void MemoryMangement::bestFit()
{
    sec_mem.resize(memory.size());
    for (size_t i = 0; i < memory.size();i++)
    {
        sec_mem[i]=memory[i];
    }
        size_t proccesesIndex = processesCount-1;
        size_t flag = 0;
        Hole h1;
        int hole_index = 0;
        for (size_t segmentIndex = 0;segmentIndex < process_list[proccesesIndex].seg_list.size();segmentIndex++)
        {
            h1.limit = 9999999;
            for (size_t currentHoleIndex = 0;currentHoleIndex < hole_list.size();currentHoleIndex++)
            {
                if (process_list[proccesesIndex].seg_list[segmentIndex].limit <= hole_list[currentHoleIndex].limit && h1.limit > hole_list[currentHoleIndex].limit)
                {
                        h1 = hole_list[currentHoleIndex];
                        hole_index = currentHoleIndex;
                }
            }
            if (h1.limit != 9999999)
            {
                // add to best fit
               process_list[proccesesIndex].seg_list[segmentIndex].base = hole_list[hole_index].base;
               process_list[proccesesIndex].seg_list[segmentIndex].seg_end =  process_list[proccesesIndex].seg_list[segmentIndex].base +  process_list[proccesesIndex].seg_list[segmentIndex].limit -1;
                flag++;
                for (int s = hole_list[hole_index].base; s < hole_list[hole_index].base+ process_list[proccesesIndex].seg_list[segmentIndex].limit;s++)
                {
                    sec_mem[s] = process_list[proccesesIndex].process_id;
                }
               // qDebug() << QString::number(hole_list[hole_index].limit);
                hole_list[hole_index].limit -= process_list[proccesesIndex].seg_list[segmentIndex].limit;
                qDebug() << QString::number(hole_list[hole_index].limit);
                hole_list[hole_index].base += process_list[proccesesIndex].seg_list[segmentIndex].limit;
            }
            else
            {
                break;
            }

        }

        if (flag == process_list[proccesesIndex].seg_list.size())
        {
            for (size_t m = 0;m < memory.size();m++)
            {
                if (memory[m] != sec_mem[m])
                {
                    ui->listWidget_processes->item(m)->setText("process "+QString::number(sec_mem[m]));
                    ui->listWidget_processes->item(m)->setBackgroundColor(Qt::white);
                    ui->listWidget_processes->item(m)->setForeground(Qt::black);
                    memory[m]=sec_mem[m];
                }
            }
        }
        else
        {
            for (size_t j=0; j < process_list[proccesesIndex].seg_list.size();j++)
            {
                process_list[proccesesIndex].seg_list[j].base = NULL;
                process_list[proccesesIndex].seg_list[j].seg_end = NULL;
            }

            QMessageBox::StandardButton replay= QMessageBox::question(this,"Process does not fit","Process does not fit delete?",QMessageBox::Yes|QMessageBox::Save);
            if (replay==QMessageBox::Save)
            {
                qDebug() <<" save";
                 //move process to pending if not fit
                pending_process.push_back(process_list[proccesesIndex]);
            }
            vector<Segment>::iterator itseg;
            size_t size_seg = process_list[proccesesIndex].seg_list.size();
            for (size_t j=0; j < size_seg;j++)
            {
                itseg = process_list[proccesesIndex].seg_list.begin();
                itseg += j;
                process_list[proccesesIndex].seg_list.erase(itseg);
            }

            vector<Process>::iterator itprocess;
            itprocess = process_list.begin();
            itprocess+= proccesesIndex;
            process_list.erase(itprocess);
            processesCount--;
            processesTotalCount--;
        }
        processesCount++;

}


void MemoryMangement::on_radioButton_BestFit_clicked()
{
    bestfit=true;

}

void MemoryMangement::on_radioButton_FirstFit_clicked()
{
   bestfit=false;
}





void MemoryMangement::on_spinBox_ProcessesCount_editingFinished()
{
    processesTotalCount=ui->spinBox_ProcessesCount->value();
    processesTotalCount=processesTotalCount+processesCount-1;
    ui->label_segmentCount->setText("Enter process " + QString::number(processesCount)+" segments Count");
//    ui->label_segmentCount->show();
//    ui->spinBox_segmentsCount->show();
//    ui->label_segmentSize->show();
//    ui->spinBox_segmentSize->show();
    //initialize vector of processes here
}

void MemoryMangement::on_spinBox_segmentsCount_editingFinished()
{
    segmentTotalCount=ui->spinBox_segmentsCount->value();
    segmentsCount=1;
    ui->label_segmentSize->setText("Enter segment size");

}

void MemoryMangement::on_spinBox_segmentSize_editingFinished()
{
    if (segmentsCount<=segmentTotalCount)
    {
        ui->label_segmentSize->setText("Enter segment size");
        segmentsCount++;
        segmentSize=ui->spinBox_segmentSize->value();
        Segment s1 ={"segment "+QString::number(segmentsCount-1),segmentSize};
        seg_list.push_back(s1);
     }
    else if (segmentsCount>segmentTotalCount)
    {
        process_list.push_back({ processesCount,seg_list });
        //segment_table.insert(processesCount,{});
        bestfit?bestFit():firstFit();
        segmentsCount=1;
        seg_list.erase(seg_list.begin(),seg_list.end());
        if (processesCount<=processesTotalCount)
            {
                ui->label_segmentSize->setText("Enter segment size");
                ui->label_segmentCount->setText("Enter process "+QString::number(processesCount) +" segments Count");
//                on_spinBox_segmentsCount_editingFinished();
//                segmentTotalCount=ui->spinBox_segmentsCount->value();
            }
    }


}

void MemoryMangement::on_listWidget_processes_itemClicked(QListWidgetItem *item)
{
    int current_index=memory[ui->listWidget_processes->currentRow()];

    if (current_index!=0)
    {
        qDebug() << process_list[current_index-1].seg_list[0].segment_name;
        qDebug() << QString::number(process_list[current_index-1].seg_list[0].base);
        qDebug() << QString::number(process_list[current_index-1].seg_list[0].seg_end);
        ui->tableWidget->setRowCount(0);
        QTableWidgetItem *SegmentName,*SegmentStart,*SegmentEnd;
        for(int i=0;i<process_list[current_index-1].seg_list.size();i++)
        {
              SegmentName= new QTableWidgetItem(process_list[current_index-1].seg_list[i].segment_name);
              SegmentStart= new QTableWidgetItem(QString::number(process_list[current_index-1].seg_list[i].base));
              SegmentEnd= new QTableWidgetItem(QString::number(process_list[current_index-1].seg_list[i].seg_end));
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            qDebug() << QString::number(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, SegmentName);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, SegmentStart);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, SegmentEnd);
        }

        ui->tableWidget->show();
    }
    else {
        return;
    }

}
bool compare_holes(hole h1,hole h2)
{
    return (h1.base<h2.base);
}
void MemoryMangement::on_listWidget_processes_itemDoubleClicked(QListWidgetItem *item)
{
    ui->tableWidget->hide();
    int current_index=memory[ui->listWidget_processes->currentRow()];

    if (current_index!=0)
    {
    QMessageBox::information(this,"free process??","free process "+QString::number(current_index)+"??");

    int process_id=current_index-1;

        for (size_t seg_loop  = 0;seg_loop<process_list[process_id].seg_list.size();seg_loop++)
        {
            if ((memory[(process_list[process_id].seg_list[seg_loop].base)-1]) ==0 && (memory[(process_list[process_id].seg_list[seg_loop].base+process_list[process_id].seg_list[seg_loop].limit)]) != 0)
            {
                for (size_t hole_loop = 0;hole_loop < hole_list.size();hole_loop++)
                {
                    if (process_list[process_id].seg_list[seg_loop].base == hole_list[hole_loop].base+hole_list[hole_loop].limit)
                    {
                        hole_list[hole_loop].limit += process_list[process_id].seg_list[seg_loop].limit;
                        break;
                    }

                }
                for (size_t i = 0; i <= process_list[process_id].seg_list[seg_loop].seg_end; i++)
                {
                  if (memory[i] == process_list[process_id].process_id)
                       memory[i] = 0;
                }
            }
            else if ((memory[(process_list[process_id].seg_list[seg_loop].base)-1]) !=0 && (memory[(process_list[process_id].seg_list[seg_loop].base+process_list[process_id].seg_list[seg_loop].limit)]) == 0)
            {
                for (size_t hole_loop = 0;hole_loop < hole_list.size();hole_loop++)
                {
                    if ((process_list[process_id].seg_list[seg_loop].seg_end) +1 == hole_list[hole_loop].base)
                    {
                        hole_list[hole_loop].base = process_list[process_id].seg_list[seg_loop].base;
                        hole_list[hole_loop].limit += process_list[process_id].seg_list[seg_loop].limit;
                        break;
                    }

                }
                for (size_t i = 0; i <= process_list[process_id].seg_list[seg_loop].seg_end; i++)
                {
                  if (memory[i] == process_list[process_id].process_id)
                       memory[i] = 0;
                }
            }
             else if ((memory[(process_list[process_id].seg_list[seg_loop].base)-1]) ==0 && (memory[(process_list[process_id].seg_list[seg_loop].base+process_list[process_id].seg_list[seg_loop].limit)]) == 0)
            {
                for (size_t hole_loop = 0;hole_loop < hole_list.size();hole_loop++)
                {
                    if (process_list[process_id].seg_list[seg_loop].base == hole_list[hole_loop].base+hole_list[hole_loop].limit)
                    {
                        vector<Hole>::iterator it;
                        it = hole_list.begin();
                        it+= hole_loop+1;
                        hole_list[hole_loop].limit +=( process_list[process_id].seg_list[seg_loop].limit + hole_list[hole_loop+1].limit);
                        hole_list.erase(it);
                        break;
                    }

                }
                for (size_t i = 0; i <= process_list[process_id].seg_list[seg_loop].seg_end; i++)
                {
                  if (memory[i] == process_list[process_id].process_id)
                       memory[i] = 0;
                }
            }
             else if ((memory[(process_list[process_id].seg_list[seg_loop].base)-1]) !=0 && (memory[(process_list[process_id].seg_list[seg_loop].base+process_list[process_id].seg_list[seg_loop].limit)]) != 0)
             {
                 Hole h;
                 h.base = process_list[process_id].seg_list[seg_loop].base;
                 h.limit = process_list[process_id].seg_list[seg_loop].limit;
                 hole_list.push_back(h);
                 sort(hole_list.begin(),hole_list.end(),compare_holes);
                for (size_t i = 0; i <= process_list[process_id].seg_list[seg_loop].seg_end; i++)
                {
                  if (memory[i] == process_list[process_id].process_id)
                       memory[i] = 0;
                }
             }
        }
        for (int i = 0; i < Size; i++)
            {
                if (memory[i] == 0)
                {
                    ui->listWidget_processes->item(i)->setText("hole ");
                    ui->listWidget_processes->item(i)->setBackgroundColor(Qt::black);
                    ui->listWidget_processes->item(i)->setForeground(Qt::white);
                }
            }
    }
    else
    {
        QMessageBox::critical(this,"can not free holes","can not free holes");

    }
    if(!pending_process.empty())
    {
      for (size_t j = 0; j< pending_process.size();j++)
      {
        process_list.push_back(pending_process[j]);
      }
      bestfit?bestFit():firstFit();
      pending_process.erase(pending_process.begin(),pending_process.begin()+1);
    }
}
