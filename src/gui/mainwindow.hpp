#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTabWidget>
#include <QMenuBar>
#include <memory>

#include "../OlympicDatabase.h"
#include "olympianmodel.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void ReadFile(const QString& filename);
    void SaveFile(const QString& filename);

    //implement this function later
    //void resizeEvent(QResizeEvent * event) override;

private slots:
    void OnSearchButtonClicked();
    void OnSortButtonClicked();
    void OnAddNewButtonClicked();
    void OnDeleteButtonClicked();
    void OnUndoDeleteButtonClicked();

    //menu
    void OnMenuOpen();
    void OnMenuSave();
    void OnMenuQuit();
private:
    void SearchByName(const std::string& name);
    void SearchByAge(const std::string& age);
    void SearchByHeight(const std::string& height);

    void SetupUi();
    //ui elements
    QTabWidget* m_tabWidget;
    QComboBox* m_searchTypeCBox, *m_sortTypeCBox;
    QLineEdit* m_searchLine;
    QPushButton* m_searchBtn, *m_sortBtn;
    QPushButton* m_addBtn, *m_deleteBtn, *m_undoDeleteBtn;

    QTableView* m_tableView;
    OlympianTableModel* m_model;

    //general window elements
    QStatusBar* m_statusBar;
    QMenuBar* m_menuBar;
    QMenu* m_menuFile;
    QAction* m_actionSave, *m_actionOpen, *m_actionQuit;

    //actual database
    std::unique_ptr<OlympicDatabase> m_database;
};
