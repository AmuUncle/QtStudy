#ifndef MAINWND_H
#define MAINWND_H

#include <QWidget>

class MainWnd : public QWidget
{
    Q_OBJECT
public:
    explicit MainWnd(QWidget *parent = 0);

private:
    void CreateAllChildWnd();
    void InitCtrl();
    void InitSolts();
    void Relayout();

    bool eventFilter(QObject *watched, QEvent *evt);
};

#endif // MAINWND_H
