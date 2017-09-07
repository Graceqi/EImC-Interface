#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化文件为未保存状态
    isUntitled = true;
    // 初始化文件名为"未命名.txt"
    curFile = tr("未命名.txt");
    // 初始化窗口标题为文件名
    setWindowTitle(curFile);
    /*添加查找对话框*/
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("查找"));
    findLineEdit = new QLineEdit(findDlg);
    QPushButton *btn= new QPushButton(tr("查找下一个"), findDlg);
    QVBoxLayout *layout= new QVBoxLayout(findDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn, &QPushButton::clicked, this, &MainWindow::showFindText);
    ui->statusBar->showMessage(tr("欢迎使用EImC解释器"),2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFile()
{
    if(maybeSave()){
        isUntitled=true;
        curFile=tr("未命名.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    //如果文档被更改
    if(ui->textEdit->document()->isModified())
    {
        //自定义一个警告栏
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile+tr("尚未保存，是否保存？"));
        QPushButton *yesBtn=box.addButton(tr("是（&Y）"),QMessageBox::YesRole);
        box.addButton(tr("否（&N)"),QMessageBox::NoRole);
        QPushButton *cancelBut=box.addButton(tr("取消"),QMessageBox::RejectRole);
        box.exec();
        if(box.clickedButton()==yesBtn)
            return save();
        else if(box.clickedButton()==cancelBut)
            return false;
    }
    //如果没被更改，则返回true
    return true;
}

bool MainWindow::save()
{
    if(isUntitled)
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString filename=QFileDialog::getSaveFileName(this,tr("另存为"),curFile);
    if(filename.isEmpty()) return false;
    return saveFile(filename);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        //%1和%2分别对应后面arg两个参数，/n起换行的作用
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法写入文件 %1: /n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    //鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<ui->textEdit->toPlainText();
    //鼠标指针恢复为原来的状态
    QApplication::restoreOverrideCursor();
    isUntitled=false;
    //获得文件的标准路径
    curFile=QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::on_action_triggered()//新建
{
    newFile();
}

void MainWindow::on_action_S_triggered()//保存
{
    save();
}

void MainWindow::on_action_A_triggered()//另存为
{
    saveAs();
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);//新建QFile对象
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法读取文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;//只读方式打开文件，出错则提示，并返回0
    }
    QTextStream in(&file);//新建文本流对象
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //读取文件中的全部文本内容，再添加到编辑器中
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    //设置当前文件
    curFile=QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::on_action_2_triggered()//打开文件
{
    if(maybeSave())
    {
        QString fileName=QFileDialog::getOpenFileName(this);
        //如果文件名不为空，则加载文件
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_action_C_triggered()//关闭文件
{
    if(maybeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_action_X_triggered()//退出
{
    //先执行关闭操作，再退出程序
    //qApp是指向应用程序的全局指针
    on_action_C_triggered();
    qApp->quit();
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    if (ui->textEdit->find(str, QTextDocument::FindBackward)==false)
    {
       QMessageBox::warning(this, tr("查找"),
                tr("找不到%1").arg(str));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //如果maybeSave()函数返回true,则关闭程序
    if(maybeSave())
    {
        event->accept();
    }
    else
    {//否则忽略该事件
        event->ignore();
    }
}

void MainWindow::on_action_F_triggered()
{
    findDlg->show();
}

void MainWindow::on_action_R_triggered()//用于跟解释器衔接
{

}

void MainWindow::on_action_H_triggered()
{
    QString str = "寻寻觅觅，冷冷清清，凄凄惨惨戚戚。乍暖还寒时候，最难将息。三杯两盏淡酒，怎敌他晓来风急？雁过也，正伤心，却是旧时相识。满地黄花堆积。憔悴损，如今有谁堪摘？守著窗儿独自，怎生得黑？梧桐更兼细雨，到黄昏、点点滴滴。这次第，怎一个愁字了得！";
    QMessageBox box;
    box.setWindowTitle("使用说明");
    box.setText(str);
    box.exec();
}
