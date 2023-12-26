#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <QTimer>
#include <functional>
#include<QMenu>
LONG g_interlock = NULL;
std::vector<HANDLE> arr;
HANDLE WINAPI mut = CreateMutex(NULL, FALSE, (LPCTSTR)"mutex");
HANDLE g_semaphore = CreateSemaphore(NULL, 1, 18, NULL);
std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> times;
std::vector<char> ch = {(char)0,(char)0,(char)0,(char)0,(char)0};

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    DataNewerClock(FuncName);
   // threadFunction('4');
   // threadFunction('a');
    ui->setupUi(this);
    ec = new EnterChar(0);
    connect(ec, &EnterChar::returnChar, this, &MainWindow::getChar);
    ui->TableView->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu = new QMenu(this);
    killAct = new QAction("Kill", this);
    suspendAct = new QAction("Suspend", this);
    startAct = new QAction("Resume", this);

    changePrio = new QMenu("Change priority");
    idleAct = new QAction("IDLE_PRIORITY_CLASS", this);
    belowAct = new QAction("BELOW_NORMAL_PRIORITY_CLASS", this);
    normalAct = new QAction("NORMAL_PRIORITY_CLASS", this);
    aboveAct = new QAction("ABOVE_NORMAL_PRIORITY_CLASS", this);
    highAct = new QAction("HIGH_PRIORITY_CLASS", this);
    realtAct = new QAction("REAL_TIME_PRIORITY_CLASS", this);

    contextMenu->addAction(killAct);
    contextMenu->addAction(suspendAct);
    contextMenu->addAction(startAct);
    contextMenu->addMenu(changePrio);
    changePrio->addAction(idleAct);
    changePrio->addAction(belowAct);
    changePrio->addAction(normalAct);
    changePrio->addAction(aboveAct);
    changePrio->addAction(highAct);
    changePrio->addAction(realtAct);

    connect(ui->TableView, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(killAct, &QAction::triggered, this, &MainWindow::killPr);
    connect(suspendAct, &QAction::triggered, this, &MainWindow::suspendPr);
    connect(startAct, &QAction::triggered, this, &MainWindow::startPr);
    connect(idleAct, &QAction::triggered, this, &MainWindow::idlePr);
    connect(belowAct, &QAction::triggered, this, &MainWindow::belowPr);
    connect(normalAct, &QAction::triggered, this, &MainWindow::normPr);
    connect(aboveAct, &QAction::triggered, this, &MainWindow::abovePr);
    connect(highAct, &QAction::triggered, this, &MainWindow::highPr);
    connect(realtAct, &QAction::triggered, this, &MainWindow::realtPr);
}
struct ThreadData{
    char a;
    int N;
    int start;
};
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::getChar(char a, int N){
    ch[N] = a;
}
DWORD WINAPI threadFunction_2(LPVOID lpParam){
    struct ThreadData* data = (struct ThreadData*)lpParam;
    //    WaitForSingleObject(arr[0], INFINITE);
    //WaitForSingleObject(arr[0], 100);
    char a = data->a;
    int N = data->N;
    int start = data->start;
      Sleep(100);
    std::ifstream fp("D:\\LABS_ALGORITHMS\\OC_FILES\\Lab5\\Laab5\\Text.txt");
    if (!fp.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
        return 1; }
    int count = 0;
    fp.seekg(start);
    //char curr;
    int n = 0;

    std::string line;
    while (std::getline(fp, line)) {
        for (char c : line) {
            if(a == c){
                count++;
            }
        }
        if(n == N){break;}
        n++;
    }

      fp.close();

    std::cout << "Count of '" << a << "' symbols in range: " << count << std::endl;
      getchar();
    return 0;
}

DWORD WINAPI threadFunction_interlock(LPVOID lpParam){
    struct ThreadData* data = (struct ThreadData*)lpParam;
    //InterlockedExchangeAdd(&g_interlock, 1);
   // while(InterlockedExchangeAdd(&g_interlock, 1) == 1)
    //HANDLE WINAPI m = OpenMutex(SYNCHRONIZE, FALSE, (LPCTSTR)"mutex");
     WaitForSingleObject(mut, INFINITE);
        Sleep(0);

        char a = data->a;
        int N = data->N;
        int start = data->start;
          //Sleep(100);
    std::ifstream fp("D:\\LABS_ALGORITHMS\\OC_FILES\\Lab5\\Laab5\\Text.txt");
    if (!fp.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
        return 1; }
    int count = 0;
    fp.seekg(start);
    char curr;
    int n = 0;

    std::string line;
    while (std::getline(fp, line)) {
        for (char c : line) {
            if(a == c){
                count++;
            }
        }
        if(n == N){break;}
        n++;
    }

    fp.close();

    std::cout << "Count of '" << a << "' symbols in range: " << count << std::endl;
    //getchar();
    //InterlockedExchange(&g_interlock, 0);
    //g_interlock = 0;
      ReleaseMutex(mut);
    return 0;
}


