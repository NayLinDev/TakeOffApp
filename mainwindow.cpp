#include "mainwindow.h"
// Ui no longer used
// #include "ui_mainwindow.h"
#if !SARIBBON_USE_3RDPARTY_FRAMELESSHELPER
#include "SAFramelessHelper.h"
#endif
#include "SARibbonApplicationButton.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include <QCheckBox>
#include "SARibbonColorToolButton.h"
#include <QComboBox>
#include "SARibbonCustomizeDialog.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonGallery.h"

#include "SARibbonMenu.h"
#include "SARibbonPanel.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonToolButton.h"
#include "SARibbonCtrlContainer.h"
#include "colorWidgets/SAColorGridWidget.h"
#include "colorWidgets/SAColorPaletteGridWidget.h"
#include "SARibbonSystemButtonBar.h"
#include "SARibbonApplicationWidget.h"
#include "ApplicationWidget.h"
#include <QAbstractButton>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCalendarWidget>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QFileDialog>
#include <QFontComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTextEdit>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QShortcut>
#include <QLineEdit>
#include <QDialogButtonBox>

/**
 * @brief Macro for starting performance timing measurement
 * @details This macro creates a QElapsedTimer and starts it for measuring code execution time.
 *          It's useful for profiling and debugging performance issues.
 */
#define PRINT_COST_START()    \
    QElapsedTimer __TMP_COST; \
    __TMP_COST.start();       \
    int __TMP_LASTTIMES = 0

/**
 * @brief Macro for printing elapsed time cost
 * @param STR Description string for the timing point
 * @details This macro prints the elapsed time since the last timing point and the total elapsed time.
 *          It outputs to both the debug console and the UI text browser.
 */
#define PRINT_COST(STR)                                                                                               \
    do                                                                                                                \
    {                                                                                                                 \
        int ___TMP_INT = __TMP_COST.elapsed();                                                                        \
        qDebug() << STR << " cost " << ___TMP_INT - __TMP_LASTTIMES << " ms (" << ___TMP_INT << ")";                  \
        textBrowser->append(QString("%1 cost %2 ms(%3)").arg(STR).arg(___TMP_INT - __TMP_LASTTIMES).arg(___TMP_INT)); \
        __TMP_LASTTIMES = ___TMP_INT;                                                                                 \
    } while (0)

/**
 * @brief Constructor, initializes the main window and Ribbon interface
 * @param parent Parent window pointer
 */
MainWindow::MainWindow(QWidget *parent) : SARibbonMainWindow(parent) //, ui(new Ui::MainWindow) // Ui no longer used
{
    // ui->setupUi(this);
    setupUi(this);
    initUI();
}

/**
 * @brief Destructor
 * @details Cleans up the UI resources
 */
MainWindow::~MainWindow()
{
    // delete ui; // Ui no longer used
}

/**
 * @brief Initialize the user interface
 * @details This function sets up the main window's Ribbon interface, including:
 *          - Creating the Ribbon bar with categories and panels
 *          - Setting up the Application button
 *          - Creating context categories
 *          - Setting up the quick access bar and right button group
 *          - Connecting signals and slots for various UI controls
 */
void MainWindow::initUI()
{
    PRINT_COST_START();
    setWindowTitle(("ribbon mainwindow test[*]"));
    setWindowModified(true);

    SARibbonBar *ribbonBar = this->ribbonBar();
    setContentsMargins(1, 0, 1, 0);
    ribbonBar->setContentsMargins(4, 0, 4, 0);

    connect(ribbonBar, &SARibbonBar::actionTriggered, this, [this](QAction *triggeredAction)
            { textBrowser->append(QString("action object name=%1 triggered").arg(triggeredAction->objectName())); });

    createRibbonApplicationButton();

    SARibbonCategory *categoryMain = ribbonBar->addCategoryPage(tr("&Main"));
    categoryMain->setObjectName(("categoryMain"));
    createCategoryMain(categoryMain);
    PRINT_COST("new main page");

    SARibbonCategory *categoryOther = new SARibbonCategory();
    categoryOther->setCategoryName(tr("Other"));
    categoryOther->setObjectName(("categoryOther"));
    createCategoryOther(categoryOther);
    ribbonBar->addCategoryPage(categoryOther);
    PRINT_COST("add other page");

    SARibbonCategory *categoryDelete = new SARibbonCategory();
    categoryDelete->setCategoryName(("Delete"));
    categoryDelete->setObjectName(("categoryDelete"));
    ribbonBar->addCategoryPage(categoryDelete);
    createCategoryDelete(categoryDelete);
    PRINT_COST("add category delete page");

    SARibbonCategory *categorySize = new SARibbonCategory();
    categorySize->setCategoryName(("Size(example long category)"));
    categorySize->setObjectName(("categorySize"));
    ribbonBar->addCategoryPage(categorySize);
    createCategorySize(categorySize);
    PRINT_COST("add category size page");

    SARibbonCategory *categoryColor = new SARibbonCategory();
    categoryColor->setCategoryName(("Color"));
    categoryColor->setObjectName(("categoryColor"));
    ribbonBar->addCategoryPage(categoryColor);
    createCategoryColor(categoryColor);
    PRINT_COST("add category color page");

    createContextCategory1();
    PRINT_COST("add context1 category page");

    createContextCategory2();
    PRINT_COST("add context2 category page");

    createQuickAccessBar();
    PRINT_COST("add quick access bar");

    createRightButtonGroup();
    PRINT_COST("add right bar");

    createWindowButtonGroupBar();

    createActionsManager();

    setMinimumWidth(500);
    setWindowIcon(QIcon(":/icon/icon/SA.svg"));

#if QT_VERSION_MAJOR >= 6
    connect(spinBoxRibbonTitleHeight, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxRibbonTitleHeightChanged);
    connect(spinBoxRibbonTabHeight, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxRibbonTabHeightChanged);
    connect(spinBoxRibbonCategoryHeight, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxRibbonCategoryHeightChanged);
    connect(spinBoxPanelTtitleHeight, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxRibbonPanelTtitleHeightChanged);
    connect(spinBoxPanelSpacing, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxRibbonPanelSpacingChanged);
    connect(spinBoxPanelSmallIconSize, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxPanelSmallIconSizeChanged);
    connect(spinBoxPanelLargeIconSize, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxPanelLargeIconSizeChanged);
    connect(doubleSpinBoxToolbuttonMaximumAspectRatio,
            &QDoubleSpinBox::valueChanged,
            this,
            &MainWindow::onDoubleSpinBoxToolbuttonMaximumAspectRatioChanged);
#else
    connect(spinBoxRibbonTitleHeight,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxRibbonTitleHeightChanged);
    connect(spinBoxRibbonTabHeight,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxRibbonTabHeightChanged);
    connect(spinBoxRibbonCategoryHeight,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxRibbonCategoryHeightChanged);
    connect(spinBoxPanelTtitleHeight,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxRibbonPanelTtitleHeightChanged);
    connect(spinBoxPanelSpacing,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxRibbonPanelSpacingChanged);
    connect(spinBoxPanelSmallIconSize,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxPanelSmallIconSizeChanged);
    connect(spinBoxPanelLargeIconSize,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &MainWindow::onSpinBoxPanelLargeIconSizeChanged);
    connect(doubleSpinBoxToolbuttonMaximumAspectRatio,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &MainWindow::onDoubleSpinBoxToolbuttonMaximumAspectRatioChanged);
#endif
    connect(ribbonBar, &SARibbonBar::currentRibbonTabChanged, this, [this](int tabIndex)
            { textBrowser->append(QString("SARibbonBar::currentRibbonTabChanged(%1)").arg(tabIndex)); });

    spinBoxRibbonTitleHeight->setValue(this->ribbonBar()->titleBarHeight());
    spinBoxRibbonTabHeight->setValue(this->ribbonBar()->tabBarHeight());
    spinBoxRibbonCategoryHeight->setValue(this->ribbonBar()->categoryHeight());
    spinBoxPanelTtitleHeight->setValue(this->ribbonBar()->panelTitleHeight());
    spinBoxPanelSpacing->setValue(this->ribbonBar()->panelSpacing());
    spinBoxPanelSmallIconSize->setValue(this->ribbonBar()->panelSmallIconSize().width());
    spinBoxPanelLargeIconSize->setValue(this->ribbonBar()->panelLargeIconSize().width());
    doubleSpinBoxToolbuttonMaximumAspectRatio->setValue(this->ribbonBar()->buttonMaximumAspectRatio());

    mChangeTitleBkColorTimer.setInterval(5000);
    mChangeTitleBkColorTimer.start();
    connect(&mChangeTitleBkColorTimer, &QTimer::timeout, this, &MainWindow::onChangedTitleTimeout);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QTimer::singleShot(0, this, &QWidget::showMaximized);
#else
    showMaximized();
#endif
}

/**
 * @brief Create the Application Button for the Ribbon interface
 * @details The Application Button is located in the upper left corner of the Ribbon interface.
 *          It typically displays file-related operations such as New, Open, Save, etc.
 *          By default, SARibbonBar creates a SARibbonApplicationButton. Users can create
 *          their own application button and set it via SARibbonBar::setApplicationButton().
 *          Passing a nullptr will remove the application button.
 */
void MainWindow::createRibbonApplicationButton()
{
    SARibbonBar *currentRibbonBar = ribbonBar();
    if (!currentRibbonBar)
    {
        return;
    }
    QAbstractButton *applicationButton = currentRibbonBar->applicationButton();
    if (!applicationButton)
    {
        applicationButton = new SARibbonApplicationButton(this);
        currentRibbonBar->setApplicationButton(applicationButton);
    }
    applicationButton->setText(("&File"));
#ifdef USE_APPLICATION_NORMAL_MENU
    if (!mMenuApplicationBtn)
    {
        mMenuApplicationBtn = new SARibbonMenu(this);
        mMenuApplicationBtn->addAction(createAction("test1", ":/icon/icon/action.svg"));
        mMenuApplicationBtn->addAction(createAction("test2", ":/icon/icon/action2.svg"));
        mMenuApplicationBtn->addAction(createAction("test3", ":/icon/icon/action3.svg"));
        mMenuApplicationBtn->addAction(createAction("test4", ":/icon/icon/action4.svg"));
    }
    SARibbonApplicationButton *ribbonAppBtn = qobject_cast<SARibbonApplicationButton *>(applicationButton);
    if (!ribbonAppBtn)
    {
        return;
    }
    ribbonAppBtn->setMenu(mMenuApplicationBtn);
#else
    mAppWidget = new ApplicationWidget(this);
    mAppWidget->hide();
    connect(applicationButton, &QAbstractButton::clicked, this, [this](bool checked)
            {
        Q_UNUSED(checked);
        this->mAppWidget->show(); });
#endif
}

/**
 * @brief Show or hide the Context Category
 * @param visible Whether to show the context category
 * @details Context Category is a special tab that is usually displayed for specific context environments,
 *          such as showing related operations when an object is selected.
 */
void MainWindow::onShowContextCategory(bool visible)
{
    if (mContextCategory == nullptr)
    {
        createContextCategory1();
    }
    if (visible)
    {
        this->ribbonBar()->showContextCategory(mContextCategory);
    }
    else
    {
        this->ribbonBar()->hideContextCategory(mContextCategory);
    }
}

/**
 * @brief Handle Ribbon style button click
 * @param styleId The style ID corresponding to the clicked button
 * @details SARibbon supports multiple styles, including combinations of loose/compact and two-row/three-row modes.
 *          Users can switch between styles as needed.
 */
