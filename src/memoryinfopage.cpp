#include "memoryinfopage.h"

#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QFile>
#include <QMessageBox>

qint32 a0 = 0, a1 = 0, b0 = 0, b1 = 0;

MemoryInfoPage::MemoryInfoPage(QWidget *parent) :
    QWidget(parent)
{
    m_pGroupBoxCPU = new  QGroupBox(tr("CPU"));
    m_pGroupBoxMemory = new QGroupBox(tr("Memory and Swap"));
    m_pLabelCPU = new QLabel(tr("CPU"));
    m_pProgressBarCPU = new QProgressBar;
    m_pLabelMemory = new QLabel(tr("Memory"));
    m_pProgressBarMemory = new QProgressBar;
    m_pLabelMemoryUsed = new QLabel(tr("Used:"));
    m_pLabelMemoryLeft = new QLabel(tr("Left:"));
    m_pLabelMemoryTotal = new QLabel(tr("Total:"));
    m_pLabelSwap = new QLabel(tr("Swap"));
    m_pProgressBarSwap = new QProgressBar;
    m_pLabelSwapUsed = new QLabel("Used:");
    m_pLabelSwapLeft = new QLabel(tr("Left"));
    m_pLabelSwapTotal = new QLabel(tr("Total:"));
    m_pHBoxLayoutCPU= new QHBoxLayout;
    m_pGridLayoutMemory = new QGridLayout;
    m_pVBoxLayoutMain = new QVBoxLayout;

    m_pLabelMemory->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLabelSwap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_pHBoxLayoutCPU->addWidget(m_pLabelCPU);
    m_pHBoxLayoutCPU->addWidget(m_pProgressBarCPU);
    m_pGroupBoxCPU->setLayout(m_pHBoxLayoutCPU);
    m_pGridLayoutMemory->addWidget(m_pLabelMemory, 0, 0, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pProgressBarMemory, 0, 1, 1, 3);
    m_pGridLayoutMemory->addWidget(m_pLabelMemoryUsed, 1, 1, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pLabelMemoryLeft, 1, 2, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pLabelMemoryTotal, 1, 3, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pLabelSwap, 2, 0, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pProgressBarSwap, 2, 1, 1, 3);
    m_pGridLayoutMemory->addWidget(m_pLabelSwapUsed, 3, 1, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pLabelSwapLeft, 3, 2, 1, 1);
    m_pGridLayoutMemory->addWidget(m_pLabelSwapTotal, 3, 3, 1, 1);
    m_pGroupBoxMemory->setLayout(m_pGridLayoutMemory);
    m_pVBoxLayoutMain->addWidget(m_pGroupBoxCPU);
    m_pVBoxLayoutMain->addWidget(m_pGroupBoxMemory);

    setLayout(m_pVBoxLayoutMain);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    m_pTimer->start(1000);
}

void MemoryInfoPage::on_timer_timeout()
{
    QString tempStr;
    QFile tempFile;
    qint32 pos;

    {
            tempFile.setFileName("/proc/meminfo"); //打开内存信息文件
            if ( !tempFile.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(this, tr("warning"), tr("The meminfo file can not open!"), QMessageBox::Yes);
                return ;
            }
            QString memTotal;
            QString memFree;
            QString memUsed;
            QString swapTotal;
            QString swapFree;
            QString swapUsed;
            int nMemTotal, nMemFree, nMemUsed, nSwapTotal, nSwapFree, nSwapUsed;

            while (1)
            {
                tempStr = tempFile.readLine();
                pos = tempStr.indexOf("MemTotal");
                if (pos != -1)
                {
                    memTotal = tempStr.mid(pos+10, tempStr.length()-13);
                    memTotal = memTotal.trimmed();
                    nMemTotal = memTotal.toInt()/1024;
                }
                else if (pos = tempStr.indexOf("MemFree"), pos != -1)
                {
                    memFree = tempStr.mid(pos+9, tempStr.length()-12);
                    memFree = memFree.trimmed();
                    nMemFree = memFree.toInt()/1024;
                }
                else if (pos = tempStr.indexOf("SwapTotal"), pos != -1)
                {
                    swapTotal = tempStr.mid(pos+11, tempStr.length()-14);
                    swapTotal = swapTotal.trimmed();
                    nSwapTotal = swapTotal.toInt()/1024;
                }
                else if (pos = tempStr.indexOf("SwapFree"), pos != -1)
                {
                    swapFree = tempStr.mid(pos+10,tempStr.length()-13);
                    swapFree = swapFree.trimmed();
                    nSwapFree = swapFree.toInt()/1024;
                    break;
                }
            }

            nMemUsed = nMemTotal - nMemFree;
            nSwapUsed = nSwapTotal - nSwapFree;

            memUsed = QString::number(nMemUsed, 10);
            swapUsed = QString::number(nSwapUsed, 10);
            memFree = QString::number(nMemFree, 10);
            memTotal = QString::number(nMemTotal, 10);
            swapFree = QString::number(nSwapFree, 10);
            swapTotal = QString::number(nSwapTotal, 10);

            m_pLabelMemoryUsed->setText("Used: " + memUsed + " MB");
            m_pLabelMemoryLeft->setText("Left: " + memFree + " MB");
            m_pLabelMemoryTotal->setText("Total: " + memTotal + " MB");
            m_pLabelSwapUsed->setText("Used: " + swapUsed + " MB");
            m_pLabelSwapLeft->setText("Left: " + swapFree + " MB");
            m_pLabelSwapTotal->setText("Total: " + swapTotal + " MB");

            m_pProgressBarMemory->setValue(nMemUsed*100/nMemTotal);
            m_pProgressBarSwap->setValue(nSwapUsed*100/nSwapTotal);

            tempFile.close(); //关闭内存信息文件

            int tt = 2; //取2个点采样计算cpu当前利用律
            int cpuInfo[2][7];
            int cpuTotal[2][2];
            while (tt)
            {
                tempFile.setFileName("/proc/stat"); //打开CPU使用状态信息
                if ( !tempFile.open(QIODevice::ReadOnly) )
                {
                    QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
                    return;
                }
                tempStr = tempFile.readLine();
                for (int i = 0; i < 7; i++)
                {
                    cpuInfo[2-tt][i] = tempStr.section(" ", i+1, i+1).toInt();
                    cpuTotal[1][2-tt] += cpuInfo[2-tt][i];
                    if (i == 3)
                    {
                        cpuTotal[0][2-tt] += cpuInfo[2-tt][i];
                    }
                }
                tt--;
                tempFile.close(); //关闭stat文件
            }

            int a = cpuTotal[0][1] - cpuTotal[0][0];
            int b = cpuTotal[1][1] - cpuTotal[1][0];
            if (a < 0)
            {
                a = -a;
            }
            if (b < 0)
            {
                b = -b;
            }
            m_pProgressBarCPU->setValue(a*100/b);

            tempFile.setFileName("/proc/stat");
            if ( !tempFile.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(this, tr("warning"), tr("The stat file can not open!"), QMessageBox::Yes);
                return;
            }

            tempStr = tempFile.readLine();
            a0 = a1;
            b0 = b1;
            a1 = b1 = 0;
            int gg;
            for (int i = 0; i < 7; i++)
            {
                b1 += tempStr.section(" ", i+2, i+2).toInt();
                gg = b1;
                if (i == 3)
                {
                    a1 += tempStr.section(" ", i+2, i+2).toInt();
                }
            }
            int m, n;
            m = a1 - a0;
            n = b1 - b0;
            if (m < 0)
            {
                m = -m;
            }
            if (n < 0)
            {
                n = -n;
            }
            m_pProgressBarCPU->setValue( (n-m)*100/n );
            tempFile.close(); //关闭stat文件
        }
}