DWORD WINAPI ThreadFunction(LPVOID lpParam) {
      char a = ch[0];
    std::ifstream fp("D:\\LABS_ALGORITHMS\\OC_FILES\\Lab5\\Laab5\\Text.txt");
        Sleep(100);
    if (!fp.is_open()) {
        std::cerr << "Can't open file!" << std::endl;
        return 1; }
    int count = 0;

    char curr;
    while (fp.get(curr)) {
        if (curr == a) {
            count++;
        }
    }
    fp.close();

    std::cout << "Count of '" << a << "' symbols in file: " << count << std::endl;

    getchar();
    getchar();
    return 0;
}

DWORD WINAPI ThreadFunction_semaphore(LPVOID lpParam) {
    struct ThreadData* data = (struct ThreadData*)lpParam;
        char a = data->a;
        int N = data->N;
        int start = data->start;
             WaitForSingleObject(g_semaphore, INFINITE);
        std::ifstream fp("D:\\LABS_ALGORITHMS\\OC_FILES\\Lab5\\Laab5\\Text.txt");
        if (!fp.is_open()) {
            std::cerr << "Can't open file!" << std::endl;
            return 1; }
        int count = 0;
        fp.seekg(start);
        char curr;
        int n = 0;

        std::string line;
        while (std::getline(fp, line)) {
            for (char c : line) {
            if(a == c){
                count++;
            }
            }
            if(n == N){break;}
            n++;
        }

        fp.close();

        std::cout << "Count of '" << a << "' symbols in range: " << count << std::endl;
        getchar();
           ReleaseSemaphore(g_semaphore, 1, NULL);
    //  g_interlock = 0;
    return 0;
}

void MainWindow::CreateThread2(int n){
    std::ifstream fp("D:\\LABS_ALGORITHMS\\OC_FILES\\Lab5\\Laab5\\Text.txt");
    string line;
    int lineCount = 0;
    while (std::getline(fp, line)) {
        lineCount++;
    }
    fp.close();

    int u = 0;
        switch(n){
    case 2: u = 1; break;
    case 4: u = 2; break;
    case 8: u = 3; break;
    case 16: u = 4; break;
    default: u = 0; break;
    }


    for(int i = 0; i < n; i++){
        struct ThreadData *cjh = (struct ThreadData*)malloc( sizeof(struct ThreadData) );
        cjh->a = ch[u];
       cjh->N = lineCount/n;
        cjh->start = 0+i*(lineCount/n);
        HANDLE hThread;
        if(ui->interlock->isChecked()){
        hThread = CreateThread(
            NULL,
            0,
            threadFunction_interlock,
            cjh,
            CREATE_SUSPENDED,
            NULL
                );}
        if(ui->semaphore->isChecked()){
  //      HANDLE semaphore = CreateSemaphore(NULL, 1, 18, NULL);
        hThread = CreateThread(
            NULL,
            0,
            ThreadFunction_semaphore,
            cjh,
            CREATE_SUSPENDED,
            NULL
            );
        }

        if(!ui->semaphore->isChecked() && !ui->interlock->isChecked()){
        hThread = CreateThread(
            NULL,
            0,
            threadFunction_2,
            cjh,
            CREATE_SUSPENDED,
            NULL
            );
        }

        if(hThread){
        arr.push_back(hThread);
        times.push_back(std::chrono::high_resolution_clock::now());
   //     _InterlockedIncrement(&hThread);
        }
    }

}
void MainWindow::on_cteateThread_clicked()
{
    if(ui->chb1->isChecked()){
    HANDLE hThread = CreateThread(
        NULL,                   // Default security attributes
        0,                      // Default stack size
        ThreadFunction,         // Thread function
        NULL,                   // Thread parameters
        CREATE_SUSPENDED,                      // Default creation flags
        NULL                    // Receive thread identifier
        );
        if(hThread){
        arr.push_back(hThread);
        times.push_back(std::chrono::high_resolution_clock::now());}
    }

    if(ui->chb2->isChecked()){
         CreateThread2(2);
    }

    if(ui->chb4->isChecked()){
            CreateThread2(4);
    }
    if(ui->chb8->isChecked()){
            CreateThread2(8);
    }
    if(ui->chb16->isChecked()){
            CreateThread2(16);
    }


    ui->chb16->setCheckState(Qt::Unchecked);
    ui->chb1->setCheckState(Qt::Unchecked);
    ui->chb2->setCheckState(Qt::Unchecked);
    ui->chb4->setCheckState(Qt::Unchecked);
    ui->chb8->setCheckState(Qt::Unchecked);
//     counter = 0;
}

//void