void MainWindow::onStyleClicked(int styleId)
{
    SARibbonBar::RibbonStyles ribbonStyle = static_cast<SARibbonBar::RibbonStyles>(styleId);
    ribbonBar()->setRibbonStyle(ribbonStyle);
    mActionWordWrap->setChecked(ribbonBar()->isEnableWordWrap());
    switch (ribbonStyle)
    {
    case SARibbonBar::RibbonStyleLooseThreeRow:
        textBrowser->append(
            tr("\nThe text display of the \"LooseThreeRow\" style is word wrap and also displays the title bar. "
               "You can also control whether the button is line breaking through SARibbonBar::setEnableWordWrap,"
               "and whether the title bar is displayed through SARibbonBar::setEnableShowPanelTitle"));
        textBrowser->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);"));
        break;
    case SARibbonBar::RibbonStyleLooseTwoRow:
        textBrowser->append(
            tr("\nThe text display of the \"LooseTwoRow\" style is not word wrap and also displays the title bar. "
               "You can also control whether the button is line breaking through SARibbonBar::setEnableWordWrap,"
               "and whether the title bar is displayed through SARibbonBar::setEnableShowPanelTitle"));
        textBrowser->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleLooseTwoRow);"));
        break;
    case SARibbonBar::RibbonStyleCompactThreeRow:
        textBrowser->append(
            tr("\nThe text display of the \"LooseThreeRow\" style is word wrap and not displays the title bar. "
               "You can also control whether the button is line breaking through SARibbonBar::setEnableWordWrap,"
               "and whether the title bar is displayed through SARibbonBar::setEnableShowPanelTitle"));
        textBrowser->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);"));
        break;
    case SARibbonBar::RibbonStyleCompactTwoRow:
        textBrowser->append(
            tr("\nThe text display of the \"CompactTwoRow\" style is not word wrap and not displays the title bar. "
               "You can also control whether the button is line breaking through SARibbonBar::setEnableWordWrap,"
               "and whether the title bar is displayed through SARibbonBar::setEnableShowPanelTitle"));
        textBrowser->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleCompactTwoRow);"));
        break;
    default:
        break;
    }
}

/**
 * @brief Handle customize action trigger
 * @details This demonstrates how to use SARibbonCustomizeWidget to implement interface customization.
 *          The customize widget allows users to modify the ribbon layout.
 */
