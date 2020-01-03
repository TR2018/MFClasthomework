#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     this->setWindowTitle("大作业");

    _lpress = false;//初始鼠标左键未按下
    _drawType = 0;//初始为什么都不画
    _openflag = 0;//初始不打开图片
    obj._fill=false;//初始没有填充

    mapwindow=new MapWindow();//初始化海图测试窗口


    this->setGeometry(350,200,600,400);//设置窗体大小、位置
    setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听

    _Rmenu = new QMenu(this);//创建右键菜单
    _Rmenu->addAction(tr("保存  \tCtrl+S"), this, SLOT(SavePic()));//添加菜单动作
    _Rmenu->addAction(tr("退出  \tALT+F4"), this, SLOT(close()));//添加菜单动作
    _Rmenu->setStyleSheet("background-color:rgb(199,237,204)");//设置背景色

   //设置工具栏
    ui->mainToolBar->setIconSize(QSize(16, 16));//设置动作图标的尺寸
    ui->mainToolBar->setStyleSheet("background-color:rgb(199,237,204)");//背景色

    QAction *openTxtAction = new QAction(tr("&打开文档"), this);//打开序列化动作
    openTxtAction->setIcon(QIcon(":/res/open.png"));//图标
    openTxtAction->setShortcut(QKeySequence(tr("Ctrl+O")));//热键
    ui->mainToolBar->addAction(openTxtAction);//添加到工具栏

    QAction *openPicAction = new QAction(tr("&打开图片"), this);//打开动作
    openPicAction->setIcon(QIcon(":/res/open.png"));//图标
    openPicAction->setShortcut(QKeySequence(tr("Ctrl+0")));//热键
    ui->mainToolBar->addAction(openPicAction);//添加到工具栏

    QAction *saveTxtAction = new QAction(tr("&序列化保存"), this);//序列化保存动作
    saveTxtAction->setIcon(QIcon(":/res/save.png"));//图标
    saveTxtAction->setShortcut(QKeySequence(tr("Ctrl+S")));//热键
    ui->mainToolBar->addAction(saveTxtAction);//添加到工具栏

    QAction *savePicAction = new QAction(tr("&保存图片"), this);//保存图片动作
    savePicAction->setIcon(QIcon(":/res/save.png"));//图标
    savePicAction->setShortcut(QKeySequence(tr("Ctrl+S+0")));//热键
    ui->mainToolBar->addAction(savePicAction);//添加到工具栏

    QAction *lineAction = new QAction(tr("&直线"), this);//直线动作
    lineAction->setIcon(QIcon(":/res/line.png"));//图标
    lineAction->setShortcut(QKeySequence(tr("Ctrl+L")));//热键
    ui->mainToolBar->addAction(lineAction);//添加到工具栏

    QAction *linesAction = new QAction(tr("&铅笔"), this);//铅笔动作
    linesAction->setIcon(QIcon(":/res/lines.png"));//图标
    linesAction->setShortcut(QKeySequence(tr("Ctrl+P")));//热键
    ui->mainToolBar->addAction(linesAction);//添加到工具栏

    QAction *rectAction = new QAction(tr("& 矩形"), this);//矩形动作
    rectAction->setIcon(QIcon(":/res/rect.png"));//图标
    rectAction->setShortcut(QKeySequence(tr("Ctrl+R")));//热键
    ui->mainToolBar->addAction(rectAction);

    QAction *ellipseAction = new QAction(tr("&椭圆"), this);//椭圆动作
    ellipseAction->setIcon(QIcon(":/res/ellipse.png"));//图标
    ellipseAction->setShortcut(QKeySequence(tr("Ctrl+E")));//热键
    ui->mainToolBar->addAction(ellipseAction);

    QAction *lightAction = new QAction(tr("&灯浮"), this);//椭圆动作
    lightAction->setIcon(QIcon(":/res/112.png"));//图标
    lightAction->setShortcut(QKeySequence(tr("Ctrl+p")));//热键
    ui->mainToolBar->addAction(lightAction);


    QAction *fillingAction = new QAction(tr("&填充"), this);//填充动作
    fillingAction->setIcon(QIcon(":/res/1.png"));//图标
    fillingAction->setShortcut(QKeySequence(tr("Ctrl+F")));//热键
    ui->mainToolBar->addAction(fillingAction);


    //连接信号与槽函数
    connect(linesAction, SIGNAL(triggered()), this, SLOT(Lines()));
    connect(rectAction, SIGNAL(triggered()), this, SLOT(Rects()));
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(Ellipses()));
    connect(lineAction, SIGNAL(triggered()), this, SLOT(Line()));
    connect(saveTxtAction, SIGNAL(triggered()), this, SLOT(SaveTxt()));
    connect(savePicAction,SIGNAL(triggered()),this,SLOT(SavePic()));
    connect(openTxtAction, SIGNAL(triggered()), this, SLOT(OpenTxt()));
    connect(openPicAction, SIGNAL(triggered()), this, SLOT(OpenPic()));
    connect(fillingAction,SIGNAL(triggered()),this,SLOT(FillPic()));
    connect(lightAction, SIGNAL(triggered()), this, SLOT(ligh()));



    connect(mapwindow,&MapWindow::returndraw,[=](){
        mapwindow->hide();
        this->show();
    });//返回画图窗口
}


