#include <QtWidgets>
#include "operationdialog.h"

OperationDialog::OperationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Unicode2Chinese"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(QMargins(5,5,5,5));
    vboxLayout->setSizeConstraint(QLayout::SetFixedSize);

    tipsLabel=new QLabel(tr("Unicode编码的中文像这样:\\u4eba\\u751f\\u82e6\\u77ed\\uff0cpy\\u662f\\u5cb8"));
    vboxLayout->addWidget(tipsLabel);

    oldTextEdit=new QTextEdit();

    vboxLayout->addWidget(oldTextEdit);



    //unicode转中文
    {
        unicode2chineseButton = new QPushButton(tr("unicode转中文"));
        unicode2chineseButton->setObjectName(QString::fromUtf8("unicode2chineseButton"));



        QHBoxLayout* hboxLayout=new QHBoxLayout();
        hboxLayout->addWidget(unicode2chineseButton);


        vboxLayout->addLayout(hboxLayout);
    }



    newTextEdit=new QTextEdit();

//    newTextEdit->setEnabled(false);


    vboxLayout->addWidget(newTextEdit);

    setLayout(vboxLayout);

    this->setObjectName(QString::fromUtf8("OperationDialog"));
    QMetaObject::connectSlotsByName(this);
}


void OperationDialog::on_unicode2chineseButton_clicked()
{

    QString filename = this->oldTextEdit->toPlainText();
    do {
        int idx = filename.indexOf("\\u");
        QString strHex = filename.mid(idx, 6);
        strHex = strHex.replace("\\u", QString());
        int nHex = strHex.toInt(0, 16);
        filename.replace(idx, 6, QChar(nHex));
    } while (filename.indexOf("\\u") != -1);


    this->newTextEdit->setText(filename);
//    QClipboard *board = QApplication::clipboard();
//    board->setText(this->newTextEdit->toPlainText());
}


OperationDialog::~OperationDialog()
{
}

