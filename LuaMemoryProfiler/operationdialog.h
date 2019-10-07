#ifndef OPERATIONDIALOG_H
#define OPERATIONDIALOG_H

#include <QDialog>

extern "C"
{
    #include "lua-5.3.5/src/lua.h"
    #include "lua-5.3.5/src/lualib.h"
    #include "lua-5.3.5/src/lauxlib.h"
    #include "lua-5.3.5/src/luaconf.h"
}

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
class QDialogButtonBox;
QT_END_NAMESPACE

class OperationDialog : public QDialog
{
    Q_OBJECT

public:
    OperationDialog(QWidget *parent = nullptr);
    ~OperationDialog();

private:
    QLabel* tipsLabel;


    lua_Number fixedMemory;

    QPushButton *choosefileButton;


    QLabel* costMemoryLabel;

private slots:

    void on_choosefileButton_clicked();
};
#endif // OPERATIONDIALOG_H
