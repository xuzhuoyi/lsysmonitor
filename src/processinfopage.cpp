#include "processinfopage.h"
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDir>
#include <QMessageBox>

ProcessInfoPage::ProcessInfoPage(QWidget *parent) :
    QWidget(parent)
{
    m_pListWidgetProcess = new QListWidget;
    m_pLabelProcessNumber = new QLabel(tr(" Total:"));
    m_pLabelProcessRunning = new QLabel(tr("Running:"));
    m_pLabelProcessSleeping = new QLabel(tr("Sleeping:"));
    m_pLabelProcessZombie = new QLabel(tr("Zombie:"));
    m_pPushButtonKill = new QPushButton(tr("Kill"));
    m_pPushButtonRefresh = new QPushButton(tr("Refresh"));
    m_pHBoxLayoutButton = new QHBoxLayout;
    m_pVBoxLayoutLabel = new QVBoxLayout;
    m_pGridLayoutMain = new QGridLayout;

    m_pVBoxLayoutLabel->addWidget(m_pLabelProcessNumber);
    m_pVBoxLayoutLabel->addWidget(m_pLabelProcessRunning);
    m_pVBoxLayoutLabel->addWidget(m_pLabelProcessSleeping);
    m_pVBoxLayoutLabel->addWidget(m_pLabelProcessZombie);
    m_pHBoxLayoutButton->addWidget(m_pPushButtonKill);
    m_pHBoxLayoutButton->addWidget(m_pPushButtonRefresh);
    m_pHBoxLayoutButton->addStretch();
    m_pGridLayoutMain->addWidget(m_pListWidgetProcess, 0, 0, 1, 1);
    m_pGridLayoutMain->addLayout(m_pVBoxLayoutLabel, 0, 1, 1, 1);
    m_pGridLayoutMain->addLayout(m_pHBoxLayoutButton, 1, 0, 1, 1);

    setLayout(m_pGridLayoutMain);

    connect(m_pPushButtonKill, SIGNAL(clicked()), this, SLOT(on_pushButtonKill_clicked()));
    connect(m_pPushButtonRefresh, SIGNAL(clicked()), this, SLOT(on_pushButtonRefresh_clicked()));

    __ShowTabInfo();
}

void ProcessInfoPage::__ShowTabInfo()
{
    QString tempStr;
    QFile tempFile;
    m_pListWidgetProcess->clear();
    QDir qd("/proc");
    QStringList qsList = qd.entryList();
    QString qs = qsList.join("\n");
    QString id_of_pro;
    bool ok;
    qint32 find_start = 3;
    qint32 a, b;
    qint32 nProPid; //进程PID
    qint32 number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
    qint32 totalProNum = 0; //进程总数
    QString proName; //进程名
    QString proState; //进程状态
    QString proPri; //进程优先级
    QString proMem; //进程占用内存
    QListWidgetItem *title = new QListWidgetItem(tr("PID")+ "\t" + tr("Name") + "\t\t" +
                                                 tr("State") + "\t" +
                                                 tr("Priority") + "\t" +
                                                 tr("Memory"), m_pListWidgetProcess);
    //循环读取进程
    while (1)
    {
        //获取进程PID
        a = qs.indexOf("\n", find_start);
        b = qs.indexOf("\n", a+1);
        find_start = b;
        id_of_pro = qs.mid(a+1, b-a-1);
        totalProNum++;
        nProPid = id_of_pro.toInt(&ok, 10);
        if(!ok)
        {
            break;
        }

        //打开PID所对应的进程状态文件
        tempFile.setFileName("/proc/" + id_of_pro + "/stat");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = tempFile.readLine();
        if (tempStr.length() == 0)
        {
            break;
        }
        a = tempStr.indexOf("(");
        b = tempStr.indexOf(")");
        proName = tempStr.mid(a+1, b-a-1);
        proName.trimmed(); //删除两端的空格
        proState = tempStr.section(" ", 2, 2);
        proPri = tempStr.section(" ", 17, 17);
        proMem = tempStr.section(" ", 22, 22);

        switch ( proState.at(0).toLatin1() )
        {
            case 'S':   number_of_sleep++; break; //Sleep
            case 'R':   number_of_run++; break; //Running
            case 'Z':   number_of_zombie++; break; //Zombie
            default :   break;
        }

        if (proName.length() >= 12)
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, m_pListWidgetProcess);
        }
        else
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, m_pListWidgetProcess);
        }
    }
    QString temp;
    temp = QString::number(totalProNum, 10);
    m_pLabelProcessNumber->setText(tr("Total:" )+ " " + temp);
    temp = QString::number(number_of_run, 10);
    m_pLabelProcessRunning->setText(tr("Running:" )+ " " + temp);
    temp = QString::number(number_of_sleep, 10);
    m_pLabelProcessSleeping->setText(tr("Sleeping:" )+ " " + temp);
    temp = QString::number(number_of_zombie, 10);
    m_pLabelProcessZombie->setText(tr("Zombie:" )+ " " + temp);

    tempFile.close(); //关闭该PID进程的状态文件
}

void ProcessInfoPage::on_pushButtonKill_clicked()
{
    //获得进程号
    QListWidgetItem *item = m_pListWidgetProcess->currentItem();
    QString pro = item->text();
    pro = pro.section("\t", 0, 0);
    system("kill " + pro.toLatin1());
    QMessageBox::warning(this, tr("kill"), QString::fromUtf8("该进程已被杀死!"), QMessageBox::Yes);
    //回到进程信息tab表
    __ShowTabInfo();
}

void ProcessInfoPage::on_pushButtonRefresh_clicked()
{
    __ShowTabInfo();
}
