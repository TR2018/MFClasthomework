#include "mapwindow.h"
#include "ui_mapwindow.h"
#include<QPainter>
#include<QDebug>
#include"myship.h"
#include<qmath.h>
#include<QTimer>
#include<QMenuBar>
#include<QDialog>
#include<QVBoxLayout>
#include<QWheelEvent>
#include<QFileDialog>
#include<QMessageBox>
#include<QPolygon>
void MapWindow::settextedixt()
{

        QString shipname="船名: "+shiplist[shiplist.size()-1].STI.Ship_Name;
        QString callsign="呼号: "+shiplist[shiplist.size()-1].STI.Call_Sign;
        QString MMSI="MMSI: "+shiplist[shiplist.size()-1].STI.MMSI;
        QString brandth="船宽： "+QString::number(shiplist[shiplist.size()-1].STI.Ship_Breadth,'f',2);
        QString length="船长： "+QString::number(shiplist[shiplist.size()-1].STI.Ship_Length,'f',2);
        QString draft="吃水： "+QString::number(shiplist[shiplist.size()-1].STI.Draft,'f',2);
        QString displacement="排水量： "+QString::number(shiplist[shiplist.size()-1].STI.Displacement,'f',2);
        QString course="航向："+QString::number(shiplist[shiplist.size()-1].DYI.Course,'f',2);
        QString speed="航速："+QString::number(shiplist[shiplist.size()-1].DYI.Speed,'f',2);
        QString Longitude="经度："+QString::number(shiplist[shiplist.size()-1].DYI.longitude_real,'d',4);
        QString Latitude="纬度："+QString::number(shiplist[shiplist.size()-1].DYI.Latitude_real,'d',4);
        QString str=shipname+"\n"+callsign+"\n"+MMSI+"\n"+brandth+"\n"+length+"\n"+draft+"\n"+displacement+"\n"+course+"\n"+speed+"\n"+Longitude+"\n"+Latitude;
        ui->information_textEdit->setText(str);

}


void MapWindow::collision()//碰撞检测
{
    for(int i=0; i<shiplist.size();i++)
    {
        QPoint p(shiplist[i].Ship_Shape_x1+(ui->pixmap_widget->width()/2),shiplist[i].Ship_Shape_y1+(ui->pixmap_widget->height()/2)+52);
        for(int j=0;j<obj._rects.size();j++)
        {
            if(obj._rects.at(j).contains(p))
            {
                QString str=QString("'%1'发生碰撞，已删除").arg(shiplist[i].STI.Ship_Name);
                QMessageBox::critical(this,"碰撞",str);
                numCol=i;
                col=true;
                emit ui->deleteship_nutton->click();
            }
        }


        for(int j=0;j<obj._ellipse.size();j++)
        {
            if(obj._ellipse.at(j).contains(p))
            {
                QString str=QString("'%1'发生碰撞，已删除").arg(shiplist[i].STI.Ship_Name);
                QMessageBox::critical(this,"碰撞",str);
                numCol=i;
                col=true;
                emit ui->deleteship_nutton->click();
            }
        }


        for(int k=0;k<obj._lines.size();k++)
        {
            const QVector<QPoint>&line = obj._lines.at(k);//取出_lines第k个元素进行检测
            QPolygon polygon(line);
            bool iscol=polygon.containsPoint(p,Qt::OddEvenFill);
            if(iscol)
            {
                QString str=QString("'%1'发生线段碰撞，已删除").arg(shiplist[i].STI.Ship_Name);
                QMessageBox::critical(this,"碰撞",str);
                numCol=i;
                col=true;
                emit ui->deleteship_nutton->click();
            }
        }

    }
}

