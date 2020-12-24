#include <QGraphicsScene>

#include "mainwindow.h"
#include "scene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene(this);
    scene->setSceneRect(0, 0, 150, 150);
    ui->graphicsView->setScene(scene);

    QObject::connect(scene, &Scene::enableDeleteButton, this, &MainWindow::enableDeleteButton);
    QObject::connect(scene, &Scene::enableGroupButton, this, &MainWindow::enableGroupButton);
    QObject::connect(scene, &Scene::enableUngroupButton, this, &MainWindow::enableUngroupButton);
    QObject::connect(scene, &Scene::enableUndoButton, this, &MainWindow::enableUndoButton);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    scene->RemoveItems();
}

void MainWindow::on_pushButton_2_clicked()
{
    scene->GroupElements();
}

void MainWindow::on_pushButton_3_clicked()
{
    scene->UngroupElements();
}

void MainWindow::on_undoButton_clicked()
{
    scene->Undo();
}

void MainWindow::on_radioButton_clicked()
{
    scene->ReceiveFigure(ShapeType::kCircle);
}

void MainWindow::on_radioButton_2_clicked()
{
    scene->ReceiveFigure(ShapeType::kSquare);
}

void MainWindow::enableDeleteButton(bool enable) {
    ui->pushButton->setEnabled(enable);
}

void MainWindow::enableGroupButton(bool enable) {
    ui->pushButton_2->setEnabled(enable);

}

void MainWindow::enableUngroupButton(bool enable) {
    ui->pushButton_3->setEnabled(enable);
}

void MainWindow::enableUndoButton(bool enable) {
    ui->undoButton->setEnabled(enable);
}
