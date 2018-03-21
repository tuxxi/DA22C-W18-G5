#include <QFileDialog>
#include <QStatusBar>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QLabel>

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
    //if we still didn't open a file, just exit
    if (!file) exit(-1);

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

    auto label = new QLabel(tr("Search for an entry: "));
    searchLine = new QLineEdit;
    searchBtn = new QPushButton(tr("Search"));
    connect(searchBtn, &QPushButton::clicked,
        this, &MainWindow::OnSearchButtonClicked);
    searchTypeCBox = new QComboBox;

    searchTypeCBox->addItems(QStringList()
                                 << "By Name"
                                 << "By Age"
                                 << "By Height");

    auto searchLayout = new QHBoxLayout;
    searchLayout->addWidget(label);
    searchLayout->addWidget(searchLine);
    searchLayout->addSpacing(10);
    searchLayout->addWidget(searchBtn);
    searchLayout->addWidget(searchTypeCBox);
    auto gridlayout = new QGridLayout;
    gridlayout->addLayout(searchLayout, 0, 0);
    gridlayout->addWidget(tableView, 1, 0);

    auto frame = new QFrame;
    frame->setLayout(gridlayout);
    setCentralWidget(frame);
}
void MainWindow::OnSearchButtonClicked()
{
    std::string searchTerm = searchLine->text().toStdString();
    switch(searchTypeCBox->currentIndex())
    {
    default:
    case 0:
        SearchByName(searchTerm);
        break;
    case 1:
        SearchByAge(searchTerm);
        break;
    case 2:
        SearchByHeight(searchTerm);
        break;
    }
}
void MainWindow::SearchByName(const std::string& name)
{

}
void MainWindow::SearchByAge(const std::string &age)
{
    try
    {
        //add the search results into the display
        Vector<Olympian*> vec;
        if (database->searchByAge(std::stoi(age), vec))
        {
            model->resetModel(vec);
        }
        else
        {
            QMessageBox::warning(this, "No Results Found!", "No results found.");
        }
    }
    catch(...)
    {
        std::cerr << "Did not enter a number.";
        QMessageBox::warning(this, "Input Error", "You did not enter a number!");
    }
}
void MainWindow::SearchByHeight(const std::string &height)
{
    try
    {
        //add the search results into the display
        Vector<Olympian*> vec;
        if (database->searchByHeight(std::stoi(height), vec))
        {
            model->resetModel(vec);
        }
        else
        {
            QMessageBox::warning(this, "No Results Found!", "No results found.");
        }
    }
    catch(...)
    {
        std::cerr << "Did not enter a number.";
        QMessageBox::warning(this, "Input Error", "You did not enter a number!");
    }
}
