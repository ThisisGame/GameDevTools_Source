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
    QPushButton *getmd5Button;


    QPushButton *compareButton;
    QTextEdit *compareTextEdit;

private slots:

    void on_getmd5Button_clicked();
    void on_compareButton_clicked();

};
#endif // OPERATIONDIALOG_H
