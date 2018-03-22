#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "../OlympicDatabase.h"
#include <QComboBox>

class AddOlympianDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddOlympianDialog(OlympicDatabase& database, QWidget* parent = nullptr);
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
private:
    Olympian* CreateOlympian() const;
    OlympicDatabase& m_database;

    QPushButton* m_OKBtn, *m_cancelBtn;
    QLineEdit* m_nameLine, *m_stateLine, *m_sportLine;
    QComboBox* m_genderCBox;
    QSpinBox* m_numGoldSBox, *m_numSilverSBox, *m_numBronzeSBox;
    QSpinBox* m_heightSBox, *m_ageSBox;
    void SetupUi();
};
