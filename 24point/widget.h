#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtWidgets>

#include"_24point_calculator.h"
#include"m_record.h"
#include"dialogreporthistory.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    void setup_the_MainStyle();

    ~Widget();

    void LineEditBox_PokerMode(bool Enable_signal);
    void LinearDistribution_totality(bool Enable_signal);
    void Limit_the_Num_range_selected(int upperbound, int lowerbound);   


public slots:
    void Btn_TrySolveClicked();
    void Btn_clr_all_Clicked();
    void Btn_random_produce_Clicked();
    void Btn_history_record_Clicked();
    void Btn_Verify_clicked();
//    void PokerMode_checked();
    void keyPressEvent(QKeyEvent *K_event);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
protected slots:
    bool Input_Check();   //check all LineEditBox
    void PokerMode_checkChange_controlRangeEnable();
    void updateExtractionState();
    void ExtractionRange_Maintained();


private:
    Ui::Widget *ui;
    void setup_the_infopanel_v1();
    void setup_the_infopanel_v2(); //upgrade the infopanel by the form of diaglog

    //internal logic
    void Opd_transmit(); //传递操作数

    void FocusIn_EditBox_Proc(QLineEdit *hand);
    void closeEvent(QCloseEvent *event);
    //

    //msg respond
    void decreaseAnswerBracket(); //重构回答的函数，目前还没实现，属于语法分析级别。
    //

    bool PokerMode_Selection; //used to determine the probability of numbers selected for totality;
    bool PokerMode_Input_EN; //used to constrain the content they enter, true by default;
    short upperbound, lowerbound; //抽取的上下限






    //my basic ui elements
     QWidget *ButtonContainer,*InputBoxContainer;
     QPushButton *btn_clr_all, *btn_find_solution,*btn_history_record,*btn_ramdom_produce;
     QLineEdit *LineBox1;
     QLineEdit *LineBox2;
     QLineEdit *LineBox3;
     QLineEdit *LineBox4;
     QLabel *c_label,*Answer_label;


    //after consideration, I think it is better to include the Ui elements in this header/class
    _24point_calculator *Calculator;
    M_Record *recordmanager;
    DialogReportHistory* d;
};

#endif // WIDGET_H
