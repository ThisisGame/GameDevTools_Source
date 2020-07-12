#include "dialog.h"
#include "ui_dialog.h"
#include<QtWidgets>
#include<QLineEdit>
#include <QStringListModel>
#include <QStandardItemModel>
#include"MuItemDelegate.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog),unzipWorking(false)
{
    ui->setupUi(this);
    setObjectName(tr("GameDevTools"));
    setWindowTitle(tr("GameDevTools"));

    //去除边框 标题，这样设置后界面不能调整大小
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置界面透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    //添加搜索结果
    ui->toolsInfoListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    pItemDelegate = new MuItemDelegate(this);
    ui->toolsInfoListView->setItemDelegate(pItemDelegate);

    filenameModel= new QStandardItemModel(this);
    ui->toolsInfoListView->setModel(filenameModel);

    //搜索Tools子目录 搜索所有的info.json文件
    this->SearchToolsInfoFile("./Tools");

    //加载所有的info.json文件
    this->LoadToolsInfoJson();

    //刷新下拉框
    this->RefreshListView(toolsInfoJsonList);


    //通过控件 名称 来链接信号槽,自动生成的代码中已经有了，所以这里注释掉。
    //QMetaObject::connectSlotsByName(this);

    //右下角显示icon
    QSystemTrayIcon* trayicon = new QSystemTrayIcon(this);
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
    QIcon icon("./logo.trayico");
    trayicon->setIcon(icon);
    trayicon->show();

    QMenu* trayMenu = new QMenu(this);
    QAction* quitAction=new QAction(tr("退出"),this);
    trayMenu->addAction(quitAction);
    trayicon->setContextMenu(trayMenu);
    QObject::connect(quitAction,SIGNAL(triggered()),this,SLOT(on_quitAction()));

    QObject::connect(&downloadManager, SIGNAL(downloadingOne(const QString&,qint64,qint64,const QString&)), this, SLOT(downloadProgress(const QString&,qint64,qint64,const QString&)));
    QObject::connect(&downloadManager, SIGNAL(finishedOne(const QString&)), this, SLOT(finishedOne(const QString&)));
}

void Dialog::SearchToolsInfoFile(const QString& varDirPath)
{
    QDir dir;
    dir.cd((varDirPath));
    dir.setFilter(QDir::Files);

    QStringList filters;
    filters.append("*.json");
    QFileInfoList fileInfoList= dir.entryInfoList(filters,QDir::AllDirs|QDir::Files);
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if (fileInfo.isDir())
        {
            this->SearchToolsInfoFile(fileInfo.filePath());
        }
        else
        {
            qDebug()<<fileInfo.absoluteFilePath();
            toolsInfoJsonFilePathList.append(fileInfo.absoluteFilePath());
        }
    }
}

void Dialog::LoadToolsInfoJson()
{
    for(int i=0;i<toolsInfoJsonFilePathList.size();i++)
    {
        QString infoJsonFilePath=toolsInfoJsonFilePathList.at(i);
        //读取保存的数据
        QFile loadFile(infoJsonFilePath);
        if(!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "could't open json "+infoJsonFilePath;
            continue;
        }

        QByteArray allData = loadFile.readAll();
        loadFile.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

        if(json_error.error != QJsonParseError::NoError)
        {
            qDebug() << "json error! "+infoJsonFilePath;
            continue;
        }

        QJsonObject rootObj = jsonDoc.object();

        if(rootObj.contains("name")==false)
        {
            continue;
        }
        if(rootObj.contains("desc")==false)
        {
            continue;
        }
        if(rootObj.contains("keyword")==false)
        {
            continue;
        }
        if(rootObj.contains("7zfile")==false)
        {
            continue;
        }
        if(rootObj.contains("exepath")==false)
        {
            continue;
        }

        //默认状态为未下载
        int state=TOOL_STATE_NOT_DOWNLOAD;
        if(this->Exist_Tool(rootObj))
        {
            state=TOOL_STATE_UNZIP_FINISH;
        }
        else
        {
            if(this->Exist_7z(rootObj))
            {
                state=TOOL_STATE_UNZIP;
            }
            else
            {
                state=TOOL_STATE_NOT_DOWNLOAD;
            }
        }
        rootObj["state"]=state;


        rootObj["bytesReceived"]=0;
        rootObj["bytesTotal"]=0;

        toolsInfoJsonList.append(rootObj);
    }

}

void Dialog::RefreshListView(const QList<QJsonObject>& varJsonObjectList)
{
    filenameModel->clear();
    for(int i=0;i<varJsonObjectList.size();i++)
    {
        QJsonObject jsonObject=varJsonObjectList[i];

        QStandardItem* standardItem=new QStandardItem();
        MuItemData itemData;
        itemData.toolName=jsonObject["name"].toString();
        itemData.jsonObject=jsonObject;
        standardItem->setData(QVariant::fromValue(itemData),Qt::UserRole+1);
        filenameModel->appendRow(standardItem);
    }

    //更新ListView高度
    ui->toolsInfoListView->resize(ui->toolsInfoListView->width(),varJsonObjectList.size()*52);
}

