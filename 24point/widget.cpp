#include "widget.h"
#include "ui_widget.h"

#include<QTextCodec>




Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setup_the_MainStyle();

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8")); //problem remain,edited on 2020.03.02


    //setup the internal logic, connect function
    QList<QLineEdit *> List_LiEdt=InputBoxContainer->findChildren<QLineEdit *>(QString(),Qt::FindDirectChildrenOnly);
    QLineEdit *variable;
    foreach(variable,List_LiEdt)
    {
        variable->installEventFilter(this);
        connect(variable,SIGNAL(textChanged(QString)),this,SLOT(Input_Check()));
        //connect(variable,)
    }
    //other options
    ui->btn_verify_range->setEnabled(false);
    ui->radioButton_linearDistribution->setCheckable(false);
    ui->radioButton_linearDistribution->setEnabled(!ui->checkBox_PokerMode->isChecked());
    ui->radioButton_PokerDistribution->setEnabled(!ui->checkBox_PokerMode->isChecked());
    //
    connect(this->btn_find_solution,SIGNAL(clicked(bool)),this,SLOT(Btn_TrySolveClicked()));
    connect(this->btn_clr_all,SIGNAL(clicked(bool)),this,SLOT(Btn_clr_all_Clicked()));
    connect(ui->checkBox_PokerMode,SIGNAL(stateChanged(int)),this,SLOT(PokerMode_checkChange_controlRangeEnable()));
    connect(this->btn_ramdom_produce,SIGNAL(clicked(bool)),this,SLOT(Btn_random_produce_Clicked()));
    connect(this->btn_history_record,SIGNAL(clicked(bool)),this,SLOT(Btn_history_record_Clicked()));
    connect(ui->btn_verify_range,SIGNAL(clicked(bool)),this,SLOT(Btn_Verify_clicked()));
    connect(ui->spinBox_lowerbound,SIGNAL(valueChanged(int)),this,SLOT(ExtractionRange_Maintained()));
    connect(ui->spinBox_upperbound,SIGNAL(valueChanged(int)),this,SLOT(ExtractionRange_Maintained()));
    connect(ui->radioButton_PokerDistribution,SIGNAL(toggled(bool)),this,SLOT(updateExtractionState()));
    //initial the calculator object
    Calculator=new _24point_calculator();
    recordmanager=new M_Record();
    //other default settings
    upperbound=13; lowerbound=1;
    PokerMode_Selection=true;
    srand(time(0));

    //translate work

}

