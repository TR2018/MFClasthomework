#ifndef DATACLASS_H
#define DATACLASS_H

#include <QWidget>
#include<QVector>
#include<QPoint>
#include<QDataStream>
class DataClass : public QWidget
{
    Q_OBJECT
public:
    explicit DataClass(QWidget *parent = 0);


    QVector<QVector<QPoint> > _lines;//线条集合(一条线条可包含多个线段)
    QVector<QRect> _rects;//矩形集合
    QVector<QRect> _ellipse;//椭圆集合
    QVector<QRect>  _line;//直线集合
    QVector<int>  _shape;//图形类型集合，用于撤回功能
    QVector<QPoint>_lights;//灯浮点集合
    bool _fill;//判断是否填充
signals:

public slots:
};

#endif // DATACLASS_H
