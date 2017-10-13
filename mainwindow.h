#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QComboBox>
#include<QGroupBox>
#include "toile.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Toile *const toile;
    void fillColorList(QComboBox *comboBox);
    QGroupBox *creatProperyBox();

    QComboBox* lineTypeBox;
    QComboBox* penCapStyleBox;
    QComboBox* penJoinStyleBox;
    QComboBox* colorComboBox;
    QSlider* widthSlider;

public slots:
    void openFile();
    void saveFile();
    void quitApp();
    void setPropertyPanel(QPen* pen);
};



#endif // MAINWINDOW_H
