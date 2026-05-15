#ifndef APPLICATIONWIDGET_H
#define APPLICATIONWIDGET_H

#include <QWidget>

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "SARibbonApplicationWidget.h"
class SARibbonMainWindow;

// Ui no longer used
/*
namespace Ui
{
class ApplicationWidget;
}
*/

class ApplicationWidget : public SARibbonApplicationWidget
{
    Q_OBJECT

public:
    explicit ApplicationWidget(SARibbonMainWindow *parent = nullptr);
    ~ApplicationWidget();
    /*
    private:
        Ui::ApplicationWidget *ui;
    */

public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonCancel2;
    QListView *listView;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButtonCancel;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ApplicationWidget)
    {
        if (ApplicationWidget->objectName().isEmpty())
            ApplicationWidget->setObjectName("ApplicationWidget");
        ApplicationWidget->resize(567, 400);
        horizontalLayout = new QHBoxLayout(ApplicationWidget);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pushButtonCancel2 = new QPushButton(ApplicationWidget);
        pushButtonCancel2->setObjectName("pushButtonCancel2");

        verticalLayout->addWidget(pushButtonCancel2);

        listView = new QListView(ApplicationWidget);
        listView->setObjectName("listView");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(listView);

        horizontalLayout->addLayout(verticalLayout);

        label = new QLabel(ApplicationWidget);
        label->setObjectName("label");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(label);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        pushButtonCancel = new QPushButton(ApplicationWidget);
        pushButtonCancel->setObjectName("pushButtonCancel");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/icon/delete.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButtonCancel->setIcon(icon);
        pushButtonCancel->setFlat(true);

        verticalLayout_2->addWidget(pushButtonCancel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout->addLayout(verticalLayout_2);

        retranslateUi(ApplicationWidget);
        QObject::connect(pushButtonCancel, &QPushButton::clicked, ApplicationWidget, qOverload<>(&QWidget::hide));
        QObject::connect(pushButtonCancel2, &QPushButton::clicked, ApplicationWidget, qOverload<>(&QWidget::hide));

        QMetaObject::connectSlotsByName(ApplicationWidget);
    } // setupUi

    void retranslateUi(QWidget *ApplicationWidget)
    {
        ApplicationWidget->setWindowTitle(QCoreApplication::translate("ApplicationWidget", "Form", nullptr));
        pushButtonCancel2->setText(QCoreApplication::translate("ApplicationWidget", "Cancel", nullptr));
        label->setText(QCoreApplication::translate("ApplicationWidget", "Press the Esc key to exit the window.", nullptr));
        pushButtonCancel->setText(QString());
    } // retranslateUi
};

#endif // APPLICATIONWIDGET_H
