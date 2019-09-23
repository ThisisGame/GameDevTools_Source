#include <QtWidgets>
#include <QStringListModel>
#include <QStandardItemModel>

#include "maindialog.h"


MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
{


    //左侧文件列表
    QVBoxLayout *leftLayout = new QVBoxLayout();
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
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSizeConstraint(QLayout::SetFixedSize);

    scrollArea=new QScrollArea();
    scrollArea->setMinimumWidth(590);
    scrollArea->setMinimumHeight(900);

//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
//    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);


    rightLayout->addWidget(scrollArea);

    //主layout
    QHBoxLayout* mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("网络协议记录"));

    this->setObjectName(QString::fromUtf8("MainDialog"));
    QMetaObject::connectSlotsByName(this);

    //加载测试Json文件
    CreateProtocalArray("./demo.json");

    //把窗口设置大一点
    this->resize(1000,1000);
}

QWidget* MainDialog::CreateProtocalPart(const QString& varTypeStr,neb::CJsonObject& varProtocalJsonObject)
{
    std::string gameMessageIDStr;
    varProtocalJsonObject.Get("GameMessageID",gameMessageIDStr);


    std::string protobufStr;
    varProtocalJsonObject.Get("ProtoBuf",protobufStr);

    qDebug()<<"GameMessageID:"<<gameMessageIDStr.data();
    qDebug()<<"ProtoBuf:"<<protobufStr.data();

    QLabel *typeLabel=new QLabel(varTypeStr);

    QLabel *messageidTipsLabel=new QLabel(tr("消息ID:"));
    QLineEdit *messageidLineEdit=new QLineEdit();
    messageidLineEdit->setPlaceholderText(tr("输入消息ID"));
    messageidLineEdit->setText(QString::fromUtf8(gameMessageIDStr.data()));
    neb::CJsonObject* protocalJsonObject=&varProtocalJsonObject;
    connect(messageidLineEdit,&QLineEdit::textChanged,[messageidLineEdit,varProtocalJsonObject]
    {
        qDebug()<<messageidLineEdit->text();
        qDebug()<<varProtocalJsonObject.ToString().data();
        varProtocalJsonObject.
        varProtocalJsonObject.Replace("GameMessageID",messageidLineEdit->text().toStdString());
//        this->SaveCurrentJsonFile();
    });

    QLabel  *protocalTipsLabel=new QLabel(tr("消息结构:"));
    QTextEdit *protocalTextEdit=new QTextEdit();
    protocalTextEdit->setPlaceholderText(tr("输入protobuf结构"));
    protocalTextEdit->setText(QString::fromUtf8(protobufStr.data()));
    protocalTextEdit->setContentsMargins(QMargins());
    connect(protocalTextEdit,&QTextEdit::textChanged,[this,&varProtocalJsonObject,protocalTextEdit]
    {
        varProtocalJsonObject.Replace("ProtoBuf",protocalTextEdit->toPlainText().toStdString());
        this->SaveCurrentJsonFile();
    });

    QVBoxLayout *protocalVBoxLayout = new QVBoxLayout();
    protocalVBoxLayout->setContentsMargins(QMargins());
    protocalVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);


    protocalVBoxLayout->addWidget(typeLabel);
    protocalVBoxLayout->addWidget(messageidTipsLabel);
    protocalVBoxLayout->addWidget(messageidLineEdit);
    protocalVBoxLayout->addWidget(protocalTipsLabel);
    protocalVBoxLayout->addWidget(protocalTextEdit);

    QFrame* containerWidget=new QFrame();
    containerWidget->setFrameStyle(QFrame::StyledPanel);
    containerWidget->setLayout(protocalVBoxLayout);

    return containerWidget;
}

//创建一对协议的UI
//参数的Json是一对协议的
QWidget* MainDialog::CreateProtocal(neb::CJsonObject& varJsonObject)
{
    QGridLayout *protocalGridLayout = new QGridLayout();
    protocalGridLayout->setContentsMargins(QMargins());
    protocalGridLayout->setSizeConstraint(QLayout::SetFixedSize);
    protocalGridLayout->setContentsMargins(10,10,10,10);

    //客户端协议信息,一条
    neb::CJsonObject requestJsonObject;
    if(varJsonObject.Get("Request",requestJsonObject))
    {
        qDebug()<<"clientProtocal";

        QWidget *clientProtocalWidget=this->CreateProtocalPart(tr("客户端"),requestJsonObject);
        protocalGridLayout->addWidget(clientProtocalWidget,0,0);
    }

    //服务器协议信息，多条
    neb::CJsonObject returnJsonObject;
    if(varJsonObject.Get("Return",returnJsonObject))
    {
        qDebug()<<"serverProtocal";
        for(int serverProtocalIndex = 0; serverProtocalIndex< returnJsonObject.GetArraySize(); serverProtocalIndex++)
        {
             neb::CJsonObject serverProtocalJsonObject= returnJsonObject[serverProtocalIndex];

             QWidget *serverProtocalWidget=this->CreateProtocalPart(tr("服务端"),serverProtocalJsonObject);
             protocalGridLayout->addWidget(serverProtocalWidget,0+serverProtocalIndex,2);
        }

        QPushButton* addServerPartPushButton=new QPushButton(tr("添加服务器返回消息"));

//        connect(addServerPartPushButton,&QPushButton::clicked,[=,&serverProtocalJsonArray]()
//        {
//            qDebug()<<"click add";

//            //新建一个json节点
//            QJsonObject serverProtocalJsonObject=QJsonObject();
//            serverProtocalJsonObject.insert("GameMessageID","11");
//            serverProtocalJsonObject.insert("ProtoBuf","22");
//            serverProtocalJsonArray.append(QJsonValue(serverProtocalJsonObject));
//            QWidget *serverProtocalWidget=this->CreateProtocalPart(tr("服务端"),serverProtocalJsonObject);

//            protocalGridLayout->removeWidget(addServerPartPushButton);

//            protocalGridLayout->addWidget(serverProtocalWidget,protocalGridLayout->rowCount(),2);
//            protocalGridLayout->addWidget(addServerPartPushButton,protocalGridLayout->rowCount(),2);
//        });
        protocalGridLayout->addWidget(addServerPartPushButton,1+returnJsonObject.GetArraySize(),2);
    }

    QFrame* containerWidget=new QFrame();
    containerWidget->setFrameStyle(QFrame::StyledPanel);
    containerWidget->setLayout(protocalGridLayout);
    return containerWidget;
}