MapWindow::MapWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("大作业");

    ui->splitter->setStyleSheet("QSplitter::handle { background-color: black }");
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,8);
    ui->pixmap_widget->installEventFilter(this);
    ui->information_textEdit->setGeometry(ui->pixmap_widget->width()-ui->information_textEdit->width(),0,300,200);
    QTimer * time=new QTimer(this);
    _openflag=0;//初始不打开图片
    col=false;//初始无碰撞
    //qDebug()<<ui->pixmap_widget->width()/2;

    time->start(100);
    connect(time,&QTimer::timeout,[=](){
        if(shiplist.size()!=0)
        {
            for(int i=0;i!=shiplist.size();i++)
            {
                shiplist[i].ShipShape_Move();
            }
            update();
            settextedixt();
//            QPoint p(shiplist[0].Ship_Shape_x1+(ui->pixmap_widget->width()/2),shiplist[0].Ship_Shape_y1+(ui->pixmap_widget->height()/2)+52);
//          qDebug()<<p.x();
           collision();
        }
    });
}
 bool MapWindow::eventFilter(QObject *watched, QEvent *event)
 {
     if (watched == ui->pixmap_widget && event->type() == QEvent::Paint)
             paintEvent();
         return QWidget::eventFilter(watched, event);

 }
 void MapWindow::clearlineedit()
 {
     ui->ship_name_lineEdit->setText("");
     ui->call_sign_lineEdit->setText("");
     ui->MMSI_lineEdit->setText("");
     ui->ship_length_lineEdit->setText("");
     ui->ship_breath_lineEdit->setText("");
     ui->draft_lineEdit->setText("");
     ui->displacement_lineEdit->setText("");
     ui->couse_lineEdit->setText("");
     ui->spped_lineEdit->setText("");
     ui->longitude_lineEdit->setText("");
     ui->latitude_lineEdit->setText("");
 }

void MapWindow::paintEvent()
{
    QPainter painter(ui->pixmap_widget);
     //painter.scale(sx,sy);
    if(_openflag == 0)//打开图片,不是序列化文档
    {
       painter.drawPixmap(0,0,ui->pixmap_widget->width(),ui->pixmap_widget->height(),_pixmap);
    }
    else
    {

        QPixmap pix= QPixmap(this->size());
        pix.fill(QColor(152,211,237));
        QPainter p(&pix);

        if(obj._fill)
        {
            QBrush brush(QColor(174,160,82));
            p.setBrush(brush);
        }
        QPainterPath path;
        unsigned int i1=0,i2=0,i3=0,i4=0,i5=0;//各种图形的索引

        for(int c = 0;c<obj._shape.size();++c)//控制用户当前所绘图形总数
        {
            if(obj._shape.at(c) == 1)//线条
            {
                const QVector<QPoint>& line = obj._lines.at(i1++);//取出一条线条
                  path.moveTo(line.at(0));
                for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
                {
                    //path.moveTo(line.at(0));
                    path.lineTo(line.at(j+1));
                    //qDebug()<<line.at(j);

                   //  p.drawLine(line.at(j), line.at(j+1));

                    if(obj._fill)
                    {
                        QBrush brush(QColor(174,160,82));
                        QPen pen;
                        path.setFillRule(Qt::WindingFill);
                        p.fillPath(path,brush);
                        p.strokePath(path,pen);
                    }
                    else
                    {
                        //p.drawLine(line.at(j), line.at(j+1));
                        p.drawPath(path);
                    }
                }

            }
            else if(obj._shape.at(c) == 2)//矩形
            {
                p.drawRect(obj._rects.at(i2++));
                //qDebug()<<obj._rects.last().topLeft()<<"  "<<obj._rects.last().bottomRight();
            }
            else if(obj._shape.at(c) == 3)//椭圆
            {
                p.drawEllipse(obj._ellipse.at(i3++));
            }
            else if(obj._shape.at(c) == 4)//直线
            {
                p.drawLine(obj._line.at(i4).topLeft(),obj._line.at(i4).bottomRight());
                i4++;
            }
            else if(obj._shape.at(c) == 5)//灯浮
            {
                QPixmap map;
                map.load(":/res/112.png");
                p.drawPixmap(obj._lights.at(i5).x(),obj._lights.at(i5).y(),20,20,map);
                i5++;
            }

        }

       painter.drawPixmap(0,-52,pix);
    }
    ///////////////////////////////海图绘制结束//////////////////////////////////////////////


    //painter.drawPixmap(0,0,ui->pixmap_widget->width(),ui->pixmap_widget->height(),_pixmap);
    painter.translate(ui->pixmap_widget->width()/2,ui->pixmap_widget->height()/2);
    if(shiplist.size()!=0)
    {
        for(int i=0;i!=shiplist.size();i++)
        {

            painter.drawLine(shiplist[i].Ship_Shape_x1,shiplist[i].Ship_Shape_y1,shiplist[i].Ship_Shape_x2,shiplist[i].Ship_Shape_y2);
            painter.drawLine(shiplist[i].Ship_Shape_x2,shiplist[i].Ship_Shape_y2,shiplist[i].Ship_Shape_x3,shiplist[i].Ship_Shape_y3);
            painter.drawLine(shiplist[i].Ship_Shape_x3,shiplist[i].Ship_Shape_y3,shiplist[i].Ship_Shape_x1,shiplist[i].Ship_Shape_y1);
        }
    }
}