void MainWindow::DataNewer(){
    for(int i = 0; i < arr.size(); i++){
    if(i == 0){
            ui->TableView->setRowCount(0);
    }
    DWORD exitCode;
    if (GetExitCodeThread(arr[i], &exitCode)) {
            if (exitCode == STILL_ACTIVE) {
                addTableInfo(i);} else {
                arr.erase(arr.begin() + i);
                times.erase(times.begin() + i);
                i--;
            }
    }
 //   }

}
}
void MainWindow::addTableInfo(int i){
    ui->TableView->insertRow(ui->TableView->rowCount());
    DWORD threadId = GetThreadId(arr[i]);
   // arr[i].renewData();
    ui->TableView->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(static_cast<int>(threadId)))));
        DWORD exitCode;
    GetExitCodeProcess(arr[i], &exitCode);
        ui->TableView->setItem(i, 1, new QTableWidgetItem(QString::fromStdString((exitCode == STILL_ACTIVE)?"Active":"Suspended")));

    ui->TableView->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(beautifulShow(i))));
        ui->TableView->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(showPriority(i))));

}

string MainWindow::showPriority(int i){
    switch(GetThreadPriority(arr[i])){
    case -15: return "IDLE";break;
    case -2: return "LOWEST";break;
    case -1: return "BELOW_NORMAL";break;
    case 0: return "NORMAL";break;
    case 1: return "ABOVE_NORMAL";break;
    case 2: return "HIGHEST";break;
    case 15: return "TIME_CRITICAL";break;
    default: return "NORMAL";break;
    }
     return "";
}

int MainWindow::getTime(int i)
{   auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - times[i]);
    return elapsed_time.count();
}

std::string MainWindow::beautifulShow(int i){
    int elapsed_time_seconds = getTime(i) / 1000; // Convert milliseconds to seconds
    int hours = elapsed_time_seconds / 3600;
    int minutes = (elapsed_time_seconds % 3600) / 60;
    int seconds = elapsed_time_seconds % 60;
    std::stringstream af;
    af <<  setfill('0') << setw(2) << hours << ":"
      << setfill('0') << setw(2) << minutes << ":" <<
        setfill('0') << setw(2) << seconds;

    return af.str();
}

void MainWindow::DataNewerClock(std::function<void(void)> a){
    int duration = 1000;
    std::thread([a, duration](){
        while(true){
            a();
            auto ms = std::chrono::steady_clock::now() + std::chrono::milliseconds(duration);
            std::this_thread::sleep_until(ms);
        }
    }).detach();

}

int MainWindow::findProcessByID(int r){
    QString te = ui->TableView->item(r, 0)->text();
    for(int i = 0; i < arr.size(); i++){
    if(static_cast<DWORD>(stoi(te.toStdString())) == GetThreadId(arr[i])){
            return i;
    }
    }
    return -1;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    rightClickedRow = ui->TableView->indexAt(pos).row();
    ui->TableView->selectRow(rightClickedRow);// Store the right-clicked row
    contextMenu->exec(ui->TableView->mapToGlobal(pos));
}

void MainWindow::killPr()
{
    TerminateThread(arr[rightClickedRow], 0);
}

void MainWindow::suspendPr()
{
    SuspendThread(arr[rightClickedRow]);
}

void MainWindow::startPr()
{
    ResumeThread(arr[rightClickedRow]);
}
void MainWindow::idlePr(){
    SetThreadPriority(arr[rightClickedRow], THREAD_PRIORITY_IDLE);
}
void MainWindow::belowPr(){
     SetThreadPriority(arr[rightClickedRow], THREAD_PRIORITY_BELOW_NORMAL);
}
void MainWindow::normPr(){
      SetThreadPriority(arr[rightClickedRow], THREAD_PRIORITY_NORMAL);
}
void MainWindow::abovePr(){
       SetThreadPriority(arr[rightClickedRow], THREAD_PRIORITY_ABOVE_NORMAL);
}
void MainWindow::highPr(){
        SetThreadPriority(arr[rightClickedRow], THREAD_PRIORITY_HIGHEST);
}
void MainWindow::realtPr(){
         SetThreadPriority(arr[rightClickedRow], THREAD_PRIORITY_TIME_CRITICAL);
}

void MainWindow::on_chb1_stateChanged(int arg1)
{
         if(arg1 == 2){
             ec->getN(0);
             ec->show();
         }
}

void MainWindow::on_chb2_stateChanged(int arg1)
{
         if(arg1 == 2){
             ec->getN(1);
             ec->show();
         }
}

void MainWindow::on_chb4_stateChanged(int arg1)
{
         if(arg1 == 2){
             ec->getN(2);
             ec->show();
         }
}

void MainWindow::on_chb8_stateChanged(int arg1)
{
         if(arg1 == 2){
             ec->getN(3);
             ec->show();
         }
}

void MainWindow::on_chb16_stateChanged(int arg1)
{
         if(arg1 == 2){
             ec->getN(4);
             ec->show();
         }
}

void MainWindow::on_startAll_clicked()
{
         for(int i = 0; i < arr.size(); i++){
             ResumeThread(arr[i]);
         }
}

