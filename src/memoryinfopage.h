#ifndef MEMORYINFOPAGE_H
#define MEMORYINFOPAGE_H

#include <QWidget>

class QGroupBox;
class QLabel;
class QProgressBar;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

class MemoryInfoPage : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryInfoPage(QWidget *parent = 0);

signals:

public slots:

private:
    QGroupBox *m_pGroupBoxCPU;
    QGroupBox *m_pGroupBoxMemory;
    QLabel *m_pLabelCPU;
    QProgressBar *m_pProgressBarCPU;
    QLabel *m_pLabelMemory;
    QProgressBar *m_pProgressBarMemory;
    QLabel *m_pLabelMemoryUsed;
    QLabel *m_pLabelMemoryLeft;
    QLabel *m_pLabelMemoryTotal;
    QLabel *m_pLabelSwap;
    QProgressBar *m_pProgressBarSwap;
    QLabel *m_pLabelSwapUsed;
    QLabel *m_pLabelSwapLeft;
    QLabel *m_pLabelSwapTotal;
    QHBoxLayout *m_pHBoxLayoutCPU;
    QGridLayout *m_pGridLayoutMemory;
    QVBoxLayout *m_pVBoxLayoutMain;

    QTimer *m_pTimer;
private slots:
    void on_timer_timeout();



};

#endif // MEMORYINFOPAGE_H