void MapWindow::on_inputship_button_clicked()
{
    MyShip ship;
    ship.STI.Ship_Name=ui->ship_name_lineEdit->text();
    ship.STI.Call_Sign=ui->call_sign_lineEdit->text();
    ship.STI.MMSI=ui->MMSI_lineEdit->text();
    ship.STI.Ship_Length=ui->ship_length_lineEdit->text().toFloat();
    ship.STI.Ship_Breadth=ui->ship_breath_lineEdit->text().toFloat();
    ship.STI.Draft=ui->draft_lineEdit->text().toFloat();
    ship.STI.Displacement=ui->displacement_lineEdit->text().toFloat();
    ship.DYI.Course=ui->couse_lineEdit->text().toFloat();
    ship.DYI.Speed=ui->spped_lineEdit->text().toFloat();
    ship.DYI.Longitude=ui->longitude_lineEdit->text().toDouble();
    ship.DYI.longitude_real=ship.DYI.Longitude;
    ship.DYI.Latitude=ui->latitude_lineEdit->text().toDouble();
    ship.DYI.Latitude_real=ship.DYI.Latitude;
    shiplist.push_back(ship);
    for(int i=0;i!=shiplist.size();i++)
    {
        shiplist[i].ShipShape();
        ui->course_verticalSlider->setValue(ship.DYI.Course);
        ui->spped_verticalSlider->setValue(ship.DYI.Speed);

   }
    update();
    clearlineedit();


}

void MapWindow::on_deleteship_nutton_clicked()
{
    if(col)
    {
         shiplist.takeAt(numCol);
         col=false;
    }
    else
    {
        shiplist.pop_back();
    }


    if(shiplist.size()!=0)
    {

     ui->course_verticalSlider->setValue(shiplist[shiplist.size()-1].DYI.Course);
     ui->spped_verticalSlider->setValue(shiplist[shiplist.size()-1].DYI.Speed);
   }
    else
    {
        ui->course_verticalSlider->setValue(0);
        ui->spped_verticalSlider->setValue(0);
        ui->information_textEdit->setText("");
    }
     update();
}

void MapWindow::on_course_verticalSlider_valueChanged(int value)
{
    if(shiplist.size()!=0)
    {
        shiplist[shiplist.size()-1].DYI.Course=value;
        shiplist[shiplist.size()-1].ShipShape();

    }
    else
    {
        return;
    }
}

void MapWindow::on_spped_verticalSlider_valueChanged(int value)
{
    if(shiplist.size()!=0)
    {
        shiplist[shiplist.size()-1].DYI.Speed=value;

    }
    else
    {
        return;
    }
}



MapWindow::~MapWindow()
{
    delete ui;
}


void MapWindow::on_actionReturn_triggered()
{
    emit returndraw();
}


void MapWindow::on_actionopenPic_triggered()
{
    //弹出文件打开对话框
    QString picPath = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.jpg *.png )");
    if(!picPath.isEmpty())//用户选择了文件
    {
        _pixmap.load(picPath);
        update();//触发窗体重绘，将图片画到窗体
    }
}

void MapWindow::on_actionopenTxt_triggered()
{
    //弹出文件打开对话框
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开"),"","Text files (*.txt)");
    if(!fileName.isEmpty())//用户选择了文件
    {

        QFile f;
        f.setFileName(fileName);
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "文件打开失败！" ;
            return;
        }

        QDataStream in(&f);
        in>>obj._fill;//是否填充
        in>>obj._lights;//灯浮
        in>>obj._shape;//图形类型集合
        in>>obj._lines;//线条集合(一条线条可包含多个线段)
        in>>obj._line;//直线集合
        in>>obj._rects;//矩形集合
        in>>obj._ellipse;//椭圆集合
        _openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }
}