bool Dialog::Exist_7z(const QJsonObject& jsonObject)//判断压缩文件是否存在;
{
    QString zipFilePath="./Tools/"+jsonObject["7zfile"].toString();
    QFileInfo zipFileInfo=QFileInfo(zipFilePath);
    return zipFileInfo.exists();
}

bool Dialog::Exist_Tool(const QJsonObject& jsonObject)//判断文件是否已经解压;
{
    //判断是否已经解压过
    QFileInfo startFileInfo("./Tools/"+jsonObject["exepath"].toString());
    return startFileInfo.exists();
}

int Dialog::Ex_7z(const QJsonObject& jsonObject)
{
    if(this->unzipWorking)
    {
        qDebug()<<"unzipWorking";
        return -1;
    }
    QString unzipExeFilePath = "./7z.exe";
    QFileInfo exeFileInfo=QFileInfo(unzipExeFilePath);
    if(exeFileInfo.exists()==false)
    {
        qDebug()<<"7z.exe not exist "<<unzipExeFilePath;
        return -1;
    }

    //判断是否已经解压过
    QFileInfo startFileInfo("./Tools/"+jsonObject["exepath"].toString());
    if(startFileInfo.exists())
    {
        qDebug()<<"have unzip,return";
        return 0;
    }

    QString zipFilePath="./Tools/"+jsonObject["7zfile"].toString();
    QFileInfo zipFileInfo=QFileInfo(zipFilePath);
    if(zipFileInfo.exists()==false)
    {
        qDebug()<<"zipfile not exist,go download "<<zipFilePath;
        this->Download_7z(jsonObject);
        return 1;
    }

    QString zipFileDirPath=zipFileInfo.absolutePath();
    qDebug()<<"ex to "<<zipFileDirPath;


    this->unzipWorking=true;
    {
        while(1)
        {
            QProcess zipProcess(this);

            QStringList args;
            args << "x" << zipFilePath << "-o" + zipFileDirPath << "-aoa";
            qDebug()<<"args:"<<args;
            //x:eXtract with full paths用文件的完整路径解压至当前目录或指定目录
            //-o (Set Output Directory)
            //-aoa 直接覆盖现有文件，而没有任何提示
            zipProcess.start(unzipExeFilePath, args);

            if(zipProcess.waitForStarted()==false)
            {
                qDebug()<<"start failed";
                break;
            }

            zipProcess.waitForFinished(60000);
            if (zipProcess.state() != QProcess::NotRunning) {
                qDebug()<<"NotRunning";
                break;
            }

            if (zipProcess.exitStatus() == QProcess::NormalExit) {
                qDebug()<<"NormalExit";
                this->ModifyState(jsonObject["name"].toString(),TOOL_STATE_UNZIP_FINISH);
                break;
            }
            break;
        }
    }
    this->unzipWorking=false;
    return 0;
}

void Dialog::Download_7z(const QJsonObject& jsonObject)//下载7z文件
{
    //已经存在，不用下载
    QString zipFilePath="./Tools/"+jsonObject["7zfile"].toString();
    QFileInfo zipFileInfo=QFileInfo(zipFilePath);
    if(zipFileInfo.exists())
    {
        qDebug()<<"zipfile exist,go unzip"<<zipFilePath;
        this->Ex_7z(jsonObject);
        return;
    }

    //下载队列
    QString name=jsonObject["name"].toString();
    QString zipUrl=jsonObject["7zurl"].toString();
    downloadManager.append(name,zipUrl,zipFilePath);

    //修改状态
    this->ModifyState(jsonObject["name"].toString(),TOOL_STATE_DOWNLOADING);
}

void Dialog::Start(const QJsonObject& jsonObject)//启动工具
{
    //判断是否解压成功
    QFileInfo startFileInfo("./Tools/"+jsonObject["exepath"].toString());
    if(startFileInfo.exists()==false)
    {
        qDebug()<<"tool not exist;"<<startFileInfo.absoluteFilePath();
        return;
    }

    qDebug()<<"startDetached "<<startFileInfo.absoluteFilePath();
    QProcess toolProcess(this);
    toolProcess.start(startFileInfo.absoluteFilePath());
    toolProcess.waitForStarted(30000);
    toolProcess.waitForFinished(60000);
}

void Dialog::ModifyState(const QString& varName,const int varState)//修改状态
{
    for(int i=0;i<toolsInfoJsonList.size();i++)
    {
        QJsonObject jsonObject=toolsInfoJsonList.at(i);
        if(jsonObject["name"]==varName)
        {
            jsonObject["state"]=varState;
            toolsInfoJsonList[i]=jsonObject;
        }
    }
    this->RefreshListView(toolsInfoJsonList);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_toolsInfoListView_clicked(QModelIndex varModelIndex)
{

}

void Dialog::on_keywordLineEdit_textChanged(const QString &varText)
{
    qDebug()<<"on_keywordLineEdit_textChanged:"<<varText;

    //将关键字转为小写
    QString lowerText=varText.toLower();

    //首先获取所有符合关键字的jsonobject
    QList<QJsonObject> toolsInfoJsonList_Keyword;
    for(int i=0;i<toolsInfoJsonList.size();i++)
    {
        QJsonObject jsonObject=toolsInfoJsonList[i];
        QString nameStr=jsonObject["name"].toString();
        if(nameStr.toLower().contains(lowerText))
        {
            toolsInfoJsonList_Keyword.append(jsonObject);
            continue;
        }
        QString descStr=jsonObject["desc"].toString();
        if(descStr.toLower().contains(lowerText))
        {
            toolsInfoJsonList_Keyword.append(jsonObject);
            continue;
        }
        QJsonArray keywordJsonArray=jsonObject["keyword"].toArray();
        for(int keywordIndex=0;keywordIndex<keywordJsonArray.size();keywordIndex++)
        {
            QString keywordStr=keywordJsonArray[keywordIndex].toString();
            if(keywordStr.toLower().contains(lowerText))
            {
                toolsInfoJsonList_Keyword.append(jsonObject);
                continue;
            }
        }

    }
    //更新匹配列表
    this->RefreshListView(toolsInfoJsonList_Keyword);
}

void Dialog::on_keywordLineEdit_BG_returnPressed()
{
    qDebug()<<"on_keywordLineEdit_BG_returnPressed";
}

void Dialog::on_keywordLineEdit_returnPressed()
{
    qDebug()<<"on_keywordLineEdit_returnPressed";
    qDebug()<<"listview itemcount:"<<QString::number(filenameModel->rowCount());

    ui->toolsInfoListView->setFocus();

    if(filenameModel->rowCount()==1)
    {
        //输入框 按回车，如果只有1个匹配项，那就直接打开

    }
    else if(filenameModel->rowCount()>1)
    {
        //如果有多个匹配项，那就将焦点，传递到列表。
    }
}

void Dialog::on_toolsInfoListView_doubleClicked(const QModelIndex &varModelIndex)
{
    if (varModelIndex.isValid()) {
        QVariant var = varModelIndex.data(Qt::UserRole+1);
        MuItemData itemData = var.value<MuItemData>();

        qDebug()<<"on_toolsInfoListView_clicked:"<<itemData.toolName;

        QJsonObject& jsonObject=itemData.jsonObject;


        QString zipFilePathStr=jsonObject["7zfile"].toString();
        qDebug()<<"unzip "<<zipFilePathStr;

        if(this->Ex_7z(jsonObject)!=0)
        {
            //开始下载 直接返回
            return;
        }

        this->Start(jsonObject);
    }
}

//实现槽函数
void Dialog::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //单击
    case QSystemTrayIcon::Trigger:
        //双击
    case QSystemTrayIcon::DoubleClick:
        if(this->isHidden())
        {
            //恢复窗口显示
            this->show();
            //一下两句缺一均不能有效将窗口置顶
            this->setWindowState(Qt::WindowActive);
            this->activateWindow();
        }
        else
        {
//            this->hide();
        }
        break;
    default:
        break;
    }
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
    case Qt::Key_F4:
        qDebug()<<"esc click";
        this->hide();
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

void Dialog::on_quitAction()
{
    qDebug()<<"quitAction";
    this->close();
}

void Dialog::downloadProgress(const QString& name,qint64 bytesReceived, qint64 bytesTotal,const QString& speedDesc)
{
    qDebug()<<"downloadProgress "<<name<<" "<<bytesReceived<<" "<<bytesTotal;

    qDebug()<<"bytesReceived:"<<bytesReceived;
    qDebug()<<"bytesTotal:"<<bytesTotal;


    for(int i=0;i<toolsInfoJsonList.size();i++)
    {
        QJsonObject jsonObject=toolsInfoJsonList.at(i);
        if(jsonObject["name"]==name)
        {
            jsonObject["bytesReceived"]=bytesReceived;
            jsonObject["bytesTotal"]=bytesTotal;
            toolsInfoJsonList[i]=jsonObject;
        }
    }
    this->RefreshListView(toolsInfoJsonList);
}

void Dialog::finishedOne(const QString& name)
{
    this->ModifyState(name,TOOL_STATE_UNZIP);
}
