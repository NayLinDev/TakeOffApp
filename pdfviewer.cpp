#include "pdfviewer.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>
#include <fpdfview.h>
// #include <fpdfdoc.h>

// PDFium must be initialized once per process
struct PdfiumInit
{
    PdfiumInit() { FPDF_InitLibrary(); }
    ~PdfiumInit() { FPDF_DestroyLibrary(); }
};

// ── Constructor / Destructor ──────────────────────────────────────────────────

PdfViewer::PdfViewer(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this))
{
    static PdfiumInit init; // init PDFium exactly once

    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

PdfViewer::~PdfViewer()
{
    if (m_document)
    {
        FPDF_CloseDocument(m_document);
    }
}

bool PdfViewer::loadPdf(const QString &filePath)
{
    if (m_document)
    {
        FPDF_CloseDocument(m_document);
        m_document = nullptr;
    }

    m_document = FPDF_LoadDocument(filePath.toUtf8().constData(), nullptr);
    if (!m_document)
    {
        unsigned long err = FPDF_GetLastError();
        qWarning() << "PDFium error loading file, code:" << err;
        return false;
    }

    m_pageCount = FPDF_GetPageCount(m_document);
    m_currentPage = 0;
    showPage(0);
    return true;
}

// ── Render a single page → QPixmap ───────────────────────────────────────────

QPixmap PdfViewer::renderPage(int pageIndex)
{
    if (!m_document || pageIndex < 0 || pageIndex >= m_pageCount)
        return {};

    FPDF_PAGE page = FPDF_LoadPage(m_document, pageIndex);
    if (!page)
        return {};

    // Get page dimensions in points (1 pt = 1/72 inch)
    double pageW = FPDF_GetPageWidth(page);
    double pageH = FPDF_GetPageHeight(page);
    debugMsg = QString("Rendering page %1/%2 (size: %3x%4 pt)- at DPI: %5, zoom: %6")
                   .arg(pageIndex + 1)
                   .arg(m_pageCount)
                   .arg(pageW)
                   .arg(pageH)
                   .arg(m_dpi)
                   .arg(m_zoomFactor);
    // Convert to pixels at target DPI, then apply zoom
    int pixelW = static_cast<int>(pageW / 72.0 * m_dpi * m_zoomFactor);
    int pixelH = static_cast<int>(pageH / 72.0 * m_dpi * m_zoomFactor);

    // Create PDFium bitmap  (BGRA format)
    int pixelW_halfed = pixelW / 2;
    int pixelH_halfed = pixelH / 2;
    FPDF_BITMAP bitmap = FPDFBitmap_Create(pixelW_halfed, pixelH_halfed, 1 /*hasAlpha*/);
    FPDFBitmap_FillRect(bitmap, 0, 0, pixelW_halfed, pixelH_halfed, 0xFFFFFFFF); // white bg

    FPDF_RenderPageBitmap(
        bitmap, page,
        0, 0, pixelW_halfed, pixelH_halfed,
        0,         // rotation (0=none, 1=90°, 2=180°, 3=270°)
        FPDF_ANNOT // render annotations
    );

    // Copy PDFium BGRA buffer → QImage
    void *buf = FPDFBitmap_GetBuffer(bitmap);
    int stride = FPDFBitmap_GetStride(bitmap);

    QImage image(
        static_cast<const uchar *>(buf),
        pixelW_halfed, pixelH_halfed,
        stride,
        QImage::Format_ARGB32 // Qt's ARGB == PDFium's BGRA on little-endian
    );

    // Must copy before releasing bitmap — QImage wraps the buffer by default
    QPixmap pixmap = QPixmap::fromImage(image.copy());

    FPDFBitmap_Destroy(bitmap);
    FPDF_ClosePage(page);
    return pixmap;
}

// ── Show page ────────────────────────────────────────────────────────────────

void PdfViewer::showPage(int pageIndex)
{
    if (pageIndex < 0 || pageIndex >= m_pageCount)
        return;

    m_currentPage = pageIndex;
    QPixmap pix = renderPage(pageIndex);

    m_scene->clear();
    m_scene->addPixmap(pix);
    m_scene->setSceneRect(pix.rect());
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

// ── Navigation / Zoom ────────────────────────────────────────────────────────

void PdfViewer::nextPage() { showPage(m_currentPage + 1); }
void PdfViewer::prevPage() { showPage(m_currentPage - 1); }

void PdfViewer::setZoom(double factor)
{
    m_zoomFactor = qBound(0.1, factor, 10.0);
    showPage(m_currentPage);
}

void PdfViewer::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        double delta = event->angleDelta().y() / 1200.0;
        setZoom(m_zoomFactor + delta);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void PdfViewer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Right:
    case Qt::Key_PageDown:
        nextPage();
        break;
    case Qt::Key_Left:
    case Qt::Key_PageUp:
        prevPage();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}