/// 创建协议列表的界面
void MainDialog::CreateProtocalArray(const QString& varJsonFilePath)
{
    QVBoxLayout* vBoxLayout=new QVBoxLayout();
    vBoxLayout->setContentsMargins(QMargins());
    vBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
    vBoxLayout->setMargin(10);

    QFrame* containerWidget=new QFrame();
//    containerWidget->setFrameStyle(QFrame::StyledPanel);
    containerWidget->setLayout(vBoxLayout);

    //打开Json文件
    QFile jsonFile(varJsonFilePath);

    if(!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "could't open json";
        return;
    }

    QString jsonStr;
    while (!jsonFile.atEnd())
    {
        QByteArray line = jsonFile.readLine();
        QString str(line);
        jsonStr.append(str);
    }
    jsonFile.close();

    neb::CJsonObject* cJsonObject=new neb::CJsonObject(std::string(jsonStr.toLocal8Bit()));

    if(cJsonObject==nullptr)
    {
        qDebug() << "json error!";
        return;
    }

    rootCJsonObject=cJsonObject;
    filePath=varJsonFilePath;

    //读取模块名，例如 登录系统
    std::string moduleName;

    if(rootCJsonObject->Get("Module",moduleName))
    {
        QString moduleNameQStr(QString::fromLocal8Bit(moduleName.data()));
       qDebug() << "moduleName:" << moduleNameQStr;

       //添加标题
       QLabel* titleLabel=new QLabel();
       titleLabel->setText(tr("模块:"));

        QLineEdit* titleLineEdit=new QLineEdit();
        titleLineEdit->setPlaceholderText(tr("输入模块名，比如 背包系统"));
        titleLineEdit->setText(moduleNameQStr);
        titleLabel->setBuddy(titleLineEdit);
        connect(titleLineEdit,&QLineEdit::textChanged,[this,moduleName,titleLineEdit]
        {
            rootCJsonObject->Replace("Module",titleLineEdit->text().toStdString());
            this->SaveCurrentJsonFile();
        });

        QHBoxLayout* titleHBoxLayout=new QHBoxLayout();
        titleHBoxLayout->addWidget(titleLabel);
         titleHBoxLayout->addWidget(titleLineEdit);

       vBoxLayout->addLayout(titleHBoxLayout);
    }

    //读取模块协议列表
    neb::CJsonObject protocalsCJsonObject;
    if(rootCJsonObject->Get("Protocals",protocalsCJsonObject))
    {
        //删除原来的协议列表
        QWidget* oldWidget=scrollArea->widget();
        if(oldWidget!=nullptr)
        {
            delete(oldWidget);
            scrollArea->setWidget(nullptr);
        }

        if(protocalsCJsonObject.IsArray()==false)
        {
            qDebug()<<"not array";
            return;
        }

        for(int i = 0; i< protocalsCJsonObject.GetArraySize(); i++)
        {
             QWidget *protocalWidget =this->CreateProtocal(protocalsCJsonObject[i]);
             vBoxLayout->addWidget(protocalWidget);
        }
    }

    scrollArea->setWidget(containerWidget);
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

    fileDirPath=dirPath;
}

void MainDialog::on_filenameListView_clicked(QModelIndex varModelIndex)
{
    qDebug()<<"on_filenameListView_clicked"<<varModelIndex.data();

    CreateProtocalArray(fileDirPath+"/"+varModelIndex.data().toString());
}

void MainDialog::on_filenameListView_doubleClicked()
{
//    qDebug()<<"on_filenameListView_doubleClicked";
}

///更新Json文件
void MainDialog::SaveCurrentJsonFile()
{
    qDebug()<<"jsonDocument:\n"<<rootCJsonObject->ToString().data();
    //打开Json文件
//    QFile jsonFile(filePath);

//    if(!jsonFile.open(QFile::WriteOnly | QIODevice::Text))
//    {
//        qDebug() << "could't open json for write";
//        return;
//    }

//    jsonFile.write(jsonDocument->toJson());
//    jsonFile.close();
}

MainDialog::~MainDialog()
{
}

