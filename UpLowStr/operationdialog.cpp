#include <QtWidgets>
#include "operationdialog.h"

OperationDialog::OperationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("字母转大小写"));

    oldTextEdit=new QTextEdit();
    oldTextEdit->setPlaceholderText(tr("GM_DarkCityBoss_Changed_Target_Request"));


    upButton = new QPushButton(tr("转大写"));
    upButton->setObjectName(QString::fromUtf8("upButton"));


    lowerButton = new QPushButton(tr("转小写"));
    lowerButton->setObjectName(QString::fromUtf8("lowerButton"));

    QHBoxLayout* hboxLayout=new QHBoxLayout();
    hboxLayout->addWidget(upButton);
    hboxLayout->addWidget(lowerButton);


    tipsLabel=new QLabel(tr("转换后，自动拷贝，直接黏贴使用"));

    newTextEdit=new QTextEdit();
    newTextEdit->setPlaceholderText(tr("GM_DARKCITYBOSS_CHANGED_TARGET_REQUEST"));
    newTextEdit->setEnabled(false);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(QMargins());
    vboxLayout->setSizeConstraint(QLayout::SetFixedSize);

    vboxLayout->addWidget(oldTextEdit);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(tipsLabel);
    vboxLayout->addWidget(newTextEdit);

    setLayout(vboxLayout);

    this->setObjectName(QString::fromUtf8("OperationDialog"));
    QMetaObject::connectSlotsByName(this);
}

void OperationDialog::on_upButton_clicked()
{
    this->newTextEdit->setText(this->oldTextEdit->toPlainText().toUpper());
    QClipboard *board = QApplication::clipboard();
    board->setText(this->newTextEdit->toPlainText());
}

void OperationDialog::on_lowerButton_clicked()
{
    this->newTextEdit->setText(this->oldTextEdit->toPlainText().toLower());
    QClipboard *board = QApplication::clipboard();
    board->setText(this->newTextEdit->toPlainText());
}


OperationDialog::~OperationDialog()
{
}

