#pragma once
#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

#include <memory>

#include "../OlympianDatabase.h"
#include "olympianmodel.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void ReadFile();
private slots:
    void OnSearchButtonClicked();
    void OnSortButtonClicked();
private:
    void SearchByName(const std::string& name);
    void SearchByAge(const std::string& age);
    void SearchByHeight(const std::string& height);

    void SetupUi();
    QComboBox* searchTypeCBox, *sortTypeCBox;
    QLineEdit* searchLine;
    QPushButton* searchBtn, *sortBtn;

    OlympianTableModel* model;
    QStatusBar* statusBar;
    QTableView* tableView;
    std::unique_ptr<OlympianDatabase> database;

};
