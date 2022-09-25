#ifndef ANALYSE_TP2_IMAGE_H
#define ANALYSE_TP2_IMAGE_H

#include <cmath>
#include <string>
#include <iostream>

#include <omp.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Image {
public :
    Image(const std::string, const int, const int, const int, const int, const int);
    void init();
    void normalize(float kernel[3][3]);
    void applyConvolutionKernel_bi(const cv::Mat);
    void applyConvolutionKernel_multi(const cv::Mat);
    int ** soloConvolution(const cv::Mat);
    void execute_bi();
    void execute_multi();
    void rotate90();
    void rotate45(int index);

    void choosekernel();
    void applyKernel(float in[3][3]);

    void colorBi();
    void colorMulti();

    //seuillage
    void getSeuilHaut(int threshold);
    void simple();
    void hysterisis();

    void makeHistogram();

    void slim_bi();
    void slim_multi();

    void write();
    void clean();

private :
    int blur, kernel;
    std::string input;

    cv::Mat img, colorImage_multi, colorImage_bi, middle, outputImg;
    cv::Size size;

    cv::Mat output_bi, output_multi;
    cv::Mat output_simpleThreshold, output_hysterisis;

    char ** array_bi_X, array_bi_Y;
    int ** directions;
    int ** directions_bi;

    int kernelSize = 3;
    int * histogram;
    int onePercent;
    int seuilHaut, seuilBas;

    int time;
    float kernel33[3][3];

    int lowValue, highValue;

    //prewitt
    float prewitt[3][3] = {{-1,0,1}, {-1,0,1}, {-1,0,1}};
    float prewitt_45[3][3] = {{0,1,1}, {-1,0,1}, {-1,-1,0}};
    float prewitt_90[3][3] = {{1,1,1}, {0,0,0}, {-1,-1,-1}};
    float prewitt_135[3][3] = {{1,1,0}, {1,0,-1}, {0,-1,-1}};
    //sobel
    float sobel[3][3] = {{-1,0,1}, {-2,0,2}, {-1, 0,1}};
    float sobel_45[3][3] = {{0,1,2}, {-1,0,1}, {-2,-1,0}};
    float sobel_90[3][3] = {{1,2,1}, {0,0,0}, {-1,-2,-1}};
    float sobel_135[3][3] = {{2,1,0}, {1,0,-1}, {0,-1,-2}};
    //kirsch
    float kirsch[3][3] = {{-3, -3, 5}, {-3, 0, 5}, {-3, -3, 5}};
    float kirsch_45[3][3] = {{-3,5, 5}, {-3,0,5}, {-3,-3,-3}};
    float kirsch_90[3][3] = {{1,1,1}, {0,0,0}, {-1,-1,-1}};
    float kirsch_135[3][3] = {{1,1,0}, {1,0,-1}, {0,-1,-1}};

};

#endif //ANALYSE_TP2_IMAGE_H
