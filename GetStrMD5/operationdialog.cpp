#include <QtWidgets>
#include "operationdialog.h"
#include <QCryptographicHash>

OperationDialog::OperationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("获取字符串MD5"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(QMargins(5,5,5,5));
    vboxLayout->setSizeConstraint(QLayout::SetFixedSize);

    tipsLabel=new QLabel(tr("获取字符串MD5"));
    vboxLayout->addWidget(tipsLabel);

    oldTextEdit=new QTextEdit();
    oldTextEdit->setPlaceholderText(tr("原始字符串"));

    vboxLayout->addWidget(oldTextEdit);
    {
        getmd5Button = new QPushButton(tr("获取字符串MD5"));
        getmd5Button->setObjectName(QString::fromUtf8("getmd5Button"));

        QHBoxLayout* hboxLayout=new QHBoxLayout();
        hboxLayout->addWidget(getmd5Button);

        vboxLayout->addLayout(hboxLayout);
    }

    newTextEdit=new QTextEdit();
    newTextEdit->setPlaceholderText(tr("这里是生成的md5"));
    newTextEdit->setEnabled(false);
    vboxLayout->addWidget(newTextEdit);


    compareButton = new QPushButton(tr("对比"));
    compareButton->setObjectName(QString::fromUtf8("compareButton"));
    compareButton->setEnabled(false);
    vboxLayout->addWidget(compareButton);

    compareTextEdit=new QTextEdit();
    compareTextEdit->setPlaceholderText(tr("这里填用来对比的md5"));
    compareTextEdit->setEnabled(false);
    vboxLayout->addWidget(compareTextEdit);

    setLayout(vboxLayout);

    this->setObjectName(QString::fromUtf8("OperationDialog"));
    QMetaObject::connectSlotsByName(this);
}


void OperationDialog::on_getmd5Button_clicked()
{

    QString tmpSrcStr=this->oldTextEdit->toPlainText();
    if(tmpSrcStr=="")
    {
        return;
    }
    QByteArray tmpSrcByteArray=tmpSrcStr.toUtf8();

    QCryptographicHash tmpCryptographicHash(QCryptographicHash::Md5);
    tmpCryptographicHash.addData(tmpSrcByteArray);

    QByteArray tmpDstByteArray=tmpCryptographicHash.result();
    QString tmpDstStr=tmpDstByteArray.toHex();

    this->newTextEdit->setText(tmpDstStr);

    this->newTextEdit->setEnabled(true);
    this->compareButton->setEnabled(true);
    this->compareTextEdit->setEnabled(true);

}

void OperationDialog::on_compareButton_clicked()
{
    if(this->newTextEdit->toPlainText()==this->compareTextEdit->toPlainText())
    {
        QMessageBox::about(this, "对比结果", "相同");
    }
    else
    {
        QMessageBox::about(this, "对比结果", "不同");
    }
}


OperationDialog::~OperationDialog()
{
}

