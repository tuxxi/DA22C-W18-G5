#pragma once
#include <QMainWindow>
#include <QTableView>

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
private:
    void SetupUi();

    OlympianTableModel* model;
    QStatusBar* statusBar;
    QTableView* tableView;
    std::unique_ptr<OlympianDatabase> database;

};