void Widget::setup_the_MainStyle()
{
    //set window's size
    this->setFixedSize(350,250);
    //set the window's title and icon;
    this->setWindowTitle(QString("24点计算器"));
    QPixmap window_ico(":/img/24point_icon.png");
    window_ico.scaled(QSize(32,32),Qt::KeepAspectRatio,Qt::FastTransformation);
    setWindowIcon(QIcon(window_ico));

    //set the window's element
    ButtonContainer=new QWidget(this,Qt::Widget);//Qt::WindowsFlag is enumeration type
    ButtonContainer->setLayout(new QVBoxLayout());
    //,this argument is a custom variant window's type selection;
    InputBoxContainer=new QWidget(this,Qt::Widget);
    QHBoxLayout *Layout_Inputbox=new QHBoxLayout();
    InputBoxContainer->setLayout(Layout_Inputbox);


    btn_clr_all=new QPushButton();
    btn_clr_all->setText(QString("清除"));

    btn_find_solution=new QPushButton();
    btn_find_solution->setText(QString("试图求解"));
    btn_ramdom_produce=new QPushButton();
    btn_ramdom_produce->setText("产生随机"); //为什么不能打“产生随机数” 经过尝试，“数”字不能出现在词尾 很是奇怪,那我就勉强按个空格进入字符串尾部
    btn_history_record=new QPushButton();
    btn_history_record->setText(QString("历史记录"));
    //ButtonContainer->children()
    //put the button in the widget's layout region;
    ButtonContainer->layout()->addWidget(btn_clr_all);
    ButtonContainer->layout()->addWidget(btn_find_solution);
    ButtonContainer->layout()->addWidget(btn_ramdom_produce);
    ButtonContainer->layout()->addWidget(btn_history_record);
    ButtonContainer->setGeometry(width()*3/4-5,5,90,150);
    //ButtonContainer->layout()->setMargin(2);
    //↑ while apply this setting, the following sentence is disabled or can be said ignored/invalid
    //unable to reset the margin of widgets
    ButtonContainer->layout()->setSpacing(2);//0 cannot be set , why??
    btn_find_solution->setEnabled(false); //disable it first
    //set up the inputbox
    LineBox1=new QLineEdit();
    LineBox2=new QLineEdit();
    LineBox3=new QLineEdit();
    LineBox4=new QLineEdit();
    //LineBox1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    //LineBox2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    //LineBox3->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    LineBox4->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);


    InputBoxContainer->layout()->addWidget(LineBox1);
    InputBoxContainer->layout()->addWidget(LineBox2);
    InputBoxContainer->layout()->addWidget(LineBox3);
    InputBoxContainer->layout()->addWidget(LineBox4);
    InputBoxContainer->setGeometry(10,10,width()*3/4-30,40);
    QIntValidator *default_IntValidator=new QIntValidator(1,13);
    Layout_Inputbox->setAlignment(Qt::AlignCenter);
    //Layout_Inputbox->setGeometry(QRect(10,5,InputBoxContainer->width(),40));



    InputBoxContainer->layout()->setSpacing(20);
    Layout_Inputbox->setContentsMargins(Layout_Inputbox->contentsMargins().left(),0,
                                        Layout_Inputbox->contentsMargins().right(),0);

    //setup LineEdit size and font attributes
    QLineEdit *t_Linedit_ptr;
    QFont Font_for_LineEdit("Mircosoft JhengHei",22);

    //iterator problem:
    //discription: children().begin() return a const QObjectList::iterator
    //Nevertheless, it is an empty iterator,which means this iterator equal children().end();
    //we
    QObjectList::const_iterator Iter_for_objLi=InputBoxContainer->children().begin();
    QObjectList::const_iterator Iter_end=InputBoxContainer->children().end();

    while(Iter_for_objLi!=Iter_end)
    {
        t_Linedit_ptr=dynamic_cast<QLineEdit *>(*Iter_for_objLi);
        if(t_Linedit_ptr!=nullptr)
        {
        t_Linedit_ptr->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        t_Linedit_ptr->setFont(Font_for_LineEdit);
        t_Linedit_ptr->setAlignment(Qt::AlignCenter);
        t_Linedit_ptr->setMaxLength(2);

        //以下正则表达式均为default ，构造函数体内
        t_Linedit_ptr->setValidator(default_IntValidator);
        }
        Iter_for_objLi++;
    }
    //Layout_Inputbox->setMargin(0);
    //InputBoxContainer->setContentsMargins(2,2,20,2);
   // QObjectList objLi=InputBoxContainer->layout()->children();

    //Layout_Inputbox->setStretchFactor(Layout_Inputbox,2); how to use this function and when? it's still need to be considered.
    //setup the info panel
    setup_the_infopanel_v1();






}

