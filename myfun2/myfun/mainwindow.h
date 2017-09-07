#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class QLineEdit;
class QDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newFile();   // 新建操作
    bool maybeSave(); // 判断是否需要保存
    bool save();      // 保存操作
    bool saveAs();    // 另存为操作
    bool saveFile(const QString &fileName); // 保存文件
    bool loadFile(const QString &fileName); // 加载文件

private slots:
    void on_action_triggered();//新建

    void on_action_S_triggered();//保存

    void on_action_A_triggered();//另存为

    void on_action_2_triggered();

    void on_action_C_triggered();

    void on_action_X_triggered();

    void showFindText();

    void on_action_F_triggered();

    void on_action_R_triggered();

    void on_action_H_triggered();

private:
    Ui::MainWindow *ui;
    // 为真表示文件没有保存过，为假表示文件已经被保存过了
    bool isUntitled;
    // 保存当前文件的路径
    QString curFile;
    QLineEdit *findLineEdit;
    QDialog *findDlg;

protected:
    void closeEvent(QCloseEvent *event); // 关闭事件
};

#endif // MAINWINDOW_H
