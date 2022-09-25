#include "Image.h"

Image::Image(const std::string input_, const int blur_, const int kernel_, const int time_, const int lowValue_, const int highValue_) :
                                                                                                                        input(input_),
                                                                                                                        blur(blur_),
                                                                                                                        kernel(kernel_),
                                                                                                                        time(time_),
                                                                                                                        lowValue(lowValue_),
                                                                                                                        highValue(highValue_) {}

void Image::init(){
    img = cv::imread(input, 0);

    if(!img.empty()){
        if(blur != 0)
            cv::GaussianBlur(img, img, cv::Size(blur, blur), 0);

        size = img.size();
        choosekernel();
        normalize(kernel33);
    }else{
        std::cout<<"Image could not be found."<<std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Image::choosekernel() {
    switch(kernel){
        case 0 :
            applyKernel(prewitt);
            break;
        case 1 :
            applyKernel(sobel);
            break;
        case 2 :
            applyKernel(kirsch);
            break;
    }
}

void Image::applyKernel(float in[3][3]) {
    for(int i=0;i<3;++i){
        for(int j=0;j<3;++j){
            kernel33[j][i] = in[j][i];
        }
    }
}

void Image::normalize(float kernel[3][3]){
    int totalSum = 0;
    int positiveSum = 0;

    for(int i=0;i<kernelSize;++i){
        for(int j=0;j<kernelSize;++j){
            int val = kernel[i][j];
            totalSum += val;
            if(val > 0)
                positiveSum += val;
        }
    }

    if(totalSum == 0){
        for(int i=0;i<kernelSize;++i){
            for(int j=0;j<kernelSize;++j){
                kernel[i][j] = kernel[i][j]/positiveSum;
            }
        }
        std::cout<<"Normalized convolution kernel."<<std::endl;
    }else{
        std::cout<<"Sum of all kernel values not equal to zero."<<std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Image::applyConvolutionKernel_bi(const cv::Mat image) {
    outputImg = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));
    output_bi = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));

    directions_bi = new int * [size.height];

    int ** outputConvolution_X;
    outputConvolution_X = new int * [size.height];

    int ** outputConvolution_Y;
    outputConvolution_Y = new int * [size.height];

    for(int i=0;i<size.height;++i){
        directions_bi[i] = new int[size.width];
        std::memset(directions_bi[i], 0, size.width * sizeof(int));

        outputConvolution_X[i] = new int[size.width];
        std::memset(outputConvolution_X[i], 0, size.width * sizeof(int));

        outputConvolution_Y[i] = new int[size.width];
        std::memset(outputConvolution_Y[i], 0, size.width * sizeof(int));
    }

    outputConvolution_X = soloConvolution(img);
    rotate90();
    normalize(kernel33);
    outputConvolution_Y = soloConvolution(img);

    for(int i=1;i<size.width-1;++i){
        for(int j=1;j<size.height-1;++j){
            if(std::abs(outputConvolution_X[j][i]) > std::abs(outputConvolution_Y[j][i]))
                directions_bi[j][i] = 0;
            else
                directions_bi[j][i] = 1;
            int value = std::sqrt(outputConvolution_X[j][i] * outputConvolution_X[j][i] + outputConvolution_Y[j][i] * outputConvolution_Y[j][i]);
            output_bi.at<uchar>(j,i) = value;
        }
    }

    cv::imshow("Bi-directional", output_bi);
    cv::waitKey(time);
    cv::imwrite("../img/bi.png", output_bi);

    output_bi.copyTo(outputImg);
}

