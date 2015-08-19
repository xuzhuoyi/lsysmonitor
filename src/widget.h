#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class MemoryInfoPage;

class QTabWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QVBoxLayout *m_pVBoxLayoutMain;
    QHBoxLayout *m_pHBoxLayoutPower;
    QTabWidget *m_pTabWidget;
    QPushButton *m_pPushButtonHalt;
    QPushButton *m_pPushButtonReboot;

    MemoryInfoPage *m_pMemoryInfoPage;
};

#endif // WIDGET_H