#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void AddNavBar(QString strTitle, QChar icon, QWidget *w);

private:
    Ui::MainWindow *ui;

    QGridLayout *m_mainVLayout;
};

#endif // MAINWINDOW_H