void Image::applyConvolutionKernel_multi(const cv::Mat image){
    outputImg = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));
    output_multi = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));

    directions = new int * [size.height];

    int ** outputConvolution_0;
    outputConvolution_0 = new int * [size.height];

    int ** outputConvolution_45;
    outputConvolution_45 = new int * [size.height];

    int ** outputConvolution_90;
    outputConvolution_90 = new int * [size.height];

    int ** outputConvolution_135;
    outputConvolution_135 = new int * [size.height];

    for(int i=0;i<size.height;++i){
        directions[i] = new int[size.width];
        std::memset(directions[i], 0, size.width * sizeof(int));

        outputConvolution_0[i] = new int[size.width];
        std::memset(outputConvolution_0[i], 0, size.width * sizeof(int));

        outputConvolution_45[i] = new int[size.width];
        std::memset(outputConvolution_45[i], 0, size.width * sizeof(int));

        outputConvolution_90[i] = new int[size.width];
        std::memset(outputConvolution_90[i], 0, size.width * sizeof(int));

        outputConvolution_135[i] = new int[size.width];
        std::memset(outputConvolution_135[i], 0, size.width * sizeof(int));
    }

    outputConvolution_0 = soloConvolution(img);
    rotate45(1);
    normalize(kernel33);
    outputConvolution_45 = soloConvolution(img);
    rotate45(2);
    normalize(kernel33);
    outputConvolution_90 = soloConvolution(img);
    rotate45(3);
    normalize(kernel33);
    outputConvolution_135 = soloConvolution(img);

    for(int i=1;i<size.width-1;++i){
        for(int j=1;j<size.height-1;++j){
            int value = std::max(outputConvolution_0[j][i], std::max(outputConvolution_45[j][i], std::max(outputConvolution_90[j][i], outputConvolution_135[j][i])));
            //direction begin
            if(value == outputConvolution_0[j][i])
                directions[j][i] = 1;
            if(value == outputConvolution_45[j][i])
                directions[j][i] = 2;
            if(value == outputConvolution_90[j][i])
                directions[j][i] = 3;
            if(value == outputConvolution_135[j][i])
                directions[j][i] = 4;
            //direction end
            if(value < 0)
                value = 0;
            if(value > 255)
                value = 255;
            output_multi.at<uchar>(j,i) = value;
        }
    }

    cv::imshow("Multi-directional", output_multi);
    cv::waitKey(time);
    cv::imwrite("../img/multi.png", output_multi);

    output_multi.copyTo(outputImg);
}

int ** Image::soloConvolution(const cv::Mat) {
    int ** outputConvolution;
    outputConvolution = new int * [size.height];

    for(int i=0;i<size.height;++i){
        outputConvolution[i] = new int[size.width];
        std::memset(outputConvolution[i], 0, size.width * sizeof(int));
    }

    for(int i=1;i<size.width-1;++i){
        for(int j=1;j<size.height-1;++j){
            float value = 0;
            for(int w=-1;w<2;++w){
                for(int z=-1;z<2;++z){
                    float val = img.at<uchar>(j+w,i+z) * kernel33[1+w][1+z];
                    value += val;
                }
            }
            float tmp = std::round(std::abs(value));
            outputConvolution[j][i] = tmp;
        }
    }
    return outputConvolution;
}

void Image::makeHistogram() {
    histogram = new int[256];
    std::memset(histogram, 0, 256 * sizeof(int));

    for(int i=0;i<size.width;++i) {
        for (int j=0; j < size.height; ++j) {
            uchar value = outputImg.at<uchar>(j, i);
            histogram[value] = histogram[value] + 1;
        }
    }

    onePercent = (size.height * size.width) / 100;
}

void Image::colorMulti() {
    colorImage_multi = cv::Mat(size.height, size.width, CV_8UC3, cv::Scalar(0,0,0));

    for(int i=1;i<size.width-1;++i) {
        for (int j = 1; j < size.height - 1; ++j) {
            uchar value = output_simpleThreshold.at<uchar>(j, i);
            if (value == 255) {
                cv::Vec3b c = colorImage_multi.at<cv::Vec3b>(j, i);
                switch (directions[j][i]) {
                    case 1 :
                        c[0] = 255; c[1] = 0; c[2] = 0;
                        break;
                    case 2 :
                        c[0] = 0; c[1] = 255; c[2] = 0;
                        break;
                    case 3 :
                        c[0] = 0; c[1] = 0; c[2] = 255;
                        break;
                    case 4 :
                        c[0] = 255; c[1] = 0; c[2] = 255;
                        break;
                }
                colorImage_multi.at<cv::Vec3b>(j, i) = c;
            }
        }
    }

    cv::imshow("Colored image multi", colorImage_multi);
    cv::waitKey(time);
    cv::imwrite("../img/color_multi.png", colorImage_multi);
}

