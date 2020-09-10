#ifndef DIALOGREPORTHISTORY_H
#define DIALOGREPORTHISTORY_H

#include <QDialog>
#include<QtWidgets>

#include"m_record.h"

namespace Ui {
class DialogReportHistory;
}

class DialogReportHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReportHistory(QWidget *parent = 0);
     void ConstructMyRecordModel(M_Record *r);
    ~DialogReportHistory();


private slots:
     void on_DeleteRecord_clicked();

     void on_pushButton_clicked();

private:
    void saveRecordPtr(M_Record *r);

    M_Record *rptr;
    Ui::DialogReportHistory *ui;
};

#endif // DIALOGREPORTHISTORY_H