void MainWindow::paintEvent(QPaintEvent *)
{
    //QPainter p1(this);
    if(_openflag == 0)//不是打开图片的，每一次新建一个空白的画布
    {
        _pixmap=QPixmap(this->size());
        _pixmap.fill(QColor(152,211,237));
    }

    QPixmap pix= _pixmap;
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
                //qDebug()<<line.at(j+1);
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

    p.end();
    p.begin(this);
    p.drawPixmap(0,0,this->width(),this->height(),pix);
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        if(_drawType == 1)//线条(铅笔)
        {
            _lpress = true;//左键按下标志
            QVector<QPoint> line;//鼠标按下，新的线条开始
            obj._lines.append(line);//将新线条添加到线条集合
            QVector<QPoint>& lastLine = obj._lines.last();//拿到新线条
            lastLine.append(e->pos());//记录鼠标的坐标(新线条的开始坐标)
            obj._shape.append(1);

        }
        else if(_drawType == 2)//矩形
        {
            _lpress = true;//左键按下标志

                QRect rect;//鼠标按下，矩形开始
               obj._rects.append(rect);//将新矩形添加到矩形集合
                QRect& lastRect = obj._rects.last();//拿到新矩形
                lastRect.setTopLeft(e->pos());//记录鼠标的坐标(新矩形的左上角坐标)
                 obj._shape.append(2);


        }
        else if(_drawType == 3)//椭圆
        {
            _lpress = true;//左键按下标志

                QRect rect;//鼠标按下，椭圆开始
                obj._ellipse.append(rect);//将新椭圆添加到椭圆集合
                QRect& lastEllipse = obj._ellipse.last();//拿到新椭圆
                lastEllipse.setTopLeft(e->pos());//记录鼠标的坐标(新椭圆的左上角坐标)
                 obj._shape.append(3);

        }
        else if(_drawType == 4)//直线
        {
            _lpress = true;//左键按下标志
            QRect rect;//鼠标按下，直线一端开始
            obj._line.append(rect);//将新直线添加到直线集合
            QRect& lastLine = obj._line.last();//拿到新直线
            lastLine.setTopLeft(e->pos());//记录鼠标的坐标(新直线开始一端坐标)
            obj._shape.append(4);
        }
        else if(_drawType==5)//灯浮
        {
            _lpress = true;
            QPoint light;//鼠标按下，新的灯浮点
            light=e->pos();
            obj._lights.append(light);
            obj._shape.append(5);
            update();
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{


    if(_lpress)
    {
        if(_drawType == 1)//铅笔画线
        {
            if( obj._lines.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = obj._lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录鼠标的坐标(线条的轨迹)
            update();//触发窗体重绘
        }
        else if(_drawType == 2)
        {

                QRect& lastRect = obj._rects.last();//拿到新矩形
                lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标


            update();//触发窗体重绘

        }
        else if(_drawType == 3)
        {

                QRect& lastEllipse = obj._ellipse.last();//拿到新椭圆
                lastEllipse.setBottomRight(e->pos());//更新椭圆的右下角坐标)



            update();//触发窗体重绘
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = obj._line.last();//拿到新直线
            lastLine.setBottomRight(e->pos());//更新直线另一端)
            update();//触发窗体重绘
        }
    }


}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(_lpress)
    {
        if(_drawType == 1)
        {
            QVector<QPoint>& lastLine = obj._lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录线条的结束坐标
            _lpress = false;//标志左键释放
        }
        else if(_drawType == 2)
        {
            QRect& lastRect = obj._rects.last();//拿到新矩形

                lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标


            _lpress = false;

        }
        else if(_drawType == 3)
        {
            QRect& lastEllipse = obj._ellipse.last();//拿到新椭圆

                lastEllipse.setBottomRight(e->pos());//更新椭圆的右下角坐标


            _lpress = false;
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = obj._line.last();//拿到新矩形
            lastLine.setBottomRight(e->pos());//更新矩形的右下角坐标)
            _lpress = false;

        }
    }
}
void MainWindow::Lines()
{
    _drawType = 1;//铅笔
}

void MainWindow::Line()
{
    _drawType = 4;//直线
}

void MainWindow::Rects()
{
    _drawType = 2;//矩形

}

void MainWindow::Ellipses()
{
    _drawType = 3;//椭圆
}

void MainWindow::ligh()
{
    _drawType = 5;//灯浮
}

void MainWindow::FillPic()
{
     obj._fill= !obj._fill;//填充

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *)  //右键菜单事件
{
    _Rmenu->exec(cursor().pos());//在光标位置弹出菜单
}

void MainWindow::keyPressEvent(QKeyEvent *e) //按键事件
{
     //Ctrl+Z撤销
     if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier)
     {
         if(obj._shape.size()>0)
         {
             switch(obj._shape.last())
             {
                 case 1: obj._lines.pop_back();
                         break;
                 case 2: obj._rects.pop_back();
                         break;
                 case 3: obj._ellipse.pop_back();
                         break;
                 case 4: obj._line.pop_back();
                         break;
             }
             obj._shape.pop_back();

             update();
         }
     }
     else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)//保存
     {
        SavePic();//Ctrl+S保存
     }
}


