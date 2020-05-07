#ifndef MEMORYMANGEMENT_H
#define MEMORYMANGEMENT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MemoryMangement; }
QT_END_NAMESPACE

class MemoryMangement : public QMainWindow
{
    Q_OBJECT

public:
    MemoryMangement(QWidget *parent = nullptr);
    ~MemoryMangement();

private slots:
    void on_spinBox_memorySize_editingFinished();

    void on_spinBox_HolesCount_editingFinished();

    void on_spinBox_holeSize_editingFinished();

    void on_spinBox_holeStart_editingFinished();

    void on_radioButton_BestFit_clicked();

    void on_radioButton_FirstFit_clicked();


    void on_listWidget_processes_doubleClicked(const QModelIndex &index);

    void on_listWidget_processes_clicked(const QModelIndex &index);

    void on_spinBox_ProcessesCount_editingFinished();

    void on_spinBox_segmentsCount_editingFinished();

    void on_spinBox_segmentSize_editingFinished();

private:
    Ui::MemoryMangement *ui;
};
#endif // MEMORYMANGEMENT_H
