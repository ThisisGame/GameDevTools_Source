#ifndef OPERATIONDIALOG_H
#define OPERATIONDIALOG_H

#include <QDialog>

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
    QTextEdit *oldTextEdit;
    QTextEdit *newTextEdit;

    QDialogButtonBox *buttonBox;
    QPushButton *unicode2chineseButton;

private slots:

    void on_unicode2chineseButton_clicked();

};
#endif // OPERATIONDIALOG_H
