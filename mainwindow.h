#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void enableGroupButton(bool enable);
    void enableUngroupButton(bool enable);
    void enableDeleteButton(bool enable);
    void enableUndoButton(bool enable);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_undoButton_clicked();

private:
    Ui::MainWindow *ui;
    Scene* scene;
};
#endif // MAINWINDOW_H
