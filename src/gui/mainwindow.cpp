#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QFrame>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>

#include <fstream>

#include "mainwindow.hpp"
#include "addolympiandialog.hpp"

#define DEFAULT_INFILE "winter-olympics.csv"
#define DEFAULT_OUTFILE "winter-olympics-out.csv"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ReadFile();
    SetupUi();
}

MainWindow::~MainWindow() = default;

void MainWindow::ReadFile()
{
    //try to open a file in the current running directory
    std::ifstream file(DEFAULT_INFILE);
    if (!file)
    {
        //prompt the user to open a file
        auto filename = QFileDialog::getOpenFileName(this,
            tr("Open Database"), "", tr(".csv files (*.csv)"));
        file = std::ifstream(filename.toUtf8().constData());
    }
    //if we still didn't open a file, just exit
    if (!file) exit(-1);

    m_database = std::make_unique<OlympianDatabase>(file);
}

void MainWindow::SetupUi()
{
    setWindowTitle(tr("Winter Olympian Database - De Anza College, CIS 22C, Group 5"));
    setGeometry(100, 100, 800, 600);

    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_statusBar->showMessage("Ready.");
    /*
     * Table
     */
    m_model = new OlympianTableModel(*m_database);
    m_tableView = new QTableView;
    m_tableView->setModel(m_model);
    //set some behaviors for our table
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    m_tableView->setSizeAdjustPolicy(QAbstractItemView::SizeAdjustPolicy::AdjustToContents);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /*
     * Tab widget for accessing and modifying the data
     */
    m_tabWidget = new QTabWidget;

    /*
     * Search and Sort
     */
    m_searchLine = new QLineEdit;
    m_searchBtn = new QPushButton(tr("Search"));
    connect(m_searchBtn, &QPushButton::clicked,
        this, &MainWindow::OnSearchButtonClicked);

    m_searchTypeCBox = new QComboBox;
    m_searchTypeCBox->addItems(QStringList()
                                 << "By Name"
                                 << "By Age"
                                 << "By Height");

    auto searchLayout = new QVBoxLayout;
    searchLayout->addWidget(m_searchLine);

    auto hSearch = new QHBoxLayout;
    hSearch->addWidget(m_searchBtn);
    hSearch->addWidget(m_searchTypeCBox);
    searchLayout->addLayout(hSearch);

    auto gbSearch = new QGroupBox(tr("Search for an entry"));
    gbSearch->setLayout(searchLayout);

    m_sortTypeCBox = new QComboBox;
    m_sortTypeCBox->addItems(QStringList()
                                 << "By Name"
                                 << "By Age"
                                 << "By Height");

    m_sortBtn = new QPushButton(tr("Sort"));
    connect(m_sortBtn, &QPushButton::clicked,
            this, &MainWindow::OnSortButtonClicked);

    auto sortLayout = new QFormLayout;
    auto hSort = new QHBoxLayout;
    hSort->addWidget(m_sortBtn);
    hSort->addWidget(m_sortTypeCBox);
    sortLayout->addRow(hSort);
    auto gbSort = new QGroupBox(tr("Display all and sort"));
    gbSort->setLayout(sortLayout);
    auto snsFrame = new QFrame;
    auto vSearchAndSort = new QVBoxLayout;
    vSearchAndSort->addWidget(gbSearch);
    vSearchAndSort->addWidget(gbSort);
    snsFrame->setLayout(vSearchAndSort);
    m_tabWidget->addTab(snsFrame, "Search / Sort");

    /*
     * Edit
     */
    m_addBtn = new QPushButton(tr("Add new data"));
    m_addBtn->setMinimumHeight(100);
    connect(m_addBtn, &QPushButton::clicked,
            this, &MainWindow::OnAddNewButtonClicked);

    m_deleteBtn = new QPushButton(tr("Delete selected entries"));
    m_deleteBtn->setMinimumHeight(100);
    connect(m_deleteBtn, &QPushButton::clicked,
        this, &MainWindow::OnDeleteButtonClicked);

    m_undoDeleteBtn = new QPushButton(tr("Undo delete"));
    m_undoDeleteBtn->setMinimumHeight(50);
    connect(m_undoDeleteBtn, &QPushButton::clicked,
            this, &MainWindow::OnUndoDeleteButtonClicked);

    auto controlsLayout = new QVBoxLayout;
    controlsLayout->addWidget(m_addBtn);
    controlsLayout->addWidget(m_deleteBtn);
    controlsLayout->addWidget(m_undoDeleteBtn);
    auto groupBox = new QGroupBox(tr("Controls"));
    groupBox->setLayout(controlsLayout);

    auto gbEdit = new QGroupBox(tr("Add and remove data"));
    gbEdit->setLayout(controlsLayout);
    m_tabWidget->addTab(gbEdit, "Edit");

    /*
     * Master Layout
     */
    auto masterLayout = new QVBoxLayout;
    auto masterHLayout = new QHBoxLayout;
    masterHLayout->addWidget(m_tableView);
    masterHLayout->addWidget(m_tabWidget);
    //the two columns have 10:1 stretch size ratio
    masterHLayout->setStretch(0, 10);
    masterHLayout->setStretch(1, 1);

    masterLayout->addLayout(masterHLayout);

    auto frame = new QFrame;
    frame->setLayout(masterLayout);
    setCentralWidget(frame);
}
void MainWindow::OnSearchButtonClicked()
{
    std::string searchTerm = m_searchLine->text().toStdString();
    switch(m_searchTypeCBox->currentIndex())
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
void MainWindow::OnSortButtonClicked()
{
    switch(m_sortTypeCBox->currentIndex())
    {
    default:
    case 0:
        m_model->setSortByName();
        statusBar()->showMessage(tr("Sorted by name."));
        break;
    case 1:
        m_model->setSortByAge();
        statusBar()->showMessage(tr("Sorted by age."));
        break;
    case 2:
        m_model->setSortByHeight();
        statusBar()->showMessage(tr("Sorted by height."));
        break;
    }
}
void MainWindow::OnDeleteButtonClicked()
{
    if (m_tableView->selectionModel()->hasSelection()) 
    {
        QModelIndexList rows = m_tableView->selectionModel()->selectedRows(); // return selected row(s)
        for (auto& row : rows)
        {
            QString qOlyName = row.data(0).toString(); // this row's name
            std::string olyName = qOlyName.toStdString(); 
            if (m_database->remove(olyName))
            {
                //delete was successful
                m_model->removeRow(row.row());
                QString message = QString("Deleted ") + qOlyName;
                m_statusBar->showMessage(message);
            }
            else
            {
                QMessageBox::warning(this, "Could not delete!",
                                     "Could not delete Olympian " + qOlyName + "!");
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "No Item Selected!", "You must select item(s) to delete.");
    }
}
void MainWindow::OnUndoDeleteButtonClicked()
{
    if (auto oly = m_model->undoDelete())
    {
        auto message = QString("Un-deleted ") + QString::fromStdString(oly->getName()) + "!";
        m_tableView->update();
        m_statusBar->showMessage(message);
    }
    else
    {
        QMessageBox::information(this, "No delete left", "Nothing left to undo!");
    }
}

void MainWindow::OnAddNewButtonClicked()
{
    auto dialog = new AddOlympianDialog(*m_database);
    dialog->show();
}

void MainWindow::SearchByName(const std::string& name)
{
    if (auto item = m_database->searchByName(name))
    {
        Vector<Olympian*> v;
        v.add(item);
        m_model->resetModel(v);
        auto message = QString("Found an entry for ")
            + QString::fromStdString(item->getName()) + QString(" !");
        m_statusBar->showMessage(message);

    }
    else
    {
        QMessageBox::warning(this, "No Results Found!", "No results found.");
    }
}
void MainWindow::SearchByAge(const std::string &age)
{
    try
    {
        //add the search results into the display
        Vector<Olympian*> vec;
        if (m_database->searchByAge(std::stoi(age), vec))
        {
            m_model->resetModel(vec);
            auto message = QString("Found ")
                + QString::number(vec.size()) + QString(" entries!");
            statusBar()->showMessage(message);
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
        if (m_database->searchByHeight(std::stoi(height), vec))
        {
            m_model->resetModel(vec);
            auto message = QString("Found ")
                + QString::number(vec.size()) + QString(" entries!");
            statusBar()->showMessage(message);
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