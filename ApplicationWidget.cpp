#include "ApplicationWidget.h"
// Ui no longer used
// #include "ui_ApplicationWidget.h"
#include "SARibbonMainWindow.h"

ApplicationWidget::ApplicationWidget(SARibbonMainWindow *parent)
    : SARibbonApplicationWidget(parent) //, ui(new Ui::ApplicationWidget)
{
    // ui->setupUi(this);
    setupUi(this);
}

ApplicationWidget::~ApplicationWidget()
{
    // delete ui;
}
