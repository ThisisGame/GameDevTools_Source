#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private:

    /// 创建一个协议的界面
    /// @variable varTypeStr 标题，客户端 or 服务器
    QWidget* CreateProtocalPart(const QString& varTypeStr);

    /// 创建一对协议的界面
    QWidget* CreateProtocal();

};
#endif // MAINWINDOW_H