void Widget::setup_the_infopanel_v1()
{
    QFrame *Info_panelPtr=new QFrame(this);
    Info_panelPtr->setFrameStyle(QFrame::StyledPanel|QFrame::Plain);
    Info_panelPtr->setGeometry(10,InputBoxContainer->pos().y()+InputBoxContainer->height()+3,
                               InputBoxContainer->width(),(height()-InputBoxContainer->height())/2);
    c_label=new QLabel(QString("Input 4 operands or generate random number"),Info_panelPtr); //标签设置中文又双叒叕乱码了
    c_label->setWordWrap(true);
    //c_label->setFrameShape(QFrame::Box);// used to check the position

    Answer_label=new QLabel(Info_panelPtr);
    //QLayout *info_layout=new QLayout();
    //info_layout->addWidget(c_label);    info_layout->addWidget(Answer_label);
    //Info_panelPtr->setLayout(info_layout);
    //Info_panelPtr;
    //c_label->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    c_label->setGeometry(Info_panelPtr->pos().x(),10,Info_panelPtr->width()-20,Info_panelPtr->height()/4);
    c_label->resize(QSize(c_label->width(),c_label->height()*2));
    QFont info_font_T_N_R("Times new Roman",15);
    c_label->setFont(info_font_T_N_R);
    info_font_T_N_R.setPointSize(20);
    Answer_label->setFont(info_font_T_N_R);
    Answer_label->setGeometry(c_label->pos().x(),c_label->pos().y()+c_label->height()-10,c_label->width(),c_label->height()/2+5);
    //Answer_label->setFrameShape(QFrame::Box);
    Answer_label->setText(""); //Answer_label->move(Answer_label->pos()-QPoint(0,30));
    Answer_label->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
    Answer_label->setAlignment(Qt::AlignTop);
    //my test code, convenient to find the problem
    //Answer_label->setFrameStyle(Info_panelPtr->frameStyle());
    //c_label->setFrameStyle(Info_panelPtr->frameStyle());
}


void Widget::Btn_TrySolveClicked()
{
    //data validity verified after
    Opd_transmit(); //pass operands

    //for the safety fo operation
    if(!Calculator->Source_isEmpty())
    {
        std::string Answer_str;
        int select_dialog=LineBox1->text().toInt()%5;
        if(Calculator->infinitely_enum_and_print(Answer_str))
        {
        c_label->setText(Find_SolutionRespond[select_dialog]);
        Answer_label->setText(QString(Answer_str.c_str()));

        }
        else
        {
            select_dialog=LineBox2->text().toInt()%5;
            c_label->setText(No_solution_Respond[select_dialog]);
            Answer_label->setText(QString(Answer_str.c_str()));
        }

        //record info
        int t_Operand[4]={
            Calculator->Operand(1),Calculator->Operand(2),Calculator->Operand(3),Calculator->Operand(4)
        };
        recordmanager->RecordInBuffer(t_Operand,Answer_str,time(0));
    }
}


void Widget::Opd_transmit()
{
    //validity checked after
    int opd[4];
    opd[0]=LineBox1->text().toInt();
    opd[1]=LineBox2->text().toInt();
    opd[2]=LineBox3->text().toInt();
    opd[3]=LineBox4->text().toInt();
    Calculator->setNumber(opd[0],opd[1],opd[2],opd[3]);
}

bool Widget::Input_Check()
{
    QLineEdit *t_ptr;
    QList<QLineEdit *> tempCtn=InputBoxContainer->findChildren<QLineEdit *>(QString(),Qt::FindDirectChildrenOnly);
    bool ok=true;
    foreach(t_ptr,tempCtn)
    {
        if(t_ptr->text().isEmpty())
        {
            ok=false;
            break;
        }
    }
    btn_find_solution->setEnabled(ok); // 使用if-else 结构代价更少还是直接调用更省？?? I dont know
    return ok;  //返回值没啥用， 留着吧。。。
}

void Widget::Btn_clr_all_Clicked()
{
    QList<QLineEdit *> Li_LineEdits=InputBoxContainer->findChildren<QLineEdit *>(QString(),Qt::FindDirectChildrenOnly);
    QLineEdit *v;
    foreach(v,Li_LineEdits)
    {
        v->clear();
    }
    c_label->setText(QString("Input 4 operands or generate random number"));
    Answer_label->clear();

}

void Widget::Btn_history_record_Clicked()
{
    d=new DialogReportHistory;  //no delete keyword in the following context, emmm
    d->ConstructMyRecordModel(recordmanager);
    d->show(); //display fail
}

