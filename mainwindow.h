#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "enterchar.h"
#include <vector>
#include <QMainWindow>
#include <windows.h>
#include <fstream>
#include <thread>
#include <QMenu>
#include <iostream>
#include <filesystem>
#include <vector>
#include <tuple>

namespace fs = std::filesystem;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::function<void(void)> FuncName = [this](){DataNewer();};
public slots:
    void getChar(char a, int N);
private slots:
    void on_cteateThread_clicked();

    void DataNewerClock(std::function<void(void)> a);
    void addTableInfo(int i);
    void DataNewer();
    std::string beautifulShow(int i);
    int getTime(int i);
    std::string showPriority(int i);
    void CreateThread2(int n);
    void killPr();
    void suspendPr();
    void startPr();
    void idlePr();
    void belowPr();
    void normPr();
    void abovePr();
    void highPr();
    void realtPr();
    int findProcessByID(int Row);
    void on_chb1_stateChanged(int arg1);
    void on_chb2_stateChanged(int arg1);
    void on_chb4_stateChanged(int arg1);
    void on_chb8_stateChanged(int arg1);
    void on_chb16_stateChanged(int arg1);
    void on_startAll_clicked();

private:
    Ui::MainWindow *ui;
    EnterChar* ec;
    QMenu *contextMenu;
    QAction *killAct;
    QAction *suspendAct;
    QAction *startAct;
    QMenu *changePrio;
    QAction *idleAct;
    QAction *normalAct;
    QAction *belowAct;
    QAction *aboveAct;
    QAction *realtAct;
    QAction *highAct;
      int rightClickedRow;
    void showContextMenu(const QPoint &pos);
  signals:
    void returnN(int n);
};
#endif // MAINWINDOW_H

