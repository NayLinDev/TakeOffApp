#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>

#include <QFileDialog>
#include <Qslider>
#include <QLabel>
#include <QToolBar>
#include <QMessageBox>
#include <QSpinBox>
#include "pdfviewer.h"

#include "SARibbonBar.h"

#include "testfunctions/test_pdfium.h"
#include "testfunctions/test_podofo.h"

void log_out_put(QtMsgType type, const QMessageLogContext &context, const QString &msg);

/**
 * @brief redefine qdebug
 * @param type
 * @param context
 * @param msg
 */
void log_out_put(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type)
    {
    case QtDebugMsg:
        fprintf(stdout, "%s |[Debug] (%s[%u],%s)\n", localMsg.constData(), context.function, context.line, context.file);
        break;

    case QtWarningMsg:
        fprintf(stdout, "%s |[Warning] (%s[%u],%s)\n", localMsg.constData(), context.function, context.line, context.file);
        break;

    case QtCriticalMsg:
        fprintf(stdout, "%s |[Critical] (%s[%u],%s)\n", localMsg.constData(), context.function, context.line, context.file);
        break;

    case QtFatalMsg:
        fprintf(stdout, "%s |[Fatal] (%s[%u],%s)\n", localMsg.constData(), context.function, context.line, context.file);
        abort();
        break;

    default:
        fprintf(stdout, "%s |[Debug](%s[%u],%s)\n", localMsg.constData(), context.function, context.line, context.file);
        break;
    }
#ifndef QT_NO_DEBUG_OUTPUT
    fflush(stdout);
#endif
}

int main(int argc, char *argv[])
{
    SARibbonBar::initHighDpi();
    test_podofo();
    test_pdfium();
    QApplication a(argc, argv);
    qInstallMessageHandler(log_out_put);
#ifdef SA_RIBBON_BAR_NO_EXPORT
    Q_INIT_RESOURCE(SARibbonResource);
#endif
    QFont f = a.font();
    f.setFamily("Aerial");
    f.setPixelSize(14);
    a.setFont(f);
    QElapsedTimer cost;
    bool runRibbonApp = false;
    if (runRibbonApp)
    {
        // run SARibbonMainWindow MainWindow
        cost.start();
        MainWindow w;
        qDebug() << "window build cost:" << cost.elapsed() << " ms";
        w.show();
        return (a.exec());
    }
    else
    {
        // run PDF viewer MainWindow
        QMainWindow window;
        auto *viewer = new PdfViewer(&window);
        window.setCentralWidget(viewer);

        // ── Toolbar ──────────────────────────────────────────────────────────────
        auto *toolbar = window.addToolBar("Controls");

        auto *openAct = toolbar->addAction("Open PDF");
        QObject::connect(openAct, &QAction::triggered, [&]
                         {
        QString path = QFileDialog::getOpenFileName(
            &window, "Open PDF", {}, "PDF Files (*.pdf)");
        if (!path.isEmpty()) viewer->loadPdf(path);
        qDebug() << "Selected file:" << path; });

        auto *debugAct = toolbar->addAction("Debug Info");
        QObject::connect(debugAct, &QAction::triggered, [&]
                         { QMessageBox::information(&window, "Debug Info", viewer->debugMsg); });
        // toolbar->addWidget(debugAct);
        // QObject::connect(debugButton, &QAction::triggered, [viewer]()
        //                  { QMessageBox::information(&window, "Debug Info", "Testing message"); }); // viewer->debugMsg

        auto *zoomresetAct = toolbar->addAction("Reset Zoom");
        QObject::connect(zoomresetAct, &QAction::triggered, [&]()
                         { viewer->setZoom(1.0); });
        toolbar->addSeparator();
        toolbar->addAction("◀", viewer, &PdfViewer::prevPage);
        toolbar->addAction("▶", viewer, &PdfViewer::nextPage);

        toolbar->addSeparator();
        toolbar->addWidget(new QLabel("Zoom:"));

        auto *zoomSlider = new QSlider(Qt::Horizontal);
        zoomSlider->setRange(10, 400); // 10% … 400%
        zoomSlider->setValue(100);
        zoomSlider->setFixedWidth(160);
        QObject::connect(zoomSlider, &QSlider::valueChanged,
                         [viewer](int v)
                         { viewer->setZoom(v / 100.0); });
        toolbar->addWidget(zoomSlider);

        // embed debug message

        auto *spinBox = new QSpinBox();
        auto *labelDpi = new QLabel();
        toolbar->addWidget(spinBox);
        toolbar->addWidget(labelDpi);
        spinBox->setRange(0, 100);
        spinBox->setValue(50);
        spinBox->setFixedWidth(200);
        QObject::connect(spinBox, qOverload<int>(&QSpinBox::valueChanged),
                         [viewer](int v)
                         {
                             viewer->setZoom(v / 100.0);
                         });

        window.resize(900, 700);
        window.setWindowTitle("Qt PDFium Viewer");
        window.show();
        return (a.exec());
        // end here
    }
    // qDebug() << "stdout test";
}