void MainWindow::SaveTxt()//序列化保存
{
    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new", "Text files (*.txt)");

    if (fileName.length() > 0)
    {


        QFile f;
        f.setFileName(fileName);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "文件打开失败！" ;
            return;
        }
        else
        {
            qDebug()<<"打开成功";
        }
        QDataStream out(&f);
        out<<obj._fill;//是否填充
        out<<obj._lights;//灯浮
        out<<obj._shape;//图形类型集合
        out<<obj._lines;//线条集合(一条线条可包含多个线段)
        out<<obj._line;//直线集合
        out<<obj._rects;//矩形集合
        out<<obj._ellipse;//椭圆集合

        f.close();
        qDebug()<<"序列化保存成功！";
    }
}

void MainWindow::SavePic()//保存成图片
{
    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new", "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(0,ui->mainToolBar->height()+ui->menuBar->height(),size().width(),size().height()-ui->mainToolBar->height()-ui->menuBar->height())).save(fileName);//不包含菜单栏工具栏
       //qDebug()<<ui->mainToolBar->height()+ui->menuBar->height();
    }
}

void MainWindow::OpenTxt()//打开序列化文档
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

void MainWindow::OpenPic()//打开图片
{
    //弹出文件打开对话框
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.jpg *.png)");
    if(!fileName.isEmpty())//用户选择了文件
    {
        QPixmap pix;
        pix.load(fileName);//加载图片
        QPainter p(&_pixmap);
        p.drawPixmap(0,30,pix);//添加工具栏的空间
        _openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }
}

void MainWindow::on_actionMap_triggered()
{
    this->hide();
    mapwindow->show();
}
