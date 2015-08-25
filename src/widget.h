#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class MemoryInfoPage;
class ProcessInfoPage;

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

public slots:
    void on_pushButtonHalt_clicked();
    void on_pushButtonReboot_clicked();

private:
    QVBoxLayout *m_pVBoxLayoutMain;
    QHBoxLayout *m_pHBoxLayoutPower;
    QTabWidget *m_pTabWidget;
    QPushButton *m_pPushButtonHalt;
    QPushButton *m_pPushButtonReboot;

    MemoryInfoPage *m_pMemoryInfoPage;
    ProcessInfoPage *m_pProcessInfoPage;

};

#endif // WIDGET_H
