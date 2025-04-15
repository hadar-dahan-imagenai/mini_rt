//
// Created by hadard on 15/04/2025.
//
#include <iostream>
#include <curves.h>
#include <color.h>

void firstAnalysis(float *original_r,float *original_g,float *original_b, int width, int hight, LUTu & histogram)
{
    double lumimul[3];

    // TMatrix wprof = ICCStore::getInstance()->workingSpaceMatrix("ProPhoto");
    // TMatrix wprof = ICCStore::getInstance()->workingSpaceMatrix(params.icm.workingProfile);
    const double wprof[3][3] = {{0.79767489999999996, 0.1351917, 0.031353399999999997},
        {0.28804020000000002,0.71187409999999995,8.5699999999999996e-05 },
        {0,0,0.82521}}; //i took it from "ProPhoto"
    lumimul[0] = wprof[1][0];
    lumimul[1] = wprof[1][1];
    lumimul[2] = wprof[1][2];

    int W = width;
    int H = hight;
    int TS =  W;
    float lumimulf[3] = {static_cast<float>(lumimul[0]), static_cast<float>(lumimul[1]), static_cast<float>(lumimul[2])};

    // calculate histogram of the y channel needed for contrast curve calculation in exposure adjustments
    histogram.clear();

        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {

                float r = original_r[i * TS + j];
                float g = original_g[i * TS + j];
                float b = original_b[i * TS + j];

                int y = (lumimulf[0] * r + lumimulf[1] * g + lumimulf[2] * b);
                histogram[y]++;
            }
        }
}
void readData (FILE *f, float *r, float *g, float *b,  size_t totalElements)
{
    // read the `r` array from the file
    size_t elementsWritten = fread(r, sizeof(float), totalElements, f);
    if (elementsWritten < totalElements) {
        printf("error writing r! elementsWritten %d < totalElements %d\n", (int)elementsWritten, (int)totalElements);
    }

    // read the `g` array from the file
    elementsWritten = fread(g, sizeof(float), totalElements, f);
    if (elementsWritten < totalElements) {
        printf("error writing g! elementsWritten %d < totalElements %d\n", (int)elementsWritten, (int)totalElements);
    }

    // read the `b` array from the file
    elementsWritten = fread(b, sizeof(float), totalElements, f);
    if (elementsWritten < totalElements) {
        printf("error writing b! elementsWritten %d < totalElements %d\n", (int)elementsWritten, (int)totalElements);
    }
}

void writeData (FILE *f, float *r, float *g, float *b,  size_t totalElements)
{
    // Write the `r` array to the file
    size_t elementsWritten = fwrite(r, sizeof(float), totalElements, f);
    if (elementsWritten < totalElements) {
        printf("error writing r! elementsWritten %d < totalElements %d\n", (int)elementsWritten, (int)totalElements);
    }

    // Write the `g` array to the file
    elementsWritten = fwrite(g, sizeof(float), totalElements, f);
    if (elementsWritten < totalElements) {
        printf("error writing g! elementsWritten %d < totalElements %d\n", (int)elementsWritten, (int)totalElements);
    }

    // Write the `b` array to the file
    elementsWritten = fwrite(b, sizeof(float), totalElements, f);
    if (elementsWritten < totalElements) {
        printf("error writing b! elementsWritten %d < totalElements %d\n", (int)elementsWritten, (int)totalElements);
    }
}
int main() {
    FILE* file = fopen("..\\matrix_test_files\\before_contrast_test1250.bin", "rb");
    // FILE* file = fopen("C:\\Users\\hadard\\Desktop\\dev\\ImAgent2\\build\\before_contrast_test.bin", "rb");
    // FILE* file = fopen("C:\\Users\\hadard\\Desktop\\dev\\ImAgent\\build\\contrast_test.bin", "rb");
    size_t hight = 833;
    // size_t hight = 200;
    size_t width = 1250;
    // size_t width = 300;
    size_t totalElements = width * hight;
    std::vector<float> original_r(totalElements);
    std::vector<float> original_g(totalElements);
    std::vector<float> original_b(totalElements);
    for (int i=0;i<hight;i++) {
        readData(file, original_r.data() + i * width,original_g.data() + i * width, original_b.data() + i * width, width);
    }
    fclose(file);

    LUTf curve1;

    LUTf curve2;
    LUTf curve;
    LUTf satcurve;
    LUTf lhskcurve;
    LUTf lumacurve;
    LUTf clcurve;
    LUTf clToningcurve;
    LUTf cl2Toningcurve;
    LUTf wavclCurve;

    LUTf rCurve;
    LUTf gCurve;
    LUTf bCurve;
    LUTu dummy;
    LUTu hist16;
    hist16(65536);

    curve1(65536);
    curve2(65536);
    curve(65536, 0);
    satcurve(65536, 0);
    lhskcurve(65536, 0);
    lumacurve(32770, 0);  // lumacurve[32768] and lumacurve[32769] will be set to 32768 and 32769 later to allow linear interpolation
    clcurve(65536, 0);
    wavclCurve(65536, 0);
    rtengine::ToneCurve customToneCurve1, customToneCurve2;

    //if(params.blackwhite.enabled) params.toneCurve.hrenabled=false;
    // perform first analysis
    // hist16(65536); //me

    //ipf.firstAnalysis(baseImg, params, hist16);//me
    double expcomp = 0;
    double black = 0;
    double hlcompr = 0;
    double hlcomprthresh = 0;
    double shcompr = 50; //by default in pp3 file
    double br = 0;
    double contr = 80;
    std::vector<double> curvePoints;
    curvePoints.push_back(0);
    std::vector<double> curvePoints2;
    curvePoints2.push_back(0);

    std::cout << "Hello and welcome to clion " << "!\n";
    rtengine::Color::init ();
    firstAnalysis(original_r.data(),original_g.data(), original_b.data(),width,hight,hist16);
    rtengine::CurveFactory::complexCurve(expcomp, black / 65535.0, hlcompr, hlcomprthresh, shcompr, br, contr,
                                         curvePoints, curvePoints2,
                                         hist16, curve1, curve2, curve,
                                         dummy, customToneCurve1, customToneCurve2);

    int stride = width; // each row has 'width' pixels

    for (int row = 0; row < hight; row++) {
        for (int col = 0; col < width; col++) {
            int index = row * stride + col;

            // Apply brightness/contrast using tone curve - from rgbProc
            rtengine::setUnlessOOG(
                original_r.data()[index],
                original_g.data()[index],
                original_b.data()[index],
                curve[original_r.data()[index]],
                curve[original_g.data()[index]],
                curve[original_b.data()[index]]
            );
        }
    }

    FILE* file_after = fopen("C:\\Users\\hadard\\Desktop\\dev\\ImAgent2\\build\\after_contrast_test.bin", "wb");
    for (int i=0;i<hight;i++) {
        writeData(file_after, original_r.data() + i * width, original_g.data() + i * width,original_b.data() + i * width,width);
    }

    return 0;
}