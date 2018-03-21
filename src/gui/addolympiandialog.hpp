#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "../OlympianDatabase.h"

class AddOlympianDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddOlympianDialog(OlympianDatabase& database, QWidget* parent = nullptr);
    void OnOkButtonClicked();
    void OnCancelButtonClicked();
private:
    Olympian* CreateOlympian() const;
    OlympianDatabase& m_database;

    QPushButton* m_OKBtn, *m_cancelBtn;
    QLineEdit* m_nameLine, *m_stateLine, *m_sportLine;
    QComboBox* m_genderCBox;
    QSpinBox* m_numGoldSBox, *m_numSilverSBox, *m_numBronzeSBox;
    QSpinBox* m_heightSBox, *m_ageSBox;
    void SetupUi();
};
