#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>
#include <QLabel>

class Layout : public QWidget
{
    Q_OBJECT
public:
    explicit Layout(int nStyleType = 0, QWidget *parent = nullptr);

private:
    void Relayout();

private:
    int m_nStyleType;

    QLabel *m_pHeader;
    QLabel *m_pAside;
    QLabel *m_pMain;
    QLabel *m_pFooter;
};

#endif // LAYOUT_H
