#include <QtWidgets>
#include "operationdialog.h"
#include <QCryptographicHash>

OperationDialog::OperationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("获取文件MD5"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(QMargins(5,5,5,5));
    vboxLayout->setSizeConstraint(QLayout::SetFixedSize);

    tipsLabel=new QLabel(tr("获取文件MD5"));
    vboxLayout->addWidget(tipsLabel);


    {
        getmd5Button = new QPushButton(tr("获取文件MD5"));
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

void OperationDialog::on_compareButton_clicked()
{
    if(this->newTextEdit->toPlainText().toLower()==this->compareTextEdit->toPlainText().toLower())
    {
        QMessageBox::about(this, "对比结果", "相同");
    }
    else
    {
        QMessageBox::about(this, "对比结果", "不同");
    }
}

void OperationDialog::on_getmd5Button_clicked()
{
    QString strFile = QFileDialog::getOpenFileName(this, tr("选择文件"), "./","*.*");
    if(strFile=="")
    {
        qDebug()<<"not choose file";
        return;
    }
    QString tmpMD5Str=this->getFileMD5(strFile);

    this->newTextEdit->setText(tmpMD5Str);

    this->newTextEdit->setEnabled(true);
    this->compareButton->setEnabled(true);
    this->compareTextEdit->setEnabled(true);

}

QString OperationDialog::getFileMD5(const QString& varFilePath)
{
    QFile sourceFile(varFilePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

OperationDialog::~OperationDialog()
{
}

