#ifndef PROCESSINFOPAGE_H
#define PROCESSINFOPAGE_H

#include <QWidget>

class QListWidget;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QLabel;

class ProcessInfoPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessInfoPage(QWidget *parent = 0);

signals:

public slots:
    void on_pushButtonKill_clicked();
    void on_pushButtonRefresh_clicked();

private:
    QListWidget *m_pListWidgetProcess;
    QPushButton *m_pPushButtonKill;
    QPushButton *m_pPushButtonRefresh;
    QLabel *m_pLabelProcessNumber;
    QLabel *m_pLabelProcessRunning;
    QLabel *m_pLabelProcessSleeping;
    QLabel *m_pLabelProcessZombie;
    QVBoxLayout *m_pVBoxLayoutLabel;
    QHBoxLayout *m_pHBoxLayoutButton;
    QGridLayout *m_pGridLayoutMain;

    void __ShowTabInfo();

};

#endif // PROCESSINFOPAGE_H
