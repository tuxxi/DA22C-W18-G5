#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTabWidget>

#include <memory>

#include "../OlympicDatabase.h"
#include "olympianmodel.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void ReadFile();
    //implement this function later
    //void resizeEvent(QResizeEvent * event) override;

private slots:
    void OnSearchButtonClicked();
    void OnSortButtonClicked();
    void OnAddNewButtonClicked();
    void OnDeleteButtonClicked();
    void OnUndoDeleteButtonClicked();

private:
    void SearchByName(const std::string& name);
    void SearchByAge(const std::string& age);
    void SearchByHeight(const std::string& height);

    void SetupUi();
    QTabWidget* m_tabWidget;
    QComboBox* m_searchTypeCBox, *m_sortTypeCBox;
    QLineEdit* m_searchLine;
    QPushButton* m_searchBtn, *m_sortBtn;
    QPushButton* m_addBtn, *m_deleteBtn, *m_undoDeleteBtn;

    OlympianTableModel* m_model;
    QStatusBar* m_statusBar;
    QTableView* m_tableView;
    std::unique_ptr<OlympicDatabase> m_database;

};
