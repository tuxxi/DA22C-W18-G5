#include <QFileDialog>
#include <QStatusBar>
#include <QGridLayout>
#include <QFrame>
#include <fstream>

#include "mainwindow.hpp"

#define DEFAULT_INFILE "winter-olympics.csv"
#define DEFAULT_OUTFILE "winter-olympics-out.csv"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ReadFile();
    SetupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::ReadFile()
{
    //try to open a file in the current running directory
    std::ifstream file(DEFAULT_INFILE);
    if (!file)
    {
        //propmt the user to open a file
        auto filename = QFileDialog::getOpenFileName(this,
            tr("Open Database"), "", tr(".csv files (*.csv)"));
        file = std::ifstream(filename.toUtf8().constData());
    }
    database = std::make_unique<OlympianDatabase>(file);
}

void MainWindow::SetupUi()
{
    setWindowTitle(tr("Winter Olympian Database - De Anza College, CIS 22C, Group 5"));
    setGeometry(100, 100, 800, 600);

    statusBar = new QStatusBar;
    setStatusBar(statusBar);
    statusBar->showMessage("Ready.");

    model = new OlympianTableModel(*database);
    tableView = new QTableView;
    tableView->setModel(model);

    auto gridlayout = new QGridLayout;
    gridlayout->addWidget(tableView);

    auto frame = new QFrame;
    frame->setLayout(gridlayout);
    setCentralWidget(frame);
}
