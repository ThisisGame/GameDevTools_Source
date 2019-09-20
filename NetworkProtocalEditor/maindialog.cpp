#include <QtWidgets>
#include <QStringListModel>
#include <QStandardItemModel>

#include "maindialog.h"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{


    //左侧文件列表
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setContentsMargins(QMargins());
    leftLayout->setSizeConstraint(QLayout::SetFixedSize);

    QPushButton* chooseDirPushButton=new QPushButton(tr("选择文件夹"));
    chooseDirPushButton->setObjectName(QString::fromUtf8("chooseDirPushButton"));

    filenameModel= new QStandardItemModel(this);

    filenameListView=new QListView();
    filenameListView->setEnabled(false);
    filenameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    filenameListView->setModel(filenameModel);
    filenameListView->setObjectName(QString::fromUtf8("filenameListView"));

    leftLayout->addWidget(chooseDirPushButton);
    leftLayout->addWidget(filenameListView);
    filenameListView->setEnabled(false);


    leftLayout->addStretch();

    //右侧消息协议列表
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(QMargins());
    rightLayout->setSizeConstraint(QLayout::SetFixedSize);

    QScrollArea* scrollArea=new QScrollArea();
    scrollArea->setMinimumHeight(900);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    QWidget *protocalWidget =this->CreateProtocal();

    scrollArea->setWidget(protocalWidget);

    rightLayout->addWidget(scrollArea);

    //主layout
    QHBoxLayout* mainLayout=new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("网络协议记录"));

    this->setObjectName(QString::fromUtf8("MainDialog"));
    QMetaObject::connectSlotsByName(this);
}

QWidget* MainDialog::CreateProtocalPart(const QString& varTypeStr)
{
    QLabel *typeLabel=new QLabel(varTypeStr);

    QLabel *messageidTipsLabel=new QLabel(tr("消息ID:"));
    QLineEdit *messageidLineEdit=new QLineEdit();
    messageidLineEdit->setPlaceholderText(tr("GM_WORLDBOSS_EXIT_RETURN"));

    QLabel  *protocalTipsLabel=new QLabel(tr("消息结构:"));
    QTextEdit *protocalTextEdit=new QTextEdit();
    protocalTextEdit->setPlaceholderText(tr("message GM_WorldBoss_Exit_Return"));
    protocalTextEdit->setContentsMargins(QMargins());

    QVBoxLayout *protocalVBoxLayout = new QVBoxLayout;
    protocalVBoxLayout->setContentsMargins(QMargins());
    protocalVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);


    protocalVBoxLayout->addWidget(typeLabel);
    protocalVBoxLayout->addWidget(messageidTipsLabel);
    protocalVBoxLayout->addWidget(messageidLineEdit);
    protocalVBoxLayout->addWidget(protocalTipsLabel);
    protocalVBoxLayout->addWidget(protocalTextEdit);

    QFrame* containerWidget=new QFrame();
    containerWidget->setFrameStyle(QFrame::Box);
    containerWidget->setLayout(protocalVBoxLayout);

    return containerWidget;
}

QWidget* MainDialog::CreateProtocal()
{
    QGridLayout *protocalGridLayout = new QGridLayout;
    protocalGridLayout->setContentsMargins(QMargins());
    protocalGridLayout->setSizeConstraint(QLayout::SetFixedSize);
    protocalGridLayout->setContentsMargins(10,10,10,10);

    QWidget *clientProtocalWidget=this->CreateProtocalPart(tr("客户端"));
    QWidget *serverProtocalWidget=this->CreateProtocalPart(tr("服务端"));
    QPushButton* addServerPartPushButton=new QPushButton(tr("添加服务器返回消息"));

    connect(addServerPartPushButton,&QPushButton::clicked,[=]()
    {
        qDebug()<<"click add";
        QWidget *serverProtocalWidget=this->CreateProtocalPart(tr("服务端"));

        protocalGridLayout->removeWidget(addServerPartPushButton);

        protocalGridLayout->addWidget(serverProtocalWidget,protocalGridLayout->rowCount(),2);
        protocalGridLayout->addWidget(addServerPartPushButton,protocalGridLayout->rowCount(),2);
    });

    protocalGridLayout->addWidget(clientProtocalWidget,0,0);
    protocalGridLayout->addWidget(serverProtocalWidget,0,2);
    protocalGridLayout->addWidget(addServerPartPushButton,1,2);

    QFrame* containerWidget=new QFrame();
    containerWidget->setFrameStyle(QFrame::Box);
    containerWidget->setLayout(protocalGridLayout);
    return containerWidget;
}

void MainDialog::on_chooseDirPushButton_clicked()
{
    qDebug()<<"on_chooseDirPushButton_clicked";
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug()<<dirPath;
    if(dirPath.size()==0)
    {
        qDebug()<<"calcel select dir";
        return;
    }

    filenameListView->setEnabled(true);
    filenameModel->clear();

    dirPath=QDir::toNativeSeparators(dirPath);
    QDir dir(dirPath);
    dir.setFilter(QDir::Files);

    QFileInfoList fileInfoList= dir.entryInfoList();
    for(int index=0;index<fileInfoList.length();index++)
    {
        QString fileName=fileInfoList.at(index).fileName();
        qDebug()<<fileName;

        QStandardItem* standardItem=new QStandardItem(fileName);
        filenameModel->appendRow(standardItem);
    }
}

void MainDialog::on_filenameListView_clicked(QModelIndex varModelIndex)
{
    qDebug()<<"on_filenameListView_clicked"<<varModelIndex.data();
}

void MainDialog::on_filenameListView_doubleClicked()
{
//    qDebug()<<"on_filenameListView_doubleClicked";
}

MainDialog::~MainDialog()
{
}

