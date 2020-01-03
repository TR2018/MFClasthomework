#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMainWindow>
#include<myship.h>
#include<QList>
#include"dataclass.h"
#include<QDataStream>
namespace Ui {
class MapWindow;
}

class MapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = 0);
    ~MapWindow();

    QPixmap _pixmap;//画布图片
    int _openflag;//判断是否是打开序列化文档
    DataClass obj;
    int numCol;//第几个船发生碰撞
    bool col;//是否碰撞

    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent();
    QList<MyShip>shiplist;
    void clearlineedit();
    void settextedixt();
     void collision();//碰撞检测
private:
    Ui::MapWindow *ui;
private slots:
    void on_inputship_button_clicked();

    void on_deleteship_nutton_clicked();

    void on_course_verticalSlider_valueChanged(int value);

    void on_spped_verticalSlider_valueChanged(int value);
    void on_actionReturn_triggered();

    void on_actionopenPic_triggered();

    void on_actionopenTxt_triggered();


signals:
    void returndraw();
};

#endif // MAPWINDOW_H
