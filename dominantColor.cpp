#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;

enum Color {
    RED = 0,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    GRAY,
    BLACK,
    WHITE 
}; 

const char* colorName[9] = {
    "red",
    "yellow",
    "green",
    "cyan",
    "blue",
    "magenta",
    "gray",
    "black",
    "white"
};



// Get color name from hls color space
// @param: hls      hue, lightness, saturation
// !!!Note: 0 <= h <= 180, 0 <= s <= 255, 0 <= l <= 255
Color color_name_from_hls(const cv::Vec3b& hls) {
    int hue = hls.val[0] * 2;
    float lightness = hls.val[1] * 1.0 / 255;
    float sat = hls.val[2] * 1.0 / 255;

    if (lightness <= 0.1) return BLACK;
    if (lightness >= 0.9) return WHITE;
    if (sat <= 0.1) return GRAY;

    if (hue >= 21 && hue <= 70)
        return YELLOW;
    else if (hue >= 71 && hue <= 155)
        return GREEN;
    else if (hue >= 156 && hue <= 210)
        return CYAN;
    else if (hue >= 211 && hue <= 270)
        return BLUE;
    else if (hue >=271 && hue <= 320)
        return MAGENTA;
    else 
        return RED;
}


Color dominant_color_from_hls(const cv::Mat &hlsImg, const cv::Rect &roi) {
    vector<int> bins(9, 0);
    const cv::Mat srcROI(hlsImg, roi);

    for (int i = 0; i < srcROI.rows; i++) {
        for (int j = 0; j < srcROI.cols; j++) {
            cv::Vec3b hls_color = srcROI.at<cv::Vec3b>(i, j);
            Color color = color_name_from_hls(hls_color);
            bins[color]++;
        }
    }

    int maxValue = 0;
    Color dominant = WHITE; // default value
    for (int i = 0; i < 9; i++) {
        if (maxValue < bins[i]) 
        {
            maxValue = bins[i];
            dominant = static_cast<Color>(i);
        }
    }
    return dominant;
}

Color dominant_color(const cv::Mat &bgrImg, const cv::Rect &roi) {
    cv::Mat hlsImg;
    cv::cvtColor(bgrImg, hlsImg, CV_BGR2HLS);
    return dominant_color_from_hls(hlsImg, roi);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: ./dominantColor <imgFile>" << endl;
        return -1;
    }

    cv::Mat img;
    img = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if (!img.data) {
        cout << "Could not open or find the image!" << endl;
        return -1;
    }

    Color color = dominant_color(img, cv::Rect(0, 0, img.cols, img.rows));
    cout << "Dominant color: " << colorName[color] << endl;

    return 0;
}