void Widget::Btn_random_produce_Clicked()
{
    if(PokerMode_Selection)
    {
        Calculator->produce_an_order(Distribution::Poker,lowerbound,upperbound);

    }
    else
    {
        Calculator->produce_an_order(Distribution::EqualProbability,lowerbound,upperbound);
    }

    LineBox1->setText(QString(to_string( Calculator->Operand(1)).c_str()));
    LineBox2->setText(QString(to_string(Calculator->Operand(2)).c_str()));
    LineBox3->setText(QString(to_string(Calculator->Operand(3)).c_str()));
    LineBox4->setText(QString(to_string(Calculator->Operand(4)).c_str()));
}

void Widget::Limit_the_Num_range_selected(int arg_lowerbound, int arg_upperbound)
{
    upperbound=arg_upperbound;
    lowerbound=arg_lowerbound;
}

void Widget::Btn_Verify_clicked()
{
    Limit_the_Num_range_selected(ui->spinBox_lowerbound->value(),ui->spinBox_upperbound->value());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::PokerMode_checkChange_controlRangeEnable()
{
    ui->btn_verify_range->setEnabled(!ui->checkBox_PokerMode->isChecked());
    ui->radioButton_PokerDistribution->setChecked(ui->checkBox_PokerMode->isChecked()); //it is so suck that setchecked() only reacts to true state
    ui->radioButton_linearDistribution->setCheckable(!ui->checkBox_PokerMode->isChecked());
    ui->radioButton_PokerDistribution->setEnabled(!ui->checkBox_PokerMode->isChecked());
    ui->radioButton_linearDistribution->setEnabled(!ui->checkBox_PokerMode->isChecked());
    if(ui->checkBox_PokerMode->isChecked())
    {
        Limit_the_Num_range_selected(1,13);
        ui->spinBox_lowerbound->setValue(1);
        ui->spinBox_upperbound->setValue(13);
    }
}

void Widget::keyPressEvent(QKeyEvent *K_event)
{

    switch(K_event->key())
    {
    case Qt::Key_Return:
        if(btn_find_solution->isEnabled())
        {
        emit btn_find_solution->click();
        }
        break;
    case Qt::Key_Enter:
        if(btn_find_solution->isEnabled())
        {
        emit btn_find_solution->click();
        }
        break;
    case Qt::Key_Tab:       //concentrate the focus on InputboxContainer's children
        LineBox1->setFocus();
        break;
    default:
        K_event->ignore();
        break;
    }
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched->inherits("QLineEdit")) // problem solved, then how to distinguish the type ? decltype
    {
        if(event->type()==QEvent::FocusIn)
        {
            QLineEdit *handle=qobject_cast<QLineEdit *>(watched);
            FocusIn_EditBox_Proc(handle);
            return false;
        }
        else if(event->type()==QEvent::KeyPress)  //这里假设了事件处理是用焦点窗口开始的
        {
            QKeyEvent *ev=static_cast<QKeyEvent *>(event);
            if((ev->key()==Qt::Key_Enter)||(ev->key()==Qt::Key_Return)||
                    (ev->key()==Qt::Key_Tab&&watched==LineBox4))
            {
                keyPressEvent(ev);   //dealed by Widget's KeyPressEvent
                return true;   //stop it pass to watched_obj
            }
            else
            {
                return false;  //pass to QlineEdit
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
       return false; //no filter
    }
}

void Widget::FocusIn_EditBox_Proc(QLineEdit *hand) //使用focusIn触发，能让Tab起效，比起鼠标事件
{
    if(!hand->text().isEmpty())
    {
        //hand->selectAll(); //here is a problem, mousePressEvent will deselect the text
        QTimer::singleShot(0,hand,SLOT(selectAll()));
        qDebug()<<"select ALL";  //add an prepocess command to dis/enable qDebug()
    }
}

void Widget::ExtractionRange_Maintained()
{
    ui->spinBox_lowerbound->setRange(1,ui->spinBox_upperbound->value()-1);
    ui->spinBox_upperbound->setRange(ui->spinBox_lowerbound->value()+1,13);
}

void Widget::updateExtractionState()
{
    PokerMode_Selection=ui->radioButton_PokerDistribution->isChecked();
}

void Widget::closeEvent(QCloseEvent *event)
{
    recordmanager->WriteHistory(); //save the buffer
}
