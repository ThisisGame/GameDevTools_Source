#include "dialog.h"

#include <QApplication>
#include <QFontDatabase>

#include <QtMessageHandler>
#include <QFile>
#include <QDir>
#include <QDebug>
#include<QDateTime>
#include <iostream>
using namespace std;

//写入信息到文件
void WriteLog(QString str, QString LogType)
{

    QString fileFolder= qApp->applicationDirPath()+"/log/"+QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir dir(fileFolder);
    if(!dir.exists())
    {
        dir.mkpath(fileFolder);
    }
    QString filePath=QString("%1/%2.log").arg(fileFolder).arg(LogType);
    QString strToWrite="日志产生时间："+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    strToWrite.append(QString("\r\n日志消息：%1").arg(str));
    strToWrite.append("\r\n---------------------------------------------------------------------");
    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream <<strToWrite<<"\r\n";
    file.flush();
    file.close();
    //打印到控制台
    std::cout << strToWrite.toLocal8Bit().constData() << std::endl;
}
//注册函数
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txtMessage = "";
    QString messageType = "";
    switch (type)    {
    case QtDebugMsg:    //调试信息提示
        messageType = "Debug";
        txtMessage = QString("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
        break;
    case QtInfoMsg:
        messageType = "Info";
        txtMessage = QString("Warning: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
        break;
    case QtWarningMsg:    //一般的warning提示
        messageType = "Waring";
        txtMessage = QString("Warning: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
        break;
    case QtCriticalMsg:    //严重错误提示
        messageType = "Critical";
        txtMessage = QString("Critical: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);

        break;
    case QtFatalMsg:    //致命错误提示
        messageType = "Fatal";
        txtMessage = QString("Fatal: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
        break;
    }
    WriteLog(txtMessage, messageType);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //注册监听级别函数
    qInstallMessageHandler(myMessageOutput);
    qDebug()<<"Application start;";

    Dialog w;
    w.show();
    return a.exec();
}
