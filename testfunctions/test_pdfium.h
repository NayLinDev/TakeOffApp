#include <podofo/podofo.h>
#include <iostream>

#include <fpdfview.h>

#include <vector>
#include <fstream>
#include <format>
#include <Qstring>
using namespace std;
QString test_pdfium()
{
    std::string return_string;
    /*
    try {
        // std::cout << "PoDoFo is working!\n";

        PoDoFo::PdfMemDocument doc;
        // std::cout << "PDF document object created.\n";
    }
    catch (const std::exception& e) {
        // std::cerr << "Error: " << e.what() << std::endl;
    }
    */

    // 1. Init PDFium
    FPDF_InitLibrary();

    // 2. Load PDF
    const char *filename = "..\\..\\data\\drawings\\drawing.pdf";

    FPDF_DOCUMENT doc = FPDF_LoadDocument(filename, nullptr);
    if (!doc)
    {
        // NL_edit
        // std::cerr << "Failed to load PDF\n";
        return "Failed to load PDF";
    }

    // 3. Load first page
    FPDF_PAGE page = FPDF_LoadPage(doc, 0);
    if (!page)
    {
        // std::cerr << "Failed to load page\n";
        return "Error: Failed to load page";
    }

    // 4. Get page size
    double width = FPDF_GetPageWidth(page);
    double height = FPDF_GetPageHeight(page);

    int w = static_cast<int>(width);
    int h = static_cast<int>(height);

    // NL_edit
    // std::cout << "Page size: " << w << " x " << h << "\n";

    // 5. Render settings
    FPDF_BITMAP bitmap = FPDFBitmap_Create(w, h, 0);

    // white background
    FPDFBitmap_FillRect(bitmap, 0, 0, w, h, 0xFFFFFFFF);

    // 6. Render page → bitmap
    FPDF_RenderPageBitmap(
        bitmap,
        page,
        0, 0,
        w, h,
        0,
        FPDF_REVERSE_BYTE_ORDER);

    // 7. Access raw pixels
    unsigned char *buffer = (unsigned char *)FPDFBitmap_GetBuffer(bitmap);
    int stride = FPDFBitmap_GetStride(bitmap);

    std::cout << "Rendered OK. Stride = " << stride << "\n";

    // 8. Save as raw BMP (simple test output)
    std::ofstream out("output.raw", std::ios::binary);
    out.write((char *)buffer, w * h * 4);
    out.close();

    // NL_edit
    // std::cout << "Saved output.raw (RGBA buffer)\n";

    // 9. Cleanup
    FPDFBitmap_Destroy(bitmap);
    FPDF_ClosePage(page);
    FPDF_CloseDocument(doc);
    FPDF_DestroyLibrary();
    QString return_str = QString("Page size: %1 x %2").arg(w).arg(h);
    return return_str;
}