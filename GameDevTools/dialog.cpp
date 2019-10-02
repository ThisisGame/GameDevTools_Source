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

void Dialog::Ex_7z(const QJsonObject& jsonObject)
{
    if(this->unzipWorking)
    {
        qDebug()<<"unzipWorking";
        return;
    }
    QString unzipExeFilePath = "7z.exe";
    QFileInfo exeFileInfo=QFileInfo(unzipExeFilePath);
    if(exeFileInfo.exists()==false)
    {
        qDebug()<<"zipfile not exist "<<unzipExeFilePath;
        return;
    }

    //判断是否已经解压过
    QFileInfo startFileInfo("./Tools/"+jsonObject["exepath"].toString());
    if(startFileInfo.exists())
    {
        qDebug()<<"have unzip,return";
        return;
    }

    QString zipFilePath="./Tools/"+jsonObject["7zfile"].toString();
    QFileInfo zipFileInfo=QFileInfo(zipFilePath);
    if(zipFileInfo.exists()==false)
    {
        qDebug()<<"zipfile not exist "<<zipFilePath;
        return;
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

                break;
            }
            break;
        }
    }
    this->unzipWorking=false;
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

        this->Ex_7z(jsonObject);

        this->Start(jsonObject);
    }
}
