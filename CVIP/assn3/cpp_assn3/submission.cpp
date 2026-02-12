// Face-Annotation-Assignment
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int max_scale = 100;

struct WorkData {
    cv::Mat source;
    cv::Mat output;
    bool scaling_down = false;

    std::string user_interface;
    std::string trackbar_scale_factor;
    std::string trackbar_scale_type;
};

void updateType(int val, void* userdata) {
    auto data = (WorkData*)userdata;
    data->scaling_down = bool(val);
    if (data->scaling_down) {
        std::cout << "[INFO] Scaling Down." << std::endl;
    } else {
        std::cout << "[INFO] Scaling Up." << std::endl;
    }
    cv::setTrackbarPos(data->trackbar_scale_factor, data->user_interface, 0);
}

void updateImage(int val, void* userdata) {
    auto data = (WorkData*)userdata;

    double scale = 0;
    if (data->scaling_down) {
        scale = 1.0 - (float(val) / (max_scale + 1));
    } else {
        scale = 1.0 + (float(val) / max_scale);
    }

    int inter_fn = (scale > 1) ? cv::INTER_LINEAR : cv::INTER_AREA;

    cv::resize(data->source, data->output, cv::Size(), scale, scale, inter_fn);

    if (data->scaling_down) {
        int padding_tb = int((data->source.rows - data->output.rows) / 2);
        int padding_lr = int((data->source.cols - data->output.cols) / 2);
        cv::copyMakeBorder(data->output,
                           data->output,
                           padding_tb,
                           padding_tb,
                           padding_lr,
                           padding_lr,
                           cv::BORDER_CONSTANT,
                           cv::Scalar(200, 200, 200));
    }
}

int main() {
    WorkData data;
    data.source = cv::imread("../data/images/pexels-hillaryfox-1595391.jpg");
    data.output = data.source;
    data.user_interface = "image_scalar";
    data.trackbar_scale_factor = "Scale Factor:";
    data.trackbar_scale_type = "Scaling Type\n0:Scale up\n1:Scale down\n";

    cv::namedWindow(data.user_interface, cv::WINDOW_AUTOSIZE);

    cv::createTrackbar(data.trackbar_scale_type,
                       data.user_interface,
                       NULL,
                       1,
                       updateType,
                       &data);
    cv::createTrackbar(data.trackbar_scale_factor,
                       data.user_interface,
                       NULL,
                       max_scale,
                       updateImage,
                       &data);

    while (true) {
        cv::imshow(data.user_interface, data.output);

        int keypress = cv::waitKey(1);

        if ((keypress == int('R')) || (keypress == int('r'))) {
            updateImage(0, &data);
            cv::setTrackbarPos(data.trackbar_scale_type,
                               data.user_interface,
                               0);
            cv::setTrackbarPos(data.trackbar_scale_factor,
                               data.user_interface,
                               0);
        } else if ((keypress == int('Q')) || (keypress == int('q'))) {
            break;
        } else if (keypress == -1) {
            continue;
        } else {
            std::cout << "[ERROR] Bad keypress. Please use 'R/r' to Reset or "
                         "'Q/q' to Quit"
                      << std::endl;
        }
    }

    cv::destroyAllWindows();

    return 0;
}