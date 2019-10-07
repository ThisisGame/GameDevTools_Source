#include <QtWidgets>
#include "operationdialog.h"
#include <QCryptographicHash>





OperationDialog::OperationDialog(QWidget *parent)
    : QDialog(parent),fixedMemory(0)
{
    setWindowTitle(tr("获取Lua数据表 占用内存"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(QMargins(5,5,5,5));
    vboxLayout->setSizeConstraint(QLayout::SetFixedSize);

    tipsLabel=new QLabel(tr("获取Lua数据表内存占用，非精准数据，只作为优化的时候进行对比参考。\n\n基于lua5.3.5。\n"));
    vboxLayout->addWidget(tipsLabel);


    choosefileButton = new QPushButton(tr("打开Lua数据表"));
    choosefileButton->setObjectName(QString::fromUtf8("choosefileButton"));

    vboxLayout->addWidget(choosefileButton);

    costMemoryLabel=new QLabel(tr(""));
    vboxLayout->addWidget(costMemoryLabel);

    setLayout(vboxLayout);

    this->setObjectName(QString::fromUtf8("OperationDialog"));
    QMetaObject::connectSlotsByName(this);

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);


        luaL_dostring(L,"return collectgarbage(\"count\")");
        LUA_NUMBER tmpMemory_Pre = lua_tonumber(L, -1);
        qDebug()<<"memory_Pre:"<<QString::number(tmpMemory_Pre);



        luaL_dostring(L,"collectgarbage(\"collect\")");

        luaL_dostring(L,"return collectgarbage(\"count\")");
        LUA_NUMBER tmpMemory_After = lua_tonumber(L, -1);
        qDebug()<<"memory_After:"<<QString::number(tmpMemory_After);

        //计算固定消耗内存
        fixedMemory=tmpMemory_After-tmpMemory_Pre;
        qDebug()<<"fixedMemory:"<<QString::number(fixedMemory);

    lua_close(L);
}


void OperationDialog::on_choosefileButton_clicked()
{

    QString strFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "","*.*");
    if(strFilePath=="")
    {
        qDebug()<<"not choose file";
        return;
    }
    lua_State* L = luaL_newstate();
    for(int i=0;i<1;i++)
    {
        luaL_openlibs(L);


        luaL_dostring(L,"return collectgarbage(\"count\")");
        LUA_NUMBER tmpMemory_Pre = lua_tonumber(L, -1);
        qDebug()<<"memory_Pre:"<<QString::number(tmpMemory_Pre);


        int tmpRet=luaL_dofile(L,strFilePath.toStdString().c_str());
        if(tmpRet!=0)
        {
            costMemoryLabel->setText("error");
            break;
        }
        luaL_dostring(L,"collectgarbage(\"collect\")");

        luaL_dostring(L,"return collectgarbage(\"count\")");
        LUA_NUMBER tmpMemory_After = lua_tonumber(L, -1);
        qDebug()<<"memory_After:"<<QString::number(tmpMemory_After);

        lua_Number costMemory=tmpMemory_After-tmpMemory_Pre-fixedMemory;
        QString costMemoryStr=QString::number(costMemory);
        qDebug()<<"costMemory:"<<costMemoryStr;

        costMemoryLabel->setText("占用内存:"+costMemoryStr+"kb");
    }


    lua_close(L);
}


OperationDialog::~OperationDialog()
{
}

