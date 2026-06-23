#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QWheelEvent>
#include <vector>
#include <fpdfview.h> // PDFium header
#include <fpdf_doc.h>

class PdfViewer : public QGraphicsView
{
    Q_OBJECT

public:
    explicit PdfViewer(QWidget *parent = nullptr);
    ~PdfViewer();

    bool loadPdf(const QString &filePath);
    void showPage(int pageIndex);
    int pageCount() const { return m_pageCount; }

public slots:
    void nextPage();
    void prevPage();
    void setZoom(double factor);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPixmap renderPage(int pageIndex);

    QGraphicsScene *m_scene = nullptr;
    FPDF_DOCUMENT m_document = nullptr;
    int m_pageCount = 0;
    int m_currentPage = 0;
    double m_zoomFactor = 1.0;
    int m_dpi = 150; // rendering DPI

public:
    QString debugMsg;
};