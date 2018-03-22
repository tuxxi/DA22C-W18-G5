#include <QFormLayout>
#include <QComboBox>

#include <QPushButton>

#include "addolympiandialog.hpp"

AddOlympianDialog::AddOlympianDialog(OlympicDatabase& database, QWidget* parent)
    : QDialog(parent), m_database(database)
{
    SetupUi();
    setAttribute(Qt::WA_DeleteOnClose);
}


void AddOlympianDialog::OnOkButtonClicked()
{
    m_database.insert(CreateOlympian());
    close();
}

void AddOlympianDialog::OnCancelButtonClicked()
{
    close();
}

Olympian* AddOlympianDialog::CreateOlympian() const
{
    return new Olympian(
        m_nameLine->text().toStdString(),
        m_sportLine->text().toStdString(),
        m_stateLine->text().toStdString(),
        m_genderCBox->currentText().toStdString()[0],
        m_ageSBox->value(),
        m_heightSBox->value(),
        m_numGoldSBox->value(),
        m_numSilverSBox->value(),
        m_numBronzeSBox->value()
    );
}

void AddOlympianDialog::SetupUi()
{
    setWindowTitle("Add New Olympian");

    auto layout = new QFormLayout;
    layout->setVerticalSpacing(5);
    const auto validator = new QRegExpValidator(QRegExp("^[a-zA-Z ]*$"), this); //regex for only alpha + space
    layout->addRow(QString("Name: "), m_nameLine = new QLineEdit);
    m_nameLine->setValidator(validator); 
    layout->addRow(QString("Sport: "), m_sportLine = new QLineEdit);
    m_sportLine->setValidator(validator);
    layout->addRow(QString("State: "), m_stateLine = new QLineEdit);
    m_stateLine->setValidator(new QRegExpValidator(QRegExp("^[A-Z]*$"), this)); //regex for only uppercase alpha

    m_stateLine->setMaxLength(2);
    layout->addRow(QString("Gender: "), m_genderCBox = new QComboBox);
    m_genderCBox->addItems(QStringList() << "Male" << "Female" << "Other"); //add M F and O to genders

    layout->addRow(QString("Age (years)"), m_ageSBox = new QSpinBox);
    m_ageSBox->setRange(14, 50);
    layout->addRow(QString("Height (inches): "), m_heightSBox = new QSpinBox);
    m_heightSBox->setRange(36, 96);

    layout->addRow(QString("# Gold Medals: "), m_numGoldSBox = new QSpinBox);
    layout->addRow(QString("# Silver Medals: "), m_numSilverSBox = new QSpinBox);
    layout->addRow(QString("# Bronze Medals: "), m_numBronzeSBox = new QSpinBox);

    auto hButtons = new QHBoxLayout;
    hButtons->addWidget(m_OKBtn = new QPushButton("Add"));
    connect(m_OKBtn, &QPushButton::clicked,
        this, &AddOlympianDialog::OnOkButtonClicked);
    hButtons->addWidget(m_cancelBtn = new QPushButton("Cancel"));
    connect(m_cancelBtn, &QPushButton::clicked,
        this, &AddOlympianDialog::OnCancelButtonClicked);

    layout->addItem(new QSpacerItem(0, 10)); //add some spacing 
    layout->addRow(hButtons);
    setLayout(layout);
}