void Image::colorBi() {
    colorImage_bi = cv::Mat(size.height, size.width, CV_8UC3, cv::Scalar(0, 0, 0));

    for (int i=1;i<size.width-1;++i) {
        for (int j=1;j<size.height-1;++j) {
            uchar value = output_simpleThreshold.at<uchar>(j, i);
            if (value == 255) {
                cv::Vec3b c = colorImage_bi.at<cv::Vec3b>(j, i);
                if (directions_bi[j][i] == 0) {
                    c[0] = 255; c[1] = 0; c[2] = 0;
                }else{
                    c[0] = 0; c[1] = 255; c[2] = 0;
                }
                colorImage_bi.at<cv::Vec3b>(j, i) = c;
            }
        }
    }

    cv::imshow("Color image bi", colorImage_bi);
    cv::waitKey(time);
    cv::imwrite("../img/color_bi.png", colorImage_bi);
}

void Image::getSeuilHaut(int threshold) {
    std::cout<<"Number of pixel in one(1) percent in image : "<<onePercent<<std::endl;
    threshold = threshold * onePercent;
    std::cout<<"Number of pixel to darken : "<<threshold<<std::endl;

    int sum = 0;
    for(int i=0;i<256;++i){
        sum += histogram[i];
        if(threshold < sum){
            threshold = i;
            break;
        }
    }

    std::cout<<"High cut off value : "<<threshold<<std::endl;
    seuilHaut = threshold;
}

void Image::simple(){
    output_simpleThreshold = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));
    for(int i=1;i<size.width-1;++i) {
        for (int j = 1; j < size.height - 1; ++j) {
            uchar value = outputImg.at<uchar>(j, i);
            if(value < seuilHaut)
                output_simpleThreshold.at<uchar>(j, i) = 0;
            else
                output_simpleThreshold.at<uchar>(j, i) = 255;
        }
    }

    cv:imshow("Simple threshold", output_simpleThreshold);
    cv::waitKey(time);
    cv::imwrite("../img/simpleThreshold.png", output_simpleThreshold);
}

void Image::hysterisis() {
    output_hysterisis = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));
    seuilBas = std::round(lowValue * seuilHaut);
    std::cout<<"Low cut off value : "<< seuilBas<<std::endl;

    for(int i=1;i<size.width-1;++i) {
        for (int j = 1; j < size.height - 1; ++j) {
            uchar value = outputImg.at<uchar>(j, i);
            //Seuil bas
            if(value < seuilBas)
                output_hysterisis.at<uchar>(j, i) = 0;
            //Seuil milieu
            if(value >= seuilBas && value <= seuilHaut){
                bool higher = false;
                for(int w=-1;w<2;++w){
                    for(int z=-1;z<2;++z){
                        uchar conn8 = outputImg.at<uchar>(j+w,i+z);
                        if(conn8 > seuilHaut) // value
                            higher = true;
                    }
                }
                if(higher) {
                    output_hysterisis.at<uchar>(j, i) = 255;
                }else{
                    output_hysterisis.at<uchar>(j, i) = 0;
                }
            }
            //Seuil haut
            if(value > seuilHaut)
                output_hysterisis.at<uchar>(j, i) = 255;
        }
    }

    cv:imshow("Hysterisis threshold", output_hysterisis);
    cv::waitKey(time);
    cv::imwrite("../img/hysterisisThreshold.png", output_hysterisis);
}


void Image::slim_bi() {
    cv::Mat test  = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));
    for(int i=1;i<size.width-1;++i) {
        for(int j=1;j<size.height-1;++j) {
            uchar value = outputImg.at<uchar>(j, i);
            uchar valueHY = output_hysterisis.at<uchar>(j, i);
            if(valueHY == 255){
                switch (directions_bi[j][i]) {
                    case 0 :
                        if (outputImg.at<uchar>(j, i-1) <= value && value >= outputImg.at<uchar>(j, i+1))
                            test.at<uchar>(j, i) = 255;
                        else
                            test.at<uchar>(j, i) = 0;
                        break;
                    case 1 :
                        if (outputImg.at<uchar>(j-1, i) <= value && value >= outputImg.at<uchar>(j+1, i))
                            test.at<uchar>(j, i) = 255;
                        else
                            test.at<uchar>(j, i) = 0;
                        break;
                }
            }
        }
    }

    cv:imshow("Bi-directionnal", test);
    cv::waitKey(time);
    cv::imwrite("../img/affinage_bi.png", test);
}

