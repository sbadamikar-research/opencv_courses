// Face-Annotation-Assignment
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

struct Annotations {
    bool flag_drawing_box = false;
    cv::Mat working_image;
    cv::Point2i last_button_down{-1, -1};
    std::vector<cv::Rect2i> boxes{};
};

void cbMouseEvent(int event, int x, int y, int flags, void* param) {
    Annotations* rundata = static_cast<Annotations*>(param);

    if (!(rundata->flag_drawing_box) && (event == cv::EVENT_LBUTTONDOWN)) {
        if (rundata->last_button_down != cv::Point2i(-1, -1)) {
            std::cout << "[WARN] Point not reset correctly." << std::endl;
        }

        rundata->flag_drawing_box = true;
        rundata->last_button_down = cv::Point2i(x, y);
        return;
    }

    if ((event == cv::EVENT_LBUTTONUP) && (rundata->flag_drawing_box)) {
        rundata->flag_drawing_box = false;
        cv::Point2i pt1 = {std::min(x, rundata->last_button_down.x), std::min(y, rundata->last_button_down.y)};

        cv::Point2i pt2 = {std::max(x, rundata->last_button_down.x), std::max(y, rundata->last_button_down.y)};

        cv::Rect2i bbox = {pt1.x, pt1.y, (pt2.x - pt1.x), (pt2.y - pt1.y)};

        rundata->boxes.push_back(bbox);

        int line_thickness = int(0.005 * rundata->working_image.cols);
        cv::rectangle(rundata->working_image, bbox, cv::Scalar(0, 255, 0), line_thickness, cv::LINE_4);

        rundata->last_button_down = cv::Point2i(-1, -1);
    }
}

void resetWindow(cv::Mat* src, Annotations* data) {
    std::cout << "[INFO] Resetting the UI." << std::endl;
    
    int banner_height = int(0.1 * (src->rows));
    int font_size = banner_height / 40;
    int font_face = cv::FONT_HERSHEY_PLAIN;

    std::string instructions_l1 = "Left-click and drag to select a face.";
    std::string instructions_l2 = "Hit 'R' to reset, 'Q' to quit, 'Enter' to complete selections.";
    cv::Point2i pos_line1 = {int(0.2 * banner_height), int(0.4 * banner_height)};
    cv::Point2i pos_line2 = {int(0.2 * banner_height), int(0.9 * banner_height)};

    // cv::Size banner_size = {banner_height, src->cols}
    cv::Mat banner(banner_height, src->cols, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::vconcat(banner, *src, data->working_image);

    cv::putText(data->working_image, instructions_l1, pos_line1, font_face, font_size, cv::Scalar(255, 255, 255),
                font_size, cv::LINE_AA);

    cv::putText(data->working_image, instructions_l2, pos_line2, font_face, font_size, cv::Scalar(255, 255, 255),
                font_size, cv::LINE_AA);
}

void processBoxes(cv::Mat* src, Annotations* data) {
    std::cout << "[INFO] Input completed. Processing boxes." << std::endl;
    int i = 0;
    for (auto box : data->boxes) {
        std::string filename = "../outputs/face_" + std::to_string(i) + ".jpeg";
        cv::imwrite(filename, data->working_image(box));
        i++;
    }
}

int main() {
    std::string user_interface = "face_annotator";

    Annotations active_annotations;
    cv::Mat source = cv::imread("../data/images/pexels-hillaryfox-1595391.jpg");

    resetWindow(&source, &active_annotations);

    cv::namedWindow(user_interface, cv::WINDOW_NORMAL);
    cv::setMouseCallback(user_interface, cbMouseEvent, &active_annotations);

    while (true) {
        cv::imshow(user_interface, active_annotations.working_image);

        int keypress = cv::waitKey(1);

        if ((keypress == int('R')) || (keypress == int('r'))) {
            resetWindow(&source, &active_annotations);
            active_annotations.boxes.clear();
            active_annotations.flag_drawing_box = false;
            active_annotations.last_button_down = cv::Point2i(-1, -1);
        } else if ((keypress == int('Q')) || (keypress == int('q'))) {
            break;
        } else if (keypress == 13) {
            processBoxes(&source, &active_annotations);
            break;
        } else if (keypress == -1) {
            continue;
        } else {
            std::cout << "[ERROR] Bad keypress. Please use 'R', 'r', 'Q', 'q' or 'Enter'" << std::endl;
        }
    }

    cv::destroyAllWindows();

    return 0;
}