#include "widget.h"
#include "memoryinfopage.h"

#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pVBoxLayoutMain = new QVBoxLayout;
    m_pHBoxLayoutPower = new QHBoxLayout;
    m_pTabWidget = new QTabWidget;
    m_pPushButtonHalt = new QPushButton(tr("Shutdown"));
    m_pPushButtonReboot = new QPushButton(tr("Reboot"));

    m_pVBoxLayoutMain->addWidget(m_pTabWidget);
    m_pHBoxLayoutPower->addStretch();
    m_pHBoxLayoutPower->addWidget(m_pPushButtonHalt);
    m_pHBoxLayoutPower->addWidget(m_pPushButtonReboot);
    m_pVBoxLayoutMain->addLayout(m_pHBoxLayoutPower);

    m_pMemoryInfoPage = new MemoryInfoPage;
    m_pTabWidget->addTab(m_pMemoryInfoPage, tr("Memory"));

    setLayout(m_pVBoxLayoutMain);
}

Widget::~Widget()
{

}
