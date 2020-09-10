#include "dialogreporthistory.h"
#include "ui_dialogreporthistory.h"


DialogReportHistory::DialogReportHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReportHistory)
{
    ui->setupUi(this);

}

DialogReportHistory::~DialogReportHistory()
{
    delete ui;
}

void DialogReportHistory::ConstructMyRecordModel(M_Record *r)
{
    //define horizontal header
    QStandardItemModel *m_model=new QStandardItemModel();
    QStringList h_headerList= QObject::trUtf8("Operands,Solution,Generate time").simplified().split(",");
    m_model->setHorizontalHeaderLabels(h_headerList);

    if(!r->Is_Read())
    {
        r->ReadHistory();
    }
    //fill the model
    QStandardItem *item_ptr;
    //push read queue
     int iter_row=0;
    while(!r->getReadBuffer()->empty())
    {
        string operandStr;
        for(int i=0;i<4;i++)
        {
            operandStr.append(to_string(r->getReadBuffer()->front().op[i]));
            operandStr.push_back(' ');
        }
        //has gotten all info now
        item_ptr =new QStandardItem(QString(operandStr.c_str()));
        m_model->setItem(iter_row,0,item_ptr);
        item_ptr=new QStandardItem(QString(r->getReadBuffer()->front().solution));
        m_model->setItem(iter_row,1,item_ptr);
        item_ptr=new QStandardItem(QString(r->getReadBuffer()->front().t));
        m_model->setItem(iter_row,2,item_ptr);
        iter_row++;
        r->getReadBuffer()->pop();
    }
    //look
    //push write queue
    queue<m_Entry> deplicate(*(r->getWriteBuffer()));
    while(!deplicate.empty())
    {
        string operandStr;
        for(int i=0;i<4;i++)
        {
            operandStr.append(to_string(deplicate.front().op[i]));
            operandStr.push_back(' ');
        }
        item_ptr=new QStandardItem(QString(operandStr.c_str()));
        m_model->setItem(iter_row,0,item_ptr);
        item_ptr=new QStandardItem(QString(deplicate.front().solution));
        m_model->setItem(iter_row,1,item_ptr);
        item_ptr=new QStandardItem(QString(deplicate.front().t));
        m_model->setItem(iter_row,2,item_ptr);
        iter_row++;
        deplicate.pop();
    }

    ui->tableView->setModel(m_model);
    ui->tableView->resizeColumnsToContents();
    saveRecordPtr(r);

}

void DialogReportHistory::on_DeleteRecord_clicked()
{

   rptr->DeleteRecordInDisk();
   //repaint the tableview
   //ui->tableView->clearSpans(); //it seems nothing happen

   ConstructMyRecordModel(rptr);  //reconstruct again
}

void DialogReportHistory::saveRecordPtr(M_Record *r)
{
    rptr=r;
}

void DialogReportHistory::on_pushButton_clicked()
{
    close();
}
