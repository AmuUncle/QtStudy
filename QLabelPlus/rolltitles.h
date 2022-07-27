#ifndef ROLLTITLES_H
#define ROLLTITLES_H

#include <QWidget>

class RollTitles : public QWidget
{
    Q_OBJECT
public:
    explicit RollTitles(QWidget *parent = nullptr, QString strTitle = "");

public:
    void SetTitle(QString strTitle);

private:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

    void UpdateRollingState();

private:
    QString  m_strTitle;
    int m_nLeftOffset;
    int m_nTimerId;
};

#endif // ROLLTITLES_H