void MainWindow::onActionCustomizeTriggered()
{
    if (nullptr == mWidgetForCustomize)
    {
        mWidgetForCustomize =
            new SARibbonCustomizeWidget(this, this, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        mWidgetForCustomize->setWindowModality(Qt::ApplicationModal);
        mWidgetForCustomize->setAttribute(Qt::WA_ShowModal, true);
        mWidgetForCustomize->setupActionsManager(mActionsManager);
    }
    mWidgetForCustomize->show();
    mWidgetForCustomize->applys();
}

/**
 * @brief Handle customize and save action trigger
 * @details This demonstrates how to use SARibbonCustomizeDialog to implement interface customization,
 *          and also demonstrates how to save the customized interface as an XML file.
 */
void MainWindow::onActionCustomizeAndSaveTriggered()
{
    SARibbonCustomizeDialog customizeDialog(this);
    customizeDialog.setupActionsManager(mActionsManager);
    if (!mHasApplyCustomizeXmlFile)
    {
        auto userResponse = QMessageBox::question(
            this, tr("question"), tr("Apply the last modification?\nIf not, local data will be reset"));
        if (userResponse == QMessageBox::Yes)
        {
            onActionLoadCustomizeXmlFileTriggered();
            return;
        }
        else
        {
            QFile::remove("customize.xml");
            customizeDialog.clear();
            mHasApplyCustomizeXmlFile = true;
        }
    }

    customizeDialog.fromXml("customize.xml");
    if (SARibbonCustomizeDialog::Accepted == customizeDialog.exec())
    {
        if (customizeDialog.isCached())
            customizeDialog.applys();

        if (!customizeDialog.isApplied())
        {
            textBrowser->append("no change to save");
            return;
        }

        QByteArray xmlContent;
        QXmlStreamWriter xmlWriter(&xmlContent);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.setAutoFormattingIndent(2);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        xmlWriter.setCodec("utf-8");
#endif
        xmlWriter.writeStartDocument();
        bool writeSuccess = customizeDialog.toXml(&xmlWriter);
        xmlWriter.writeEndDocument();
        if (writeSuccess)
        {
            QFile xmlFile("customize.xml");
            if (xmlFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
            {
                QTextStream textStream(&xmlFile);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                textStream.setCodec("utf-8");
#endif
                textStream << xmlContent;
                textStream.flush();
            }
            textBrowser->append("write xml:");
            textBrowser->append(xmlContent);
        }
    }
}

/**
 * @brief Handle customize and save with apply action trigger
 * @details This demonstrates how to embed SARibbonCustomizeWidget into a QDialog to implement interface customization.
 *          This approach allows better integration with your own software settings page.
 */
void MainWindow::onActionCustomizeAndSaveWithApplyTriggered()
{
    if (!mHasApplyCustomizeXmlFile)
    {
        auto userResponse = QMessageBox::question(
            this, tr("question"), tr("Apply the last modification?\nIf not, local data will be reset"));
        if (userResponse == QMessageBox::Yes)
        {
            onActionLoadCustomizeXmlFileTriggered();
            return;
        }
        else
        {
            QFile::remove("customize.xml");
            mHasApplyCustomizeXmlFile = true;
        }
    }

    QDialog customizeDialog;
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    customizeDialog.setLayout(dialogLayout);
    SARibbonCustomizeWidget *customizeWidget = new SARibbonCustomizeWidget(this, &customizeDialog);
    customizeWidget->setupActionsManager(mActionsManager);

    dialogLayout->addWidget(customizeWidget, 1);

    QDialogButtonBox *buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

    dialogLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &customizeDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &customizeDialog, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::clicked, &customizeDialog, [=](QAbstractButton *clickedButton)
            {
        auto buttonRole = buttonBox->buttonRole(clickedButton);
        if (buttonRole == QDialogButtonBox::ApplyRole) {
            if (customizeWidget->isCached()) {
                customizeWidget->applys();
                textBrowser->append("change applied");
            } else {
                textBrowser->append("no change to apply");
            }
        } });

    customizeWidget->fromXml("customize.xml");
    if (QDialog::Accepted == customizeDialog.exec())
    {
        if (customizeWidget->isCached())
            customizeWidget->applys();

        if (!customizeWidget->isApplied())
        {
            textBrowser->append("no change to save");
            return;
        }

        QByteArray xmlContent;
        QXmlStreamWriter xmlWriter(&xmlContent);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.setAutoFormattingIndent(2);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        xmlWriter.setCodec("utf-8");
#endif
        xmlWriter.writeStartDocument();
        bool writeSuccess = customizeWidget->toXml(&xmlWriter);
        xmlWriter.writeEndDocument();
        if (writeSuccess)
        {
            QFile xmlFile("customize.xml");
            if (xmlFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
            {
                QTextStream textStream(&xmlFile);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                textStream.setCodec("utf-8");
#endif
                textStream << xmlContent;
                textStream.flush();
            }
            textBrowser->append("write xml:");
            textBrowser->append(xmlContent);
        }
    }
    else
    {
        customizeWidget->clear();
        textBrowser->append("all changes clear, the applied changes will take no effect afer restart");
    }
}

/**
 * @brief Handle help action trigger
 * @details Display SARibbon library version information and author contact information.
 */
void MainWindow::onActionHelpTriggered()
{
    QMessageBox::information(this,
                             tr("infomation"),
                             tr("\n ==============="
                                "\n SARibbonBar version:%1"
                                "\n Author:czy"
                                "\n Email:czy.t@163.com"
                                "\n ===============")
                                 .arg(SARibbonBar::versionString()));
}

/**
 * @brief Handle remove application button action trigger
 * @param removeButton Whether to remove the application button
 * @details Demonstrate how to dynamically add or remove the Application Button.
 */
void MainWindow::onActionRemoveAppBtnTriggered(bool removeButton)
{
    if (removeButton)
    {
        ribbonBar()->setApplicationButton(nullptr);
    }
    else
    {
        SARibbonApplicationButton *newAppButton = new SARibbonApplicationButton();
        newAppButton->setText(tr("&File"));
        this->ribbonBar()->setApplicationButton(newAppButton);
        createRibbonApplicationButton();
    }
}

/**
 * @brief Handle use QSS action trigger
 * @details Demonstrate how to customize Ribbon interface styles through QSS files.
 */
void MainWindow::onActionUseQssTriggered()
{
    QFile qssFile("ribbon.qss");
    if (!qssFile.exists())
    {
        QString selectedPath = QFileDialog::getOpenFileName(this, tr("select qss file"));
        if (selectedPath.isEmpty())
        {
            return;
        }
        qssFile.setFileName(selectedPath);
    }
    if (!qssFile.open(QIODevice::ReadWrite))
    {
        return;
    }
    QString qssContent(qssFile.readAll());
    textBrowser->setText(qssContent);
    this->ribbonBar()->setStyleSheet(qssContent);
}

/**
 * @brief Handle load customize XML file action trigger
 * @details Load previously saved custom configuration from XML file.
 */
void MainWindow::onActionLoadCustomizeXmlFileTriggered()
{
    if (!mHasApplyCustomizeXmlFile)
    {
        if (!QFile::exists("customize.xml"))
        {
            mHasApplyCustomizeXmlFile = true;
            return;
        }
        QFile xmlFile("customize.xml");
        qDebug() << "size of customize.xml : " << xmlFile.size();
        if (xmlFile.size() <= 0)
        {
            mHasApplyCustomizeXmlFile = true;
            return;
        }
        mHasApplyCustomizeXmlFile = sa_apply_customize_from_xml_file("customize.xml", ribbonBar(), mActionsManager);
    }
}

/**
 * @brief Handle window flag normal button action trigger
 * @param showButtons Whether to show window buttons
 * @details Control the display of window minimize, maximize, and close buttons.
 */
void MainWindow::onActionWindowFlagNormalButtonTriggered(bool showButtons)
{
    Qt::WindowFlags currentFlags = windowFlags();
    currentFlags.setFlag(Qt::WindowCloseButtonHint, showButtons);
    currentFlags.setFlag(Qt::WindowMinMaxButtonsHint, showButtons);
    setWindowFlags(currentFlags);
}

/**
 * @brief Handle font combobox current font changed
 * @param newFont The newly selected font
 * @details When the user selects a new font in the font combobox, update the font of the Ribbon interface.
 */
void MainWindow::onFontComWidgetCurrentFontChanged(const QFont &newFont)
{
    qDebug() << "set font:" << newFont;
    ribbonBar()->setFont(newFont);
    update();
}

/**
 * @brief Handle font larger action trigger
 * @details Increase the font size of the Ribbon interface.
 */
void MainWindow::onActionFontLargerTriggered()
{
    QFont currentFont = font();
    currentFont.setPointSize(currentFont.pointSize() + 1);
    qDebug() << "before set ribbonBar font:" << ribbonBar()->font();
    ribbonBar()->setFont(currentFont);
    update();
    qDebug() << "set font:" << currentFont;
}

/**
 * @brief Handle font smaller action trigger
 * @details Decrease the font size of the Ribbon interface.
 */
void MainWindow::onActionFontSmallerTriggered()
{
    QFont currentFont = font();
    currentFont.setPointSize(currentFont.pointSize() - 1);
    qDebug() << "before set ribbonBar font:" << ribbonBar()->font();
    ribbonBar()->setFont(currentFont);
    update();
    qDebug() << "set font:" << currentFont;
}

/**
 * @brief Handle word wrap action trigger
 * @param enableWordWrap Whether to enable word wrap
 * @details Control whether text on Ribbon buttons is displayed with line breaks.
 */
void MainWindow::onActionWordWrapTriggered(bool enableWordWrap)
{
    ribbonBar()->setEnableWordWrap(enableWordWrap);
    textBrowser->append(tr("By using the SARibbonBar::setEnableWordWrap function, "
                           "you can set whether text breaks or not.\n"
                           "By default, the two line mode will not wrap, the three line mode will wrap.\n"
                           "You can force the two line mode to wrap, or the three line mode to not wrap"));
}

/**
 * @brief Handle button group action trigger
 * @param triggeredAction The triggered action
 * @details Handle button click events in the button group, used to set title alignment.
 */
void MainWindow::onButtonGroupActionTriggered(QAction *triggeredAction)
{
    QVariant alignmentVariant = triggeredAction->property("align");
    if (alignmentVariant.isValid())
    {
        Qt::Alignment alignment = static_cast<Qt::Alignment>(alignmentVariant.toInt());
        if (!ribbonBar())
        {
            return;
        }
        ribbonBar()->setWindowTitleAligment(alignment);
    }
}

/**
 * @brief Handle color button color clicked
 * @param selectedColor The selected color
 * @param clicked Whether the button was clicked
 * @details Handle color selection events for color buttons.
 */
void MainWindow::onColorButtonColorClicked(const QColor &selectedColor, bool clicked)
{
    Q_UNUSED(clicked);
    textBrowser->append(QString("color click %1").arg(selectedColor.name()));
}

/**
 * @brief Handle Ribbon theme combobox current index changed
 * @param themeIndex The index of the selected theme
 * @details Switch Ribbon interface theme.
 */
void MainWindow::onRibbonThemeComboBoxCurrentIndexChanged(int themeIndex)
{
    SARibbonTheme selectedTheme = static_cast<SARibbonTheme>(mComboboxRibbonTheme->itemData(themeIndex).toInt());
    setRibbonTheme(selectedTheme);
}

/**
 * @brief Handle hide action trigger
 * @param hideActions Whether to hide the actions
 * @details Batch control the visibility of a group of actions.
 */
void MainWindow::onActionHideActionTriggered(bool hideActions)
{
    mActionWordWrap->setVisible(hideActions);
    mActionDisable->setVisible(hideActions);
    mActionUnlock->setVisible(hideActions);
    mActionSetTextTest->setVisible(hideActions);
    mActionShowTest->setVisible(hideActions);
    mActionHideAction2->setVisible(hideActions);
    mActionHideAction4->setVisible(hideActions);
    mActionHideShowTextAct2->setVisible(hideActions);
    mActionHideShowTextAct3->setVisible(hideActions);
    mActionHideShowTextAct4->setVisible(hideActions);
    ribbonBar()->updateRibbonGeometry();
}

/**
 * @brief Handle visible all action trigger
 * @param visibleAll Whether to make all actions visible
 * @details Control the visibility of all actions registered to the action manager.
 */
void MainWindow::onActionVisibleAllTriggered(bool visibleAll)
{
    const QList<QAction *> allActions = mActionsManager->allActions();
    for (QAction *currentAction : allActions)
    {
        if (currentAction != mActionVisibleAll)
        {
            currentAction->setVisible(visibleAll);
        }
    }
    ribbonBar()->updateRibbonGeometry();
}

/**
 * @brief Handle alignment center checkbox clicked
 * @param centerAligned Whether to center align
 * @details Control whether Ribbon toolbar content is center aligned.
 */
void MainWindow::onCheckBoxAlignmentCenterClicked(bool centerAligned)
{
    if (centerAligned)
    {
        ribbonBar()->setRibbonAlignment(SARibbonAlignment::AlignCenter);
    }
    else
    {
        ribbonBar()->setRibbonAlignment(SARibbonAlignment::AlignLeft);
    }
}

/**
 * @brief Handle Ribbon title height spinbox value changed
 * @param newHeight The new title bar height
 * @details Adjust Ribbon title bar height.
 */
void MainWindow::onSpinBoxRibbonTitleHeightChanged(int newHeight)
{
    ribbonBar()->setTitleBarHeight(newHeight);
}

/**
 * @brief Handle Ribbon tab height spinbox value changed
 * @param newHeight The new tab bar height
 * @details Adjust Ribbon tab bar height.
 */
void MainWindow::onSpinBoxRibbonTabHeightChanged(int newHeight)
{
    ribbonBar()->setTabBarHeight(newHeight);
}

/**
 * @brief Handle Ribbon category height spinbox value changed
 * @param newHeight The new category area height
 * @details Adjust Ribbon category area height.
 */
void MainWindow::onSpinBoxRibbonCategoryHeightChanged(int newHeight)
{
    ribbonBar()->setCategoryHeight(newHeight);
}

/**
 * @brief Handle Ribbon panel title height spinbox value changed
 * @param newHeight The new panel title height
 * @details Adjust Ribbon panel title height.
 */
void MainWindow::onSpinBoxRibbonPanelTtitleHeightChanged(int newHeight)
{
    ribbonBar()->setPanelTitleHeight(newHeight);
}

/**
 * @brief Handle Ribbon panel spacing spinbox value changed
 * @param newSpacing The new spacing between panels
 * @details Adjust the spacing between Ribbon panels.
 */
void MainWindow::onSpinBoxRibbonPanelSpacingChanged(int newSpacing)
{
    ribbonBar()->setPanelSpacing(newSpacing);
}

/**
 * @brief Handle panel small icon size spinbox value changed
 * @param newSize The new small icon size
 * @details Adjust the icon size of small tool buttons in Ribbon panels.
 */
void MainWindow::onSpinBoxPanelSmallIconSizeChanged(int newSize)
{
    ribbonBar()->setPanelSmallIconSize(QSize(newSize, newSize));
    ribbonBar()->update();
}

/**
 * @brief Handle panel large icon size spinbox value changed
 * @param newSize The new large icon size
 * @details Adjust the icon size of large tool buttons in Ribbon panels.
 */
void MainWindow::onSpinBoxPanelLargeIconSizeChanged(int newSize)
{
    ribbonBar()->setPanelLargeIconSize(QSize(newSize, newSize));
    ribbonBar()->update();
}

/**
 * @brief Handle tool button maximum aspect ratio spinbox value changed
 * @param newRatio The new maximum aspect ratio
 * @details Adjust the maximum aspect ratio of tool buttons.
 */
void MainWindow::onDoubleSpinBoxToolbuttonMaximumAspectRatioChanged(double newRatio)
{
    ribbonBar()->setButtonMaximumAspectRatio(newRatio);
}

/**
 * @brief Handle title background brush changed timeout
 * @details Periodically change the title bar background color to demonstrate dynamic effects.
 */
void MainWindow::onTitleBackgroundBrushChangedTimeout()
{
    static bool s_useNoBrush = true;
    s_useNoBrush = (!s_useNoBrush);
    if (s_useNoBrush)
    {
        ribbonBar()->setWindowTitleTextColor(Qt::black);
        ribbonBar()->setWindowTitleBackgroundBrush(Qt::NoBrush);
    }
    else
    {
        ribbonBar()->setWindowTitleTextColor(Qt::white);
        ribbonBar()->setWindowTitleBackgroundBrush(Qt::red);
    }
    ribbonBar()->repaint();
}

/**
 * @brief Handle changed title timeout
 * @details Timer slot function used to periodically change the title bar color.
 */
void MainWindow::onChangedTitleTimeout()
{
    static int s_toggleCounter = 0;
    auto currentRibbonBar = ribbonBar();
    if (!currentRibbonBar)
    {
        return;
    }
    ++s_toggleCounter;
    if (s_toggleCounter % 2 == 0)
    {
        currentRibbonBar->setWindowTitleBackgroundBrush(QColor(222, 79, 79));
        currentRibbonBar->setWindowTitleTextColor(Qt::white);
        currentRibbonBar->update();
    }
    else
    {
        currentRibbonBar->setWindowTitleBackgroundBrush(Qt::NoBrush);
        currentRibbonBar->setWindowTitleTextColor(Qt::blue);
        currentRibbonBar->update();
    }
}

/**
 * @brief Create other actions
 * @details Create some actions that are not directly displayed in the Ribbon interface,
 *          but can be used through customization features.
 */
void MainWindow::createOtherActions()
{
    mOtherAction1 = new QAction(("text action1"), this);
    mOtherAction2 = new QAction(("text action2"), this);
    mOtherAction3 = new QAction(("text action3"), this);
    mOtherAction4 = new QAction(("text action4"), this);
    mOtherAction5 = new QAction(("text action5"), this);
    mOtherActionIcon1 = new QAction(QIcon(":/icon/icon/layout.svg"), ("action with icon"), this);
}

/**
 * @brief Handle close event
 * @param closeEvent The close event
 * @details Ask user for confirmation before closing the window.
 */
void MainWindow::closeEvent(QCloseEvent *closeEvent)
{
    auto userResponse = QMessageBox::question(this, tr("question"), tr("Confirm whether to exit"));
    if (userResponse == QMessageBox::Yes)
    {
        closeEvent->accept();
    }
    else
    {
        closeEvent->ignore();
    }
}

/**
 * @brief Handle save action trigger
 * @details Save action demonstration - shows how to handle save operations in Ribbon.
 */
void MainWindow::onSaveActionTriggered()
{
    textBrowser->append("save action clicked");
    setWindowModified(false);
    ribbonBar()->repaint();
}

/**
 * @brief Handle hide ribbon action trigger
 * @param hide Whether to hide the ribbon
 * @details Toggle between minimum and normal ribbon mode.
 */
void MainWindow::onHideRibbonActionTriggered(bool hide)
{
    ribbonBar()->setMinimumMode(hide);
}

/**
 * @brief Handle show/hide button action trigger
 * @param show Whether to show the button
 * @details Show or hide the built-in minimum mode button.
 */
void MainWindow::onShowHideButtonActionTriggered(bool show)
{
    ribbonBar()->showMinimumModeButton(show);
}

/**
 * @brief Handle show title icon action trigger
 * @param show Whether to show the title icon
 * @details Show or hide the title icon in the ribbon bar.
 */
void MainWindow::onShowTitleIconActionTriggered(bool show)
{
    ribbonBar()->setTitleIconVisible(show);
}

/**
 * @brief Handle application button expand action trigger
 * @param expand Whether to expand the application button
 * @details Set whether the Application Button occupies the title bar and tab bar.
 */
void MainWindow::onAppButtonExpandActionTriggered(bool expand)
{
    ribbonBar()->setApplicationButtonVerticalExpansion(expand);
}

/**
 * @brief Handle show context action trigger
 * @param show Whether to show the context category
 * @details Show or hide the context category.
 */
void MainWindow::onShowContextActionTriggered(bool show)
{
    onShowContextCategory(show);
}

/**
 * @brief Handle delete context action trigger
 * @details Delete the context category.
 */
void MainWindow::onDeleteContextActionTriggered()
{
    if (mContextCategory)
    {
        ribbonBar()->destroyContextCategory(mContextCategory);
        mContextCategory = nullptr;
        if (mActionDeleteContext)
        {
            mActionDeleteContext->setDisabled(true);
        }
    }
}

/**
 * @brief Handle word wrap demo action trigger
 * @details Demonstrate text wrapping functionality.
 */
void MainWindow::onWordWrapDemoActionTriggered()
{
    textBrowser->append(tr("Text can be manually wrapped(use \\n), and will appear as 1 line in the case of "
                           "SARibbonBar::setEnableWordWrap (false)"));
}

/**
 * @brief Handle word wrap with menu action trigger
 * @details Demonstrate text wrapping with menu functionality.
 */
void MainWindow::onWordWrapWithMenuActionTriggered()
{
    textBrowser->append(tr("Text can be manually wrapped(use \\n), and will appear as 1 line in the case of "
                           "SARibbonBar::setEnableWordWrap (false)"));
}

/**
 * @brief Handle remove panel action trigger
 * @param category The category containing the panel
 * @param panel The panel to remove
 * @details Remove a panel from a category.
 */
void MainWindow::onRemovePanelActionTriggered(SARibbonCategory *category, SARibbonPanel *panel)
{
    if (category && panel)
    {
        category->removePanel(panel);
    }
}

/**
 * @brief Handle remove category action trigger
 * @param category The category to remove
 * @details Remove a category from the ribbon bar.
 */
void MainWindow::onRemoveCategoryActionTriggered(SARibbonCategory *category)
{
    if (category)
    {
        ribbonBar()->removeCategory(category);
        category->hide();
        category->deleteLater();
    }
}

/**
 * @brief Handle insert panel at start action trigger
 * @param category The category to insert the panel into
 * @details Insert a new panel at the start position (index 0).
 */
void MainWindow::onInsertPanelAtStartActionTriggered(SARibbonCategory *category)
{
    if (!category)
        return;
    SARibbonPanel *newPanel = new SARibbonPanel(("Inserted at 0"));
    static int insertCount = 0;
    ++insertCount;
    QAction *newAction = new QAction("Inserted at 0\n Action " + QString::number(insertCount), newPanel);
    newPanel->addLargeAction(newAction);
    SARibbonToolButton *toolButton = newPanel->actionToRibbonToolButton(newAction);
    toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    category->insertPanel(newPanel, 0);
}

/**
 * @brief Handle insert panel at end action trigger
 * @param category The category to insert the panel into
 * @details Insert a new panel at the end position.
 */
void MainWindow::onInsertPanelAtEndActionTriggered(SARibbonCategory *category)
{
    if (!category)
        return;
    SARibbonPanel *newPanel = new SARibbonPanel(("Inserted at end"));
    static int insertCount = 0;
    ++insertCount;
    QAction *newAction = new QAction("Inserted at end\n Action " + QString::number(insertCount), newPanel);
    newPanel->addLargeAction(newAction);
    SARibbonToolButton *toolButton = newPanel->actionToRibbonToolButton(newAction);
    toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    category->insertPanel(newPanel, category->panelCount());
}

/**
 * @brief Handle insert panel at negative one action trigger
 * @param category The category to insert the panel into
 * @details Insert a new panel at position -1 (which appends to the end).
 */
void MainWindow::onInsertPanelAtNegativeOneActionTriggered(SARibbonCategory *category)
{
    if (!category)
        return;
    SARibbonPanel *newPanel = new SARibbonPanel(("Inserted at -1"));
    static int insertCount = 0;
    ++insertCount;
    QAction *newAction = new QAction("Inserted at -1\n Action " + QString::number(insertCount), newPanel);
    newPanel->addLargeAction(newAction);
    SARibbonToolButton *toolButton = newPanel->actionToRibbonToolButton(newAction);
    toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    category->insertPanel(newPanel, -1);
}

/**
 * @brief Handle unlock action trigger
 * @details Unlock the disabled action.
 */
void MainWindow::onUnlockActionTriggered()
{
    mActionDisable->setEnabled(true);
    mActionDisable->setText(("Enabled"));
}

/**
 * @brief Handle set text action toggled
 * @param checked Whether the action is checked
 * @details Toggle the text of the action between Chinese and English.
 */
void MainWindow::onSetTextActionToggled(bool checked)
{
    if (checked)
    {
        mActionSetTextTest->setText(QStringLiteral(u"setText测试"));
    }
    else
    {
        mActionSetTextTest->setText(QStringLiteral(u"set text"));
    }
}

/**
 * @brief Handle show panel action toggled
 * @param checked Whether the action is checked
 * @details Toggle the visibility of the example panel.
 */
void MainWindow::onShowPanelActionToggled(bool checked)
{
    mPanelVisbileExample->setVisible(!checked);
    if (checked)
    {
        mActionShowTest->setText(tr("hide beside panel"));
    }
    else
    {
        mActionShowTest->setText(tr("show beside panel"));
    }
    ribbonBar()->repaint();
}

/**
 * @brief Handle hide action 2 trigger
 * @param checked Whether the action is checked
 * @details Toggle the visibility of action 2.
 */
void MainWindow::onHideAction2Triggered(bool checked)
{
    if (checked)
    {
        mActionHideShowTextAct2->setVisible(true);
        mActionHideAction2->setText(tr("hide action2"));
    }
    else
    {
        mActionHideShowTextAct2->setVisible(false);
        mActionHideAction2->setText(tr("show action2"));
    }
}

/**
 * @brief Handle hide action 4 trigger
 * @param checked Whether the action is checked
 * @details Toggle the visibility of action 4.
 */
void MainWindow::onHideAction4Triggered(bool checked)
{
    if (checked)
    {
        mActionHideShowTextAct4->setVisible(true);
        mActionHideAction4->setText(tr("hide action4"));
    }
    else
    {
        mActionHideShowTextAct4->setVisible(false);
        mActionHideAction4->setText(tr("show action4"));
    }
}

/**
 * @brief Handle delete this category action trigger
 * @param category The category to delete
 * @details Delete the specified category from the ribbon bar.
 */
void MainWindow::onDeleteThisCategoryActionTriggered(SARibbonCategory *category)
{
    if (category)
    {
        ribbonBar()->removeCategory(category);
        category->deleteLater();
    }
}

/**
 * @brief Handle undo action trigger
 * @details Undo operation demonstration.
 */
void MainWindow::onUndoActionTriggered()
{
    textBrowser->append("undo");
}

/**
 * @brief Handle redo action trigger
 * @details Redo operation demonstration.
 */
void MainWindow::onRedoActionTriggered()
{
    textBrowser->append("redo");
}

/**
 * @brief Handle search editor editing finished
 * @details Process the search text when editing is finished.
 */
void MainWindow::onSearchEditorEditingFinished()
{
    textBrowser->append(mSearchEditor->text());
}

/**
 * @brief Handle login action trigger
 * @details Login action demonstration.
 */
void MainWindow::onLoginActionTriggered()
{
    textBrowser->append("Login triggered");
}

/**
 * @brief Handle add gallery item action trigger
 * @param galleryGroup The gallery group to add the item to
 * @details Add a new item to the specified gallery group.
 */
void MainWindow::onAddGalleryItemActionTriggered(SARibbonGalleryGroup *galleryGroup)
{
    if (galleryGroup)
    {
        QAction *newAction = new QAction(QIcon(":/gallery-icon/icon/gallery/Document-File.svg"), QString("New Action"));
        galleryGroup->addActionItem(newAction);
    }
}

/**
 * @brief Handle change language action trigger
 * @param toEnglish Whether to change to English
 * @details Toggle between Chinese and English panel names.
 */
void MainWindow::onChangeLanguageActionTriggered(bool toEnglish)
{
    SARibbonPanel *panelUtf8 = qobject_cast<SARibbonPanel *>(sender()->parent());
    if (toEnglish)
    {
        if (panelUtf8)
        {
            panelUtf8->setPanelName(QStringLiteral(u"show chinese char"));
        }
    }
    else
    {
        if (panelUtf8)
        {
            panelUtf8->setPanelName(QStringLiteral(u"中文显示测试"));
        }
    }
}

/**
 * @brief Handle context category 1 action trigger
 * @param visible Whether to make the context category visible
 * @details Set the visibility of context category 1.
 */
void MainWindow::onContextCategory1ActionTriggered(bool visible)
{
    ribbonBar()->setContextCategoryVisible(mContextCategory, visible);
}

/**
 * @brief Handle context category 2 action trigger
 * @param visible Whether to make the context category visible
 * @details Set the visibility of context category 2.
 */
void MainWindow::onContextCategory2ActionTriggered(bool visible)
{
    ribbonBar()->setContextCategoryVisible(mContextCategory2, visible);
}

/**
 * @brief Create main category page content
 * @param categoryPage Main category page pointer
 * @details Build panels and controls for the main category page, demonstrating basic usage of Ribbon.
 *          This function shows how to:
 *          - Create panels with different button styles
 *          - Set up keyboard shortcuts
 *          - Configure ribbon styles and themes
 *          - Add various widgets to panels
 */
void MainWindow::createCategoryMain(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelStyle = categoryPage->addPanel(tr("ribbon style"));

    QAction *actionSave = createAction(tr("Save"), ":/icon/icon/save.svg");
    connect(actionSave, &QAction::triggered, this, &MainWindow::onSaveActionTriggered);
    actionSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    addAction(actionSave);
    panelStyle->addLargeAction(actionSave);

    QAction *actionHideRibbon = createAction(tr("hide ribbon"), ":/icon/icon/hideRibbon.svg", "actHideRibbon");
    actionHideRibbon->setCheckable(true);
    panelStyle->addSmallAction(actionHideRibbon);
    connect(actionHideRibbon, &QAction::triggered, this, &MainWindow::onHideRibbonActionTriggered);
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [actionHideRibbon](SARibbonBar::RibbonMode currentMode)
            { actionHideRibbon->setChecked(currentMode == SARibbonBar::MinimumRibbonMode); });

    QAction *actionShowHideButton =
        createAction(tr("show \nhide button"), ":/icon/icon/showHideButton.svg", "show hide button");
    actionShowHideButton->setCheckable(true);
    actionShowHideButton->setChecked(ribbonBar()->haveShowMinimumModeButton());
    panelStyle->addSmallAction(actionShowHideButton);
    connect(actionShowHideButton, &QAction::triggered, this, &MainWindow::onShowHideButtonActionTriggered);

    mActionWordWrap = createAction(tr("word wrap"), ":/icon/icon/wordwrap.svg");
    mActionWordWrap->setCheckable(true);
    mActionWordWrap->setChecked(ribbonBar()->isEnableWordWrap());
    panelStyle->addSmallAction(mActionWordWrap);
    connect(mActionWordWrap, &QAction::triggered, this, &MainWindow::onActionWordWrapTriggered);

    QAction *actionShowTitleIcon =
        createAction(tr("show\n title icon"), ":/icon/icon/showHideButton.svg", "show hide title icon");
    actionShowTitleIcon->setCheckable(true);
    actionShowTitleIcon->setChecked(ribbonBar()->isTitleIconVisible());
    connect(actionShowTitleIcon, &QAction::triggered, this, &MainWindow::onShowTitleIconActionTriggered);
    panelStyle->addSmallAction(actionShowTitleIcon);

    QAction *actionAppBtnExpand = createAction(tr("App Btn\n Expand"), ":/icon/icon/layout.svg");
    actionAppBtnExpand->setCheckable(true);
    actionAppBtnExpand->setChecked(ribbonBar()->isApplicationButtonVerticalExpansion());
    panelStyle->addSmallAction(actionAppBtnExpand);
    connect(actionAppBtnExpand, &QAction::triggered, this, [this, actionShowTitleIcon](bool expand)
            {
        onAppButtonExpandActionTriggered(expand);
        if (expand) {
            actionShowTitleIcon->setChecked(false);
        } });

    panelStyle->addSeparator();

    QButtonGroup *styleButtonGroup = new QButtonGroup(categoryPage);

    QRadioButton *radioOfficeStyle = new QRadioButton();
    radioOfficeStyle->setText(tr("use office style"));
    radioOfficeStyle->setObjectName(("use office style"));
    radioOfficeStyle->setWindowTitle(radioOfficeStyle->text());
    radioOfficeStyle->setChecked(true);
    panelStyle->addSmallWidget(radioOfficeStyle);
    styleButtonGroup->addButton(radioOfficeStyle, SARibbonBar::RibbonStyleLooseThreeRow);

    QRadioButton *radioWpsStyle = new QRadioButton();
    radioWpsStyle->setObjectName(("use wps style"));
    radioWpsStyle->setText(tr("use wps style"));
    radioWpsStyle->setWindowTitle(radioWpsStyle->text());
    radioWpsStyle->setChecked(false);
    panelStyle->addSmallWidget(radioWpsStyle);
    styleButtonGroup->addButton(radioWpsStyle, SARibbonBar::RibbonStyleCompactThreeRow);

    QRadioButton *radioOffice2RowStyle = new QRadioButton();
    radioOffice2RowStyle->setObjectName(("use office 2row style"));
    radioOffice2RowStyle->setText(tr("use office 2 row style"));
    radioOffice2RowStyle->setWindowTitle(radioOffice2RowStyle->text());
    radioOffice2RowStyle->setChecked(false);
    panelStyle->addSmallWidget(radioOffice2RowStyle);
    styleButtonGroup->addButton(radioOffice2RowStyle, SARibbonBar::RibbonStyleLooseTwoRow);

    QRadioButton *radioWps2RowStyle = new QRadioButton();
    radioWps2RowStyle->setObjectName(("use wps 2row style"));
    radioWps2RowStyle->setText(tr("use wps 2row style"));
    radioWps2RowStyle->setWindowTitle(radioWps2RowStyle->text());
    radioWps2RowStyle->setChecked(false);
    panelStyle->addSmallWidget(radioWps2RowStyle);
    styleButtonGroup->addButton(radioWps2RowStyle, SARibbonBar::RibbonStyleCompactTwoRow);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(styleButtonGroup,
            static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::idClicked),
            this,
            &MainWindow::onStyleClicked);
#else
    connect(styleButtonGroup,
            static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,
            &MainWindow::onStyleClicked);
#endif

    mComboboxRibbonTheme = new QComboBox();
    mComboboxRibbonTheme->setWindowTitle(tr("RibbonTheme"));
    mComboboxRibbonTheme->setObjectName("RibbonTheme");
    mComboboxRibbonTheme->addItem("Theme Win7", static_cast<int>(SARibbonTheme::RibbonThemeWindows7));
    mComboboxRibbonTheme->addItem("Theme Office2013", static_cast<int>(SARibbonTheme::RibbonThemeOffice2013));
    mComboboxRibbonTheme->addItem("Theme Office2016 Blue", static_cast<int>(SARibbonTheme::RibbonThemeOffice2016Blue));
    mComboboxRibbonTheme->addItem("Theme Office2021 Blue", static_cast<int>(SARibbonTheme::RibbonThemeOffice2021Blue));
    mComboboxRibbonTheme->addItem("Theme Dark", static_cast<int>(SARibbonTheme::RibbonThemeDark));
    mComboboxRibbonTheme->addItem("Theme Dark2", static_cast<int>(SARibbonTheme::RibbonThemeDark2));
    mComboboxRibbonTheme->setCurrentIndex(mComboboxRibbonTheme->findData(static_cast<int>(ribbonTheme())));
    connect(mComboboxRibbonTheme,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::onRibbonThemeComboBoxCurrentIndexChanged);
    panelStyle->addSmallWidget(mComboboxRibbonTheme);

    QCheckBox *checkBoxAlignment = new QCheckBox(this);
    checkBoxAlignment->setText(tr("Alignment Center"));
    checkBoxAlignment->setObjectName("checkBoxAlignmentCenter");
    checkBoxAlignment->setWindowTitle(checkBoxAlignment->text());
    connect(checkBoxAlignment, &QCheckBox::clicked, this, &MainWindow::onCheckBoxAlignmentCenterClicked);
    panelStyle->addSmallWidget(checkBoxAlignment);

    SARibbonPanel *panelToolButtonStyle = categoryPage->addPanel(("sa ribbon toolbutton style"));

    SARibbonMenu *demoMenu = new SARibbonMenu(this);
    QAction *menuItemAction = nullptr;
    {
        QIcon itemIcon = QIcon(":/icon/icon/item.svg");
        for (int itemIndex = 0; itemIndex < 5; ++itemIndex)
        {
            menuItemAction = demoMenu->addAction(itemIcon, tr("item %1").arg(itemIndex + 1));
            menuItemAction->setObjectName(QStringLiteral("item %1").arg(itemIndex + 1));
        }
    }

    QAction *actionTest1 = createAction(tr("test 1"), ":/icon/icon/test1.svg");
    QVariant testData("Test");
    actionTest1->setData(testData);
    actionTest1->setMenu(demoMenu);
    actionTest1->setToolTip(tr("use QToolButton::MenuButtonPopup mode"));
    panelToolButtonStyle->addSmallAction(actionTest1, QToolButton::MenuButtonPopup);

    QAction *actionTest2 = createAction(tr("test 2"), ":/icon/icon/test2.svg");
    actionTest2->setMenu(demoMenu);
    actionTest2->setToolTip(tr("use QToolButton::InstantPopup mode"));
    panelToolButtonStyle->addSmallAction(actionTest2, QToolButton::InstantPopup);

    panelToolButtonStyle->addSeparator();

    QAction *actionDelayedPopup = createAction(tr("Delayed\nPopup"), ":/icon/icon/folder-cog.svg");
    actionDelayedPopup->setMenu(demoMenu);
    panelToolButtonStyle->addLargeAction(actionDelayedPopup, QToolButton::DelayedPopup);
    connect(actionDelayedPopup, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);

    QAction *actionMenuButtonPopup = createAction(tr("Menu Button Popup"), ":/icon/icon/folder-star.svg");
    actionMenuButtonPopup->setMenu(demoMenu);
    panelToolButtonStyle->addLargeAction(actionMenuButtonPopup, QToolButton::MenuButtonPopup);
    connect(actionMenuButtonPopup, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckabletriggered);

    QAction *actionInstantPopup = createAction(tr("Instant Popup"), ":/icon/icon/folder-stats.svg");
    actionInstantPopup->setMenu(demoMenu);
    panelToolButtonStyle->addLargeAction(actionInstantPopup, QToolButton::InstantPopup);
    connect(actionInstantPopup, &QAction::triggered, this, &MainWindow::onInstantPopupCheckabletriggered);

    QAction *actionDelayedPopupCheckable = createAction(tr("Delayed Popup checkable"), ":/icon/icon/folder-table.svg");
    actionDelayedPopupCheckable->setCheckable(true);
    actionDelayedPopupCheckable->setMenu(demoMenu);
    panelToolButtonStyle->addLargeAction(actionDelayedPopupCheckable, QToolButton::DelayedPopup);
    connect(actionDelayedPopupCheckable, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckableTest);

    QAction *actionMenuButtonPopupCheckable =
        createAction(tr("Menu Button Popup checkable"), ":/icon/icon/folder-checkmark.svg");
    actionMenuButtonPopupCheckable->setCheckable(true);
    actionMenuButtonPopupCheckable->setMenu(demoMenu);
    panelToolButtonStyle->addLargeAction(actionMenuButtonPopupCheckable, QToolButton::MenuButtonPopup);
    connect(actionMenuButtonPopupCheckable, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckableTest);

    QAction *actionDisabled = createAction(tr("disable action"), ":/icon/icon/disable.svg");
    actionDisabled->setCheckable(true);
    actionDisabled->setMenu(demoMenu);
    actionDisabled->setEnabled(false);
    panelToolButtonStyle->addLargeAction(actionDisabled);

    QAction *optionAction = new QAction(this);
    connect(optionAction, &QAction::triggered, this, [this](bool triggered)
            {
        Q_UNUSED(triggered);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered")); });
    panelToolButtonStyle->setOptionAction(optionAction);

    SARibbonPanel *panelContextTest = categoryPage->addPanel(("panel 2"));

    QAction *actionShowContext = createAction(tr("show Context"), ":/icon/icon/showContext.svg");
    actionShowContext->setCheckable(true);
    panelContextTest->addLargeAction(actionShowContext);
    connect(actionShowContext, &QAction::triggered, this, &MainWindow::onShowContextCategory);

    mActionDeleteContext = createAction(tr("delete Context"), ":/icon/icon/deleteContext.svg");
    panelContextTest->addLargeAction(mActionDeleteContext);
    connect(mActionDeleteContext, &QAction::triggered, this, &MainWindow::onDeleteContextActionTriggered);

    QAction *actionWordWrapDemo = createAction(tr("Word\nWrap"), ":/icon/icon/setText.svg");
    panelContextTest->addLargeAction(actionWordWrapDemo);
    connect(actionWordWrapDemo, &QAction::triggered, this, &MainWindow::onWordWrapDemoActionTriggered);

    QAction *actionWordWrapWithMenu = createAction(tr("Word \nWrap"), ":/icon/icon/setText.svg");
    actionWordWrapWithMenu->setMenu(demoMenu);
    panelContextTest->addLargeAction(actionWordWrapWithMenu);
    connect(actionWordWrapWithMenu, &QAction::triggered, this, &MainWindow::onWordWrapWithMenuActionTriggered);

    QAction *actionIconOnly = createAction(QString(), ":/icon/icon/setText.svg", "ToolButtonIconOnly");
    panelContextTest->addLargeAction(actionIconOnly);
    if (SARibbonToolButton *toolButton = panelContextTest->lastAddActionButton())
    {
        toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }

    QAction *actionTextOnly = createAction(tr("Text Only"), ":/icon/icon/setText.svg", "ToolButtonTextOnly");
    panelContextTest->addLargeAction(actionTextOnly);
    if (SARibbonToolButton *toolButton = panelContextTest->lastAddActionButton())
    {
        toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }

    SARibbonPanel *panelWidgetTest = categoryPage->addPanel(tr("widget test"));
    panelWidgetTest->setObjectName(QStringLiteral(u"panelWidgetTest"));

    QComboBox *comboBoxTest = new QComboBox(this);
    comboBoxTest->setObjectName("QComboBox test");
    for (int itemIndex = 0; itemIndex < 40; ++itemIndex)
    {
        comboBoxTest->addItem(QString("QComboBox test%1").arg(itemIndex + 1));
    }
    comboBoxTest->setEditable(true);
    panelWidgetTest->addSmallWidget(comboBoxTest);

    QComboBox *comboBoxEditable = new QComboBox(this);
    comboBoxEditable->setObjectName("ComboBox Editable");
    for (int itemIndex = 0; itemIndex < 40; ++itemIndex)
    {
        comboBoxEditable->addItem(QString("item %1").arg(itemIndex + 1));
    }
    panelWidgetTest->addSmallWidget(comboBoxEditable);

    QLineEdit *lineEditWidget = new QLineEdit(this);
    lineEditWidget->setObjectName("Line Edit");
    lineEditWidget->setText("LineEdit");
    panelWidgetTest->addSmallWidget(lineEditWidget);

    panelWidgetTest->addSeparator();

    QCalendarWidget *calendarWidget = new QCalendarWidget(this);
    calendarWidget->setObjectName(("calendarWidget"));
    panelWidgetTest->addLargeWidget(calendarWidget);
    QAction *calendarOptionAction = new QAction(this);
    connect(calendarOptionAction, &QAction::triggered, this, [this](bool triggered)
            {
        Q_UNUSED(triggered);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered")); });
    panelWidgetTest->setOptionAction(calendarOptionAction);

    panelWidgetTest->setVisible(true);
}

/**
 * @brief Create other category page content
 * @param categoryPage Other category page pointer
 * @details Build content for other category pages, demonstrating more Ribbon control usages.
 *          This function shows how to:
 *          - Create button groups
 *          - Use Gallery controls
 *          - Handle context categories
 */
void MainWindow::createCategoryOther(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelButtonGroups = new SARibbonPanel(tr("panel one"));
    panelButtonGroups->setObjectName("CategoryOther-panel1");
    categoryPage->addPanel(panelButtonGroups);

    SARibbonButtonGroupWidget *buttonGroup1 = new SARibbonButtonGroupWidget(panelButtonGroups);
    buttonGroup1->setObjectName("SARibbonButtonGroupWidget1");

    buttonGroup1->addAction(createAction(tr("Decrease Margin"), ":/icon/icon/Decrease-Margin.svg"));
    buttonGroup1->addAction(createAction(tr("Decrease Indent"), ":/icon/icon/Decrease-Indent.svg"));
    buttonGroup1->addAction(createAction(tr("Wrap Image Left"), ":/icon/icon/Wrap-Image Left.svg"));
    buttonGroup1->addAction(createAction(tr("Wrap Image Right"), ":/icon/icon/Wrap-Image Right.svg"));
    panelButtonGroups->addWidget(buttonGroup1, SARibbonPanelItem::Medium);

    SARibbonButtonGroupWidget *buttonGroup2 = new SARibbonButtonGroupWidget(panelButtonGroups);
    buttonGroup2->setObjectName("SARibbonButtonGroupWidget2");

    QAction *actionAlignRight = createAction(tr("Align Right"), ":/icon/icon/Align-Right.svg");
    actionAlignRight->setProperty("align", (int)Qt::AlignRight | Qt::AlignVCenter);
    buttonGroup2->addAction(actionAlignRight);
    QAction *actionAlignLeft = createAction(tr("Align Left"), ":/icon/icon/Align-Left.svg");
    actionAlignLeft->setProperty("align", (int)Qt::AlignLeft | Qt::AlignVCenter);
    buttonGroup2->addAction(actionAlignLeft);
    QAction *actionAlignCenter = createAction(tr("Align Center"), ":/icon/icon/Align-Center.svg");
    actionAlignCenter->setProperty("align", (int)Qt::AlignCenter);
    buttonGroup2->addAction(actionAlignCenter);
    panelButtonGroups->addWidget(buttonGroup2, SARibbonPanelItem::Medium);
    connect(buttonGroup2, &SARibbonButtonGroupWidget::actionTriggered, this, &MainWindow::onButtonGroupActionTriggered);

    SARibbonGallery *gallery = panelButtonGroups->addGallery();
    QList<QAction *> galleryActions;
    auto createGalleryAction = [this](const QString &text, const QString &iconPath) -> QAction *
    {
        QAction *galleryAction = this->createAction(text, iconPath);
        this->connect(galleryAction, &QAction::triggered, this, [this, text]()
                      {
            if (this->textBrowser) {
                this->textBrowser->append(QString("%1 triggered").arg(text));
            } });
        return galleryAction;
    };
    galleryActions.append(createGalleryAction("Document File", ":/gallery-icon/icon/gallery/Document-File.svg"));
    galleryActions.append(createGalleryAction("Download File", ":/gallery-icon/icon/gallery/Download-File.svg"));
    galleryActions.append(createGalleryAction("Drive File Four Word", ":/gallery-icon/icon/gallery/Drive-File.svg"));
    galleryActions.append(createGalleryAction("Dropbox File", ":/gallery-icon/icon/gallery/Dropbox-File.svg"));
    galleryActions.append(createGalleryAction("Email File", ":/gallery-icon/icon/gallery/Email-File.svg"));
    galleryActions.append(createGalleryAction("Encode File", ":/gallery-icon/icon/gallery/Encode-File.svg"));
    galleryActions.append(createGalleryAction("Favorit File", ":/gallery-icon/icon/gallery/Favorit-File.svg"));
    galleryActions.append(createGalleryAction("File Error", ":/gallery-icon/icon/gallery/File-Error.svg"));
    galleryActions.append(createGalleryAction("File Read Only", ":/gallery-icon/icon/gallery/File-Readonly.svg"));
    galleryActions.append(createGalleryAction("File Settings", ":/gallery-icon/icon/gallery/File-Settings.svg"));
    galleryActions.append(createGalleryAction("Presentation File", ":/gallery-icon/icon/gallery/Presentation-File.svg"));
    SARibbonGalleryGroup *galleryGroup1 = gallery->addCategoryActions(tr("Files"), galleryActions);
    galleryGroup1->setGalleryGroupStyle(SARibbonGalleryGroup::IconWithWordWrapText);
    galleryGroup1->setGridMinimumWidth(80);
    galleryActions.clear();
    galleryActions.append(createGalleryAction("Photoshop", ":/gallery-icon/icon/gallery/Photoshop.svg"));
    galleryActions.append(createGalleryAction("Internet-Explorer", ":/gallery-icon/icon/gallery/Internet-Explorer.svg"));
    galleryActions.append(createGalleryAction("Illustrator", ":/gallery-icon/icon/gallery/Illustrator.svg"));
    galleryActions.append(createGalleryAction("Google-Maps", ":/gallery-icon/icon/gallery/Google-Maps.svg"));
    galleryActions.append(createGalleryAction("Adobe", ":/gallery-icon/icon/gallery/Adobe.svg"));
    galleryActions.append(createGalleryAction("Word", ":/gallery-icon/icon/gallery/Word.svg"));
    SARibbonGalleryGroup *galleryGroup2 = gallery->addCategoryActions(tr("Apps"), galleryActions);
    galleryGroup2->setGridMinimumWidth(80);
    gallery->setCurrentViewGroup(galleryGroup1);

    QAction *galleryOptionAction = new QAction(this);
    galleryOptionAction->setObjectName(("add gallery item"));
    connect(galleryOptionAction, &QAction::triggered, this, [galleryGroup2]()
            {
        QAction* newGalleryAction =
            new QAction(QIcon(":/gallery-icon/icon/gallery/Document-File.svg"), QString("New Action"));
        galleryGroup2->addActionItem(newGalleryAction); });
    panelButtonGroups->setOptionAction(galleryOptionAction);

    SARibbonPanel *panelAppButton = new SARibbonPanel(tr("panel two"));
    panelAppButton->setObjectName("CategoryOther-panel2");
    categoryPage->addPanel(panelAppButton);
    QAction *actionRemoveAppBtn = createAction(tr("remove application button"), ":/icon/icon/remove-app-btn.svg");
    actionRemoveAppBtn->setCheckable(true);
    connect(actionRemoveAppBtn, &QAction::toggled, this, &MainWindow::onActionRemoveAppBtnTriggered);
    panelAppButton->addLargeAction(actionRemoveAppBtn);

    QAction *actionLongText =
        createAction(tr("show very long text in a button,balabalabala etc"), ":/icon/icon/long-text.svg", "long-text");
    panelAppButton->addLargeAction(actionLongText);

    SARibbonPanel *panelStyle = new SARibbonPanel(tr("style"));
    panelStyle->setObjectName("CategoryOther-panelStyle");
    categoryPage->addPanel(panelStyle);

    QAction *actionUseQss = createAction(tr("use qss"), ":/icon/icon/useqss.svg");
    connect(actionUseQss, &QAction::triggered, this, &MainWindow::onActionUseQssTriggered);
    panelStyle->addSmallAction(actionUseQss);

    QAction *actionLoadCustomizeXmlFile = createAction(tr("load customize from xml file"), ":/icon/icon/useCustomize.svg");
    connect(actionLoadCustomizeXmlFile, &QAction::triggered, this, &MainWindow::onActionLoadCustomizeXmlFileTriggered);
    panelStyle->addSmallAction(actionLoadCustomizeXmlFile);

    QAction *actionWindowFlagNormalButton = createAction(tr("window normal button"), ":/icon/icon/windowsflag-normal.svg");
    actionWindowFlagNormalButton->setCheckable(true);
    actionWindowFlagNormalButton->setChecked(true);
    panelStyle->addSmallAction(actionWindowFlagNormalButton);
    connect(actionWindowFlagNormalButton, &QAction::triggered, this, &MainWindow::onActionWindowFlagNormalButtonTriggered);

    SARibbonPanel *panelUtf8 = new SARibbonPanel(QStringLiteral(u"中文显示测试"));
    panelUtf8->setObjectName("CategoryOther-panelUtf8");
    categoryPage->addPanel(panelUtf8);

    QAction *actionChangeText =
        createAction(QStringLiteral(u"改变显示为英文"), ":/icon/icon/chinese-char.svg", "actionChangeText");
    actionChangeText->setCheckable(true);
    actionChangeText->setChecked(false);
    panelUtf8->addLargeAction(actionChangeText);
    connect(actionChangeText, &QAction::triggered, this, [panelUtf8, actionChangeText](bool toEnglish)
            {
        if (toEnglish) {
            panelUtf8->setPanelName(QStringLiteral(u"show chinese char"));
            actionChangeText->setText(QStringLiteral(u"show in chinese"));
        } else {
            panelUtf8->setPanelName(QStringLiteral(u"中文显示测试"));
            actionChangeText->setText(QStringLiteral(u"改变显示为英文"));
        } });

    SARibbonPanel *panelContextCategory = new SARibbonPanel(("Context Category"));
    categoryPage->addPanel(panelContextCategory);
    QAction *actionContextCategory1 = createAction("Context Category 1", ":/icon/icon/ContextCategory.svg");
    actionContextCategory1->setCheckable(true);
    connect(actionContextCategory1, &QAction::triggered, this, &MainWindow::onContextCategory1ActionTriggered);

    QAction *actionContextCategory2 = createAction("Context Category 2", ":/icon/icon/ContextCategory.svg");
    actionContextCategory2->setCheckable(true);
    connect(actionContextCategory2, &QAction::triggered, this, &MainWindow::onContextCategory2ActionTriggered);
    panelContextCategory->addLargeAction(actionContextCategory1);
    panelContextCategory->addLargeAction(actionContextCategory2);
}

/**
 * @brief Create delete category page content
 * @param categoryPage Delete category page pointer
 * @details Demonstrate how to dynamically create and delete Ribbon category pages.
 *          This function shows how to:
 *          - Remove panels from categories
 *          - Remove categories from the ribbon bar
 *          - Insert panels at specific positions
 */
void MainWindow::createCategoryDelete(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelRemoveTest = new SARibbonPanel(("panel 1"));
    SARibbonPanel *panelCategoryTest = new SARibbonPanel(("panel 2"));

    QAction *actionRemovePanel = createAction(tr("remove panel"), ":/icon/icon/remove.svg");
    connect(actionRemovePanel, &QAction::triggered, this, [this, categoryPage, panelCategoryTest]()
            { onRemovePanelActionTriggered(categoryPage, panelCategoryTest); });
    panelRemoveTest->addLargeAction(actionRemovePanel);

    QAction *actionRemoveCategory = createAction(tr("remove this category"), ":/icon/icon/removePage.svg");
    connect(actionRemoveCategory, &QAction::triggered, this, [this, categoryPage, actionRemoveCategory]()
            {
        onRemoveCategoryActionTriggered(categoryPage);
        actionRemoveCategory->setDisabled(true); });
    panelCategoryTest->addLargeAction(actionRemoveCategory);

    categoryPage->addPanel(panelRemoveTest);
    categoryPage->addPanel(panelCategoryTest);

    SARibbonPanel *panelInsertTest = categoryPage->addPanel(("insert panel test"));
    QAction *actionInsertPanelAt0 = createAction(tr("insert at 0"), ":/icon/icon/test1.svg");
    QAction *actionInsertPanelAtEnd = createAction(tr("insert at end"), ":/icon/icon/test2.svg");
    QAction *actionInsertPanelAtNeg = createAction(tr("insert at -1"), ":/icon/icon/item.svg");

    connect(actionInsertPanelAt0, &QAction::triggered, this, [this, categoryPage]()
            { onInsertPanelAtStartActionTriggered(categoryPage); });
    connect(actionInsertPanelAtEnd, &QAction::triggered, this, [this, categoryPage]()
            { onInsertPanelAtEndActionTriggered(categoryPage); });
    connect(actionInsertPanelAtNeg, &QAction::triggered, this, [this, categoryPage]()
            { onInsertPanelAtNegativeOneActionTriggered(categoryPage); });

    panelInsertTest->addLargeAction(actionInsertPanelAt0);
    panelInsertTest->addLargeAction(actionInsertPanelAtEnd);
    panelInsertTest->addLargeAction(actionInsertPanelAtNeg);
}

/**
 * @brief Create size category page content
 * @param categoryPage Size category page pointer
 * @details Build size adjustment page, demonstrating the use of font and size related controls.
 *          This function shows how to:
 *          - Use font comboboxes in ribbon panels
 *          - Create button groups for font tools
 *          - Adjust font sizes dynamically
 */
void MainWindow::createCategorySize(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelFont = categoryPage->addPanel(tr("Font"));
    SARibbonButtonGroupWidget *groupFontFamily = new SARibbonButtonGroupWidget(panelFont);
    groupFontFamily->setObjectName(QStringLiteral(u"group1"));
    QLabel *labelFontSize = new QLabel(groupFontFamily);
    labelFontSize->setText(tr("select font"));
    labelFontSize->setObjectName(QStringLiteral(u"labelFontSize"));
    groupFontFamily->addWidget(labelFontSize);
    QFontComboBox *fontComboBox = new QFontComboBox(groupFontFamily);
    fontComboBox->setObjectName(QStringLiteral(u"fontComboBox"));
    connect(fontComboBox, &QFontComboBox::currentFontChanged, this, &MainWindow::onFontComWidgetCurrentFontChanged);
    groupFontFamily->setWindowTitle(tr("Font ComboBox"));
    groupFontFamily->addWidget(fontComboBox);
    QAction *actionWidgetFontFamily = panelFont->addWidget(groupFontFamily, SARibbonPanelItem::Medium);
    actionWidgetFontFamily->setObjectName(QStringLiteral("Size.Font.FontFamile"));

    SARibbonButtonGroupWidget *groupFontTools = new SARibbonButtonGroupWidget(panelFont);
    groupFontTools->setObjectName(QStringLiteral(u"group2"));
    groupFontTools->addAction(createAction("Bold", ":/icon/icon/bold.svg"));
    groupFontTools->addAction(createAction("Italic", ":/icon/icon/Italic.svg"));
    groupFontTools->addSeparator();
    groupFontTools->addAction(createAction("left alignment", ":/icon/icon/al-left.svg"));
    groupFontTools->addAction(createAction("center alignment", ":/icon/icon/al-center.svg"));
    groupFontTools->addAction(createAction("right alignment", ":/icon/icon/al-right.svg"));
    groupFontTools->addAction(createAction("line up on both sides", ":/icon/icon/al-bothside.svg"));
    groupFontTools->setWindowTitle(tr("Font Tools"));
    QAction *actionWidgetFontTools = panelFont->addWidget(groupFontTools, SARibbonPanelItem::Medium);
    actionWidgetFontTools->setObjectName(QStringLiteral("Size.Font.FontTools"));

    panelFont->addSeparator();

    QAction *actionLargerFontSize = createAction(tr("Larger"), ":/icon/icon/largerFont.svg", "actLargerFontSize");
    panelFont->addLargeAction(actionLargerFontSize);

    QAction *actionSmallFontSize = createAction(tr("Smaller"), ":/icon/icon/smallFont.svg", "actSmallFontSize");
    panelFont->addLargeAction(actionSmallFontSize);

    connect(actionLargerFontSize, &QAction::triggered, this, &MainWindow::onActionFontLargerTriggered);
    connect(actionSmallFontSize, &QAction::triggered, this, &MainWindow::onActionFontSmallerTriggered);

    SARibbonPanel *panelFontSize = new SARibbonPanel(tr("Font Size"));
    QFont panelFontSetting = font();
    panelFontSetting.setPointSize(14);
    panelFontSize->setFont(panelFontSetting);
    QAction *actionLargeFontText = createAction(tr("Larger Font Text"), ":/icon/icon/file.svg", "actLargeFontText");
    panelFontSize->addLargeAction(actionLargeFontText);
    categoryPage->addPanel(panelFontSize);
}

/**
 * @brief Create color category page content
 * @param categoryPage Color category page pointer
 * @details Build color-related page, demonstrating the use of color selection controls.
 *          This function shows how to:
 *          - Use SARibbonColorToolButton for color selection
 *          - Configure different color button styles
 *          - Use SAColorPaletteGridWidget for color palettes
 */
void MainWindow::createCategoryColor(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelColor = categoryPage->addPanel(tr("color"));

    auto createColorButton = [this, panelColor](const QColor &defaultColor = Qt::red) -> SARibbonColorToolButton *
    {
        SARibbonColorToolButton *colorButton = new SARibbonColorToolButton(panelColor);
        colorButton->setColor(defaultColor);
        colorButton->setupStandardColorMenu();
        this->connect(colorButton, &SARibbonColorToolButton::colorClicked, this, &MainWindow::onColorButtonColorClicked);
        return colorButton;
    };
    SARibbonColorToolButton *colorButton = createColorButton();
    colorButton->setObjectName("ColorFillToIcon-NoIconNoText");
    colorButton->setColorStyle(SARibbonColorToolButton::ColorFillToIcon);
    panelColor->addSmallWidget(colorButton);

    colorButton = createColorButton(Qt::blue);
    colorButton->setColorStyle(SARibbonColorToolButton::ColorFillToIcon);
    colorButton->setObjectName("ColorFillToIcon-NoIconHaveText");
    colorButton->setText("No Icon have text");
    panelColor->addSmallWidget(colorButton);

    colorButton = createColorButton(QColor());
    colorButton->setIcon(QIcon(":/icon/icon/long-text.svg"));
    panelColor->addSmallWidget(colorButton);

    colorButton = createColorButton(Qt::red);
    colorButton->setIcon(QIcon(":/icon/icon/long-text.svg"));
    colorButton->setText("have Icon have text");
    panelColor->addSmallWidget(colorButton);

    colorButton = createColorButton(QColor());
    colorButton->setButtonType(SARibbonToolButton::LargeButton);
    colorButton->setObjectName("ColorFillToIcon-LargeColorButton");
    colorButton->setColorStyle(SARibbonColorToolButton::ColorFillToIcon);
    colorButton->setText("Large Color Button");
    panelColor->addLargeWidget(colorButton);

    colorButton = createColorButton(QColor());
    colorButton->setButtonType(SARibbonToolButton::LargeButton);
    colorButton->setIcon(QIcon(":/icon/icon/long-text.svg"));
    colorButton->setObjectName("ColorUnderIcon-LargeColorButton");
    colorButton->setText("Large Color Button");
    panelColor->addLargeWidget(colorButton);

    panelColor->addSeparator();
    SAColorPaletteGridWidget *colorPaletteWidget = new SAColorPaletteGridWidget(SA::getStandardColorList(), panelColor);
    colorPaletteWidget->setFactor({75, 120});
    panelColor->addLargeWidget(colorPaletteWidget);
}

/**
 * @brief Create context category 1
 * @details Create context-related category pages, which are usually displayed under specific conditions.
 *          Context categories are special tabs that appear when certain objects or modes are active.
 */
void MainWindow::createContextCategory1()
{
    SARibbonBar *currentRibbonBar = ribbonBar();
    mContextCategory = currentRibbonBar->addContextCategory(tr("context"), QColor(), 1);

    SARibbonCategory *contextCategoryPage1 = mContextCategory->addCategoryPage(tr("Page1"));
    createContextCategoryPage1(contextCategoryPage1);

    SARibbonCategory *contextCategoryPage2 = mContextCategory->addCategoryPage(tr("Page2"));
    createContextCategoryPage2(contextCategoryPage2);
}

/**
 * @brief Create context category 2
 * @details Create another context tag, demonstrating the management of multiple context tags.
 */
void MainWindow::createContextCategory2()
{
    SARibbonBar *currentRibbonBar = ribbonBar();
    mContextCategory2 = currentRibbonBar->addContextCategory(("context2"), QColor(), 2);
    mContextCategory2->addCategoryPage(("context2 Page1"));
    mContextCategory2->addCategoryPage(("context2 Page2"));
}

/**
 * @brief Create first context category page content
 * @param categoryPage First context category page pointer
 * @details Build content for the first context category page, demonstrating the unique features of context pages.
 *          This function shows how to:
 *          - Control action visibility
 *          - Enable/disable actions dynamically
 *          - Use keyboard shortcuts in context pages
 */
void MainWindow::createContextCategoryPage1(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelShowHide = categoryPage->addPanel(tr("show and hide test"));

    QAction *actionHideActions = createAction("hide action", ":/icon/icon/hidePanel.svg");
    actionHideActions->setCheckable(true);
    actionHideActions->setChecked(true);
    panelShowHide->addLargeAction(actionHideActions);
    connect(actionHideActions, &QAction::triggered, this, &MainWindow::onActionHideActionTriggered);

    mActionDisable = createAction(tr("Disable"), ":/icon/icon/enableTest.svg");
    mActionDisable->setDisabled(true);
    panelShowHide->addLargeAction(mActionDisable);
    connect(mActionDisable, &QAction::triggered, this, [this](bool triggered)
            {
        Q_UNUSED(triggered);
        mActionDisable->setDisabled(true); });

    mActionUnlock = createAction(tr("unlock"), ":/icon/icon/unlock.svg");
    mActionUnlock->setShortcut(QKeySequence(QLatin1String("Ctrl+E")));
    mActionUnlock->setShortcutContext(Qt::ApplicationShortcut);
    panelShowHide->addLargeAction(mActionUnlock);
    connect(mActionUnlock, &QAction::triggered, this, &MainWindow::onUnlockActionTriggered);

    QAction *actionShortText = createAction(tr("1"), ":/icon/icon/unlock.svg");
    actionShortText->setToolTip(tr("very short string"));
    panelShowHide->addLargeAction(actionShortText);

    mActionSetTextTest = createAction("set text", ":/icon/icon/setText.svg");
    mActionSetTextTest->setCheckable(true);
    mActionSetTextTest->setShortcut(QKeySequence(QLatin1String("Ctrl+D")));
    mActionSetTextTest->setShortcutContext(Qt::ApplicationShortcut);
    panelShowHide->addLargeAction(mActionSetTextTest);
    connect(mActionSetTextTest, &QAction::toggled, this, &MainWindow::onSetTextActionToggled);

    mActionShowTest = createAction("show beside panel", ":/icon/icon/show.svg");

    mActionShowTest->setCheckable(true);
    panelShowHide->addLargeAction(mActionShowTest);

    mPanelVisbileExample = categoryPage->addPanel(tr("show/hide"));
    mPanelVisbileExample->addLargeAction(mActionSetTextTest);

    connect(mActionShowTest, &QAction::toggled, this, [this](bool visible)
            {
        mPanelVisbileExample->setVisible(!visible);
        if (visible) {
            mActionShowTest->setText(tr("hide beside panel"));
        } else {
            mActionShowTest->setText(tr("show beside panel"));
        }
        ribbonBar()->repaint(); });

    SARibbonPanel *panelShowHideAction = categoryPage->addPanel(("show/hide action test"));

    mActionHideAction2 = createAction("hide action 2", ":/icon/icon/action.svg");
    mActionHideAction4 = createAction("hide action 4", ":/icon/icon/action.svg");
    mActionHideAction2->setCheckable(true);
    mActionHideAction2->setChecked(true);
    mActionHideAction4->setCheckable(true);
    mActionHideAction4->setChecked(true);
    mActionHideShowTextAct2 = createAction("action2", ":/icon/icon/action2.svg");
    mActionHideShowTextAct3 = createAction("action3", ":/icon/icon/action3.svg");
    mActionHideShowTextAct4 = createAction("action4", ":/icon/icon/action4.svg");
    panelShowHideAction->addLargeAction(mActionHideAction2);
    panelShowHideAction->addLargeAction(mActionHideAction4);
    panelShowHideAction->addSmallAction(mActionHideShowTextAct2);
    panelShowHideAction->addSmallAction(mActionHideShowTextAct3);
    panelShowHideAction->addSmallAction(mActionHideShowTextAct4);
    connect(mActionHideAction2, &QAction::triggered, this, &MainWindow::onHideAction2Triggered);
    connect(mActionHideAction4, &QAction::triggered, this, &MainWindow::onHideAction4Triggered);

    SARibbonPanel *panelWidget = categoryPage->addPanel(("widget"));
    QSpinBox *spinBoxWidget = new QSpinBox(this);
    SARibbonCtrlContainer *ctrlContainer1 = new SARibbonCtrlContainer(panelWidget);
    ctrlContainer1->setContainerWidget(spinBoxWidget);
    ctrlContainer1->setText(tr("spinbox:"));
    ctrlContainer1->setEnableShowIcon(false);
    ctrlContainer1->setMaximumHeight(fontMetrics().lineSpacing() * 1.5);
    panelWidget->addMediumWidget(ctrlContainer1);

    QLineEdit *lineEditWidget = new QLineEdit(this);
    SARibbonCtrlContainer *ctrlContainer2 = new SARibbonCtrlContainer(panelWidget);
    ctrlContainer2->setContainerWidget(lineEditWidget);
    ctrlContainer2->setText(tr("linedit:"));
    ctrlContainer2->setEnableShowIcon(false);
    ctrlContainer2->setMaximumHeight(fontMetrics().lineSpacing() * 1.5);
    panelWidget->addMediumWidget(ctrlContainer2);
}

/**
 * @brief Create second context category page content
 * @param categoryPage Second context category page pointer
 * @details Build content for the second context category page, demonstrating special layouts and control combinations.
 *          This function shows how to:
 *          - Delete categories dynamically
 *          - Create custom panel layouts with different item sizes
 *          - Use separators in panels
 */
void MainWindow::createContextCategoryPage2(SARibbonCategory *categoryPage)
{
    SARibbonPanel *panelDeleteTest = categoryPage->addPanel(("删除CategoryPage测试"));

    QAction *actionDeleteThisCategory = createAction("delete this category", ":/icon/icon/delete.svg");
    panelDeleteTest->addLargeAction(actionDeleteThisCategory);
    connect(actionDeleteThisCategory, &QAction::triggered, this, [this, categoryPage]()
            { onDeleteThisCategoryActionTriggered(categoryPage); });

    SARibbonPanel *panelLayout = categoryPage->addPanel(("特殊布局"));

    panelLayout->addAction(
        createAction("Large", ":/icon/icon/layout.svg", "@Large1"), QToolButton::InstantPopup, SARibbonPanelItem::Large);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small1"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small2"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addSeparator();
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small3"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small4"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small5"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small6"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addSeparator();
    panelLayout->addAction(
        createAction("Large", ":/icon/icon/layout.svg", "@Large2"), QToolButton::InstantPopup, SARibbonPanelItem::Large);
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium1"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium2"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small7"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium3"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(
        createAction("Large", ":/icon/icon/layout.svg", "@Large3"), QToolButton::InstantPopup, SARibbonPanelItem::Large);
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium4"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium5"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(
        createAction("Large", ":/icon/icon/layout.svg", "@Large4"), QToolButton::InstantPopup, SARibbonPanelItem::Large);
    panelLayout->addSeparator();
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium6"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(
        createAction("Large", ":/icon/icon/layout.svg", "@Large5"), QToolButton::InstantPopup, SARibbonPanelItem::Large);
    panelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium7"),
                           QToolButton::InstantPopup,
                           SARibbonPanelItem::Medium);
    panelLayout->addAction(
        createAction("Small", ":/icon/icon/layout.svg", "@Small8"), QToolButton::InstantPopup, SARibbonPanelItem::Small);
}

/**
 * @brief Create quick access toolbar
 * @details Build quick access toolbar, placing commonly used operation buttons.
 *          The quick access toolbar is located at the top of the ribbon bar and provides
 *          quick access to frequently used commands.
 */
void MainWindow::createQuickAccessBar()
{
    SARibbonQuickAccessBar *quickAccessBar = ribbonBar()->quickAccessBar();

    quickAccessBar->addAction(createAction("save", ":/icon/icon/save.svg", "save-quickbar"));
    quickAccessBar->addSeparator();

    QAction *actionUndo = createAction("undo", ":/icon/icon/undo.svg");
    actionUndo->setShortcut(QKeySequence("Ctrl+Shift+z"));
    actionUndo->setShortcutContext(Qt::ApplicationShortcut);
    quickAccessBar->addAction(actionUndo);

    QAction *actionRedo = createAction("redo", ":/icon/icon/redo.svg");
    actionRedo->setShortcut(QKeySequence("Ctrl+z"));
    actionRedo->setShortcutContext(Qt::ApplicationShortcut);
    quickAccessBar->addAction(actionRedo);

    connect(actionUndo, &QAction::triggered, this, &MainWindow::onUndoActionTriggered);
    connect(actionRedo, &QAction::triggered, this, &MainWindow::onRedoActionTriggered);

    quickAccessBar->addSeparator();

    QMenu *menuPresentationFile1 = new QMenu("Presentation File 1", this);
    menuPresentationFile1->setIcon(QIcon(":/icon/icon/presentationFile.svg"));
    for (int fileIndex = 0; fileIndex < 10; ++fileIndex)
    {
        menuPresentationFile1->addAction(createAction(QString("file%1 - 1").arg(fileIndex + 1), ":/icon/icon/file.svg"));
    }
    quickAccessBar->addMenuAction(menuPresentationFile1->menuAction());

    QMenu *menuPresentationFile2 = new QMenu("Presentation File 2", this);
    menuPresentationFile2->setIcon(QIcon(":/icon/icon/presentationFile.svg"));
    for (int fileIndex = 0; fileIndex < 10; ++fileIndex)
    {
        menuPresentationFile2->addAction(createAction(QString("file%1 - 2").arg(fileIndex + 1), ":/icon/icon/file.svg"));
    }
    quickAccessBar->addMenuAction(menuPresentationFile2, QToolButton::MenuButtonPopup);

    QAction *actionCustomize = createAction("customize", ":/icon/icon/customize0.svg", "customize2");
    quickAccessBar->addAction(actionCustomize);
    connect(actionCustomize, &QAction::triggered, this, &MainWindow::onActionCustomizeTriggered);

    QAction *actionCustomizeAndSave = createAction("customize and save", ":/icon/icon/customize.svg");
    quickAccessBar->addAction(actionCustomizeAndSave);
    connect(actionCustomizeAndSave, &QAction::triggered, this, &MainWindow::onActionCustomizeAndSaveTriggered);

    QAction *actionCustomizeAndSaveWithApply = createAction("customize and save with apply", ":/icon/icon/customize.svg");
    quickAccessBar->addAction(actionCustomizeAndSaveWithApply);
    connect(actionCustomizeAndSaveWithApply, &QAction::triggered, this, &MainWindow::onActionCustomizeAndSaveWithApplyTriggered);

    mSearchEditor = new QLineEdit(this);
    mSearchEditor->setMinimumWidth(150);
    mSearchEditor->setPlaceholderText("Search");
    quickAccessBar->addWidget(mSearchEditor);
    connect(mSearchEditor, &QLineEdit::editingFinished, this, &MainWindow::onSearchEditorEditingFinished);
}

/**
 * @brief Create right button group
 * @details RightButtonGroup is a toolbar on the right side of the ribbonbar where you can place some shortcut icons.
 *          This is useful for placing help buttons, settings, and other frequently accessed commands.
 */
void MainWindow::createRightButtonGroup()
{
    SARibbonBar *currentRibbonBar = ribbonBar();
    if (!currentRibbonBar)
    {
        return;
    }
    SARibbonButtonGroupWidget *rightBar = currentRibbonBar->rightButtonGroup();
    QAction *actionHelp = createAction(tr("help"), ":/icon/icon/help.svg");
    mActionVisibleAll = createAction(tr("Visible"), ":/icon/icon/visible-true.svg");
    mActionVisibleAll->setCheckable(true);
    mActionVisibleAll->setChecked(true);
    connect(actionHelp, &QAction::triggered, this, &MainWindow::onActionHelpTriggered);
    connect(mActionVisibleAll, &QAction::triggered, this, &MainWindow::onActionVisibleAllTriggered);
    rightBar->addAction(actionHelp);
    rightBar->addAction(mActionVisibleAll);
}

/**
 * @brief Create window button group bar
 * @details Add custom tool buttons next to system window buttons (minimize, maximize, close).
 *          This allows you to add custom buttons to the title bar area.
 */
void MainWindow::createWindowButtonGroupBar()
{
    SARibbonSystemButtonBar *windowButtonBar = this->windowButtonBar();
    if (!windowButtonBar)
    {
        return;
    }
    QAction *actionLogin = new QAction(QIcon(), tr("Login"), this);
    QAction *actionHelp = createAction(tr("help"), ":/icon/icon/help.svg");
    connect(actionLogin, &QAction::triggered, this, &MainWindow::onLoginActionTriggered);
    connect(actionHelp, &QAction::triggered, this, &MainWindow::onActionHelpTriggered);
    windowButtonBar->addAction(actionLogin);
    windowButtonBar->addAction(actionHelp);
}

/**
 * @brief Create ActionsManager
 * @details Create ActionsManager to manage actions and customize SARibbonBar.
 *          The ActionsManager is essential for:
 *          - Managing all actions in the application
 *          - Enabling ribbon customization features
 *          - Categorizing actions for the customize dialog
 */
void MainWindow::createActionsManager()
{
    createOtherActions();
    mTagForActionText = SARibbonActionsManager::UserDefineActionTag + 1;
    mTagForActionIcon = SARibbonActionsManager::UserDefineActionTag + 2;

    mActionsManager = new SARibbonActionsManager(ribbonBar());

    mActionsManager->registeAction(mOtherAction1, SARibbonActionsManager::CommonlyUsedActionTag);
    mActionsManager->registeAction(mOtherAction3, SARibbonActionsManager::CommonlyUsedActionTag);
    mActionsManager->registeAction(mOtherAction5, SARibbonActionsManager::CommonlyUsedActionTag);
    mActionsManager->registeAction(mOtherActionIcon1, SARibbonActionsManager::CommonlyUsedActionTag);

    mActionsManager->registeAction(mOtherAction1, mTagForActionText);
    mActionsManager->registeAction(mOtherAction2, mTagForActionText);
    mActionsManager->registeAction(mOtherAction3, mTagForActionText);
    mActionsManager->registeAction(mOtherAction4, mTagForActionText);
    mActionsManager->registeAction(mOtherAction5, mTagForActionText);

    mActionsManager->registeAction(mOtherActionIcon1, mTagForActionIcon);

    mActionsManager->setTagName(SARibbonActionsManager::CommonlyUsedActionTag, tr("in common use"));
    mActionsManager->setTagName(mTagForActionText, tr("no icon action"));
    mActionsManager->setTagName(mTagForActionIcon, tr("have icon action"));
}

/**
 * @brief Create an action with text, icon, and object name
 * @param text Action text
 * @param iconPath Icon path
 * @param objName Object name
 * @return Created action pointer
 * @details This is a helper function to create QAction objects with consistent settings.
 *          The object name is important for action management and customization.
 */
QAction *MainWindow::createAction(const QString &text, const QString &iconPath, const QString &objName)
{
    QAction *newAction = new QAction(this);
    newAction->setText(text);
    newAction->setIcon(QIcon(iconPath));
    newAction->setObjectName(objName);
    return newAction;
}

/**
 * @brief Create an action with text and icon (overloaded version)
 * @param text Action text
 * @param iconPath Icon path
 * @return Created action pointer
 * @details This overloaded version auto-generates an object name based on the text.
 */
QAction *MainWindow::createAction(const QString &text, const QString &iconPath)
{
    QAction *newAction = new QAction(this);
    newAction->setText(text);
    newAction->setIcon(QIcon(iconPath));
    newAction->setObjectName(text);
    return newAction;
}

/**
 * @brief Test the checkable function in menu button popup mode
 * @param checked Whether the button is checked
 */
void MainWindow::onMenuButtonPopupCheckableTest(bool checked)
{
    textBrowser->append(QString("MenuButtonPopupCheckableTest : %1").arg(checked));
}

/**
 * @brief Test the checkable function in instant popup mode
 * @param checked Whether the button is checked
 */
void MainWindow::onInstantPopupCheckableTest(bool checked)
{
    textBrowser->append(QString("InstantPopupCheckableTest : %1").arg(checked));
}

/**
 * @brief Test the checkable function in delayed popup mode
 * @param checked Whether the button is checked
 */
void MainWindow::onDelayedPopupCheckableTest(bool checked)
{
    textBrowser->append(QString("DelayedPopupCheckableTest : %1").arg(checked));
}

/**
 * @brief Handle trigger events for checkable actions in menu button popup mode
 * @param checked Whether the button is checked
 */
void MainWindow::onMenuButtonPopupCheckabletriggered(bool checked)
{
    textBrowser->append(QString("MenuButtonPopupCheckabletriggered : %1").arg(checked));
}

/**
 * @brief Handle trigger events for checkable actions in instant popup mode
 * @param checked Whether the button is checked
 */
void MainWindow::onInstantPopupCheckabletriggered(bool checked)
{
    textBrowser->append(QString("InstantPopupCheckabletriggered : %1").arg(checked));
}

/**
 * @brief Handle trigger events for checkable actions in delayed popup mode
 * @param checked Whether the button is checked
 * @details This also demonstrates how to use SARibbonToolButton::setPopupMode(QToolButton::DelayedPopup)
 *          to set the menu pop-up method to delayed pop-up.
 */
void MainWindow::onDelayedPopupCheckabletriggered(bool checked)
{
    Q_UNUSED(checked);
    textBrowser->append(tr("The SARibbonToolButton::setPopupMode(QToolButton::DelayedPopup) method "
                           "can be used to set the menu pop-up method to delayed pop-up. "
                           "This also demonstrates manually setting text wrapping"));
}