void Image::slim_multi() {
    cv::Mat test  = cv::Mat(size.height, size.width, CV_8UC1, cv::Scalar(0));
    for(int i=1;i<size.width-1;++i) {
        for(int j=1;j<size.height-1;++j) {
            uchar value = outputImg.at<uchar>(j, i);
            uchar valueHY = output_hysterisis.at<uchar>(j, i);
            if(valueHY == 255){
                switch (directions[j][i]) {
                    case 1 :
                        if (outputImg.at<uchar>(j, i-1) <= value && value >= outputImg.at<uchar>(j, i+1))
                            test.at<uchar>(j, i) = 255;
                        else
                            test.at<uchar>(j, i) = 0;
                        break;
                    case 2 :
                        if (outputImg.at<uchar>(j-1, i+1) <= value && value >= outputImg.at<uchar>(j+1, i-1))
                            test.at<uchar>(j, i) = 255;
                        else
                            test.at<uchar>(j, i) = 0;
                        break;
                    case 3 :
                        if (outputImg.at<uchar>(j-1, i) <= value && value >= outputImg.at<uchar>(j+1, i))
                            test.at<uchar>(j, i) = 255;
                        else
                            test.at<uchar>(j, i) = 0;
                        break;
                    case 4 :
                        if (outputImg.at<uchar>(j-1, i-1) <= value && value >= outputImg.at<uchar>(j+1, i+1))
                            test.at<uchar>(j, i) = 255;
                        else
                            test.at<uchar>(j, i) = 0;
                        break;
                }
            }
        }
    }

    cv:imshow("Multi-directionnal", test);
    cv::waitKey(time);
    cv::imwrite("../img/affinage_multi.png", test);
}

void Image::rotate90(){
// Swapping elements after each iteration in Clockwise direction
    for (int i=0;i<3/2;i++){
        for (int j=i;j<3-i-1;j++){
            float temp=kernel33[i][j];
            kernel33[i][j] = kernel33[3-1-j][i];
            kernel33[3-1-j][i] = kernel33[3-1-i][3-1-j];
            kernel33[3-1-i][3-1-j] = kernel33[j][3-1-i];
            kernel33[j][3-1-i]=temp;
        }
    }
}

//I'm not proud...But it works
void Image::rotate45(int index){
    for(int i=0;i<3;++i){
        for(int j=0;j<3;++j){
            switch(kernel){
                case 0 :
                    switch(index){
                        case 1:
                            kernel33[i][j] = prewitt_45[i][j];
                            break;
                        case 2 :
                            kernel33[i][j] = prewitt_90[i][j];
                            break;
                        case 3:
                            kernel33[i][j] = prewitt_135[i][j];
                            break;
                    }
                    break;
                case 1 :
                    switch(index){
                        case 1:
                            kernel33[i][j] = sobel_45[i][j];
                            break;
                        case 2 :
                            kernel33[i][j] = sobel_90[i][j];
                            break;
                        case 3:
                            kernel33[i][j] = sobel_135[i][j];
                            break;
                    }
                    break;
                case 2 :
                    switch(index){
                        case 1:
                            kernel33[i][j] = kirsch_45[i][j];
                            break;
                        case 2 :
                            kernel33[i][j] = kirsch_90[i][j];
                            break;
                        case 3:
                            kernel33[i][j] = kirsch_135[i][j];
                            break;
                    }
                    break;
            }
        }
    }
}

void Image::execute_bi() {
    applyConvolutionKernel_bi(img);
    makeHistogram();
    getSeuilHaut(highValue);
    simple();
    hysterisis();
    colorBi();
    slim_bi();
}

void Image::execute_multi() {
    applyConvolutionKernel_multi(img);
    makeHistogram();
    getSeuilHaut(highValue);
    simple();
    hysterisis();
    colorMulti();
    slim_multi();
}