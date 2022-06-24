#ifndef BEAUTIFULTABLE_H
#define BEAUTIFULTABLE_H

#include <QDialog>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QVBoxLayout>

class BeautifulTable : public QDialog
{
    Q_OBJECT

public:
    BeautifulTable(QWidget *parent = 0);
    ~BeautifulTable();

private:
    void InitCtrl();
    void AddData();
    void SetStyle();

    void timerEvent(QTimerEvent *event);

private:
    QTableView *m_pTableView;
    QStandardItemModel *m_pModel;
};

#endif // BEAUTIFULTABLE_H
