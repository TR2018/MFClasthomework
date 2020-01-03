#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QAction>
#include <QPalette>
#include <QColor>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
#include<QBrush>
#include<QPainterPath>
#include<QPen>
#include"mapwindow.h"
#include<QDataStream>
#include"dataclass.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

   DataClass obj;
protected:
    void paintEvent(QPaintEvent *);//重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);//重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);//重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);//重写鼠标移动事件
    void contextMenuEvent(QContextMenuEvent *);//重写菜单事件
    void keyPressEvent(QKeyEvent *e); //重写按键事件
private:
    Ui::MainWindow *ui;
    int _lpress;//左键按下标志
    int _drawType;//描绘类型
    QMenu *_Rmenu;//右键菜单
    int _openflag;//打开图片
    QPixmap _pixmap;//画布图片

    int _time;//填充点击次数

    MapWindow * mapwindow=NULL;


public slots:
    void Lines();//铅笔画线
    void SavePic();//保存图片
    void SaveTxt();//保存序列化文档
    void Rects();//画矩形
    void Ellipses();//画椭圆
    void Line();//画直线
    void OpenPic();//打开图片
     void OpenTxt();//打开序列化文档
    void FillPic();//填充事件
    void ligh();//画灯浮


private slots:
    void on_actionMap_triggered();
};

#endif // MAINWINDOW_H
