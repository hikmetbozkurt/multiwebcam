#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::string placeholder_image_path = "/Users/hikmetaydogan/camera_0_frame_1724103857.jpg";  // Change the path your dir
    cv::Mat placeholder_image = cv::imread(placeholder_image_path);

    if (placeholder_image.empty()) {
        throw std::runtime_error("Image not found at path: " + placeholder_image_path);
    }

    // Capture video
    cv::VideoCapture video_capture_0(0);
    cv::VideoCapture video_capture_1(1);
    cv::VideoCapture video_capture_2(2);

    int frame_width = static_cast<int>(video_capture_0.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(video_capture_0.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::resize(placeholder_image, placeholder_image, cv::Size(frame_width, frame_height));

    while (true) {
        cv::Mat frame0, frame1, frame2;
        bool ret0 = video_capture_0.read(frame0);
        bool ret1 = video_capture_1.read(frame1);
        bool ret2 = video_capture_2.read(frame2);

        if (!ret0) {
            frame0 = placeholder_image;
        }
        if (!ret1) {
            frame1 = placeholder_image;
        } else {
            cv::resize(frame1, frame1, cv::Size(frame0.cols, frame0.rows));
        }
        if (!ret2) {
            frame2 = placeholder_image;
        } else {
            cv::resize(frame2, frame2, cv::Size(frame0.cols, frame0.rows));
        }

        cv::Mat top_row, bottom_row, combined_frame;
        cv::hconcat(frame0, frame1, top_row);
        cv::hconcat(frame2, placeholder_image, bottom_row);
        cv::vconcat(top_row, bottom_row, combined_frame);

        // Display part
        cv::imshow("Multi-Camera View", combined_frame);

        // q for exit
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    video_capture_0.release();
    video_capture_1.release();
    video_capture_2.release();
    cv::destroyAllWindows();

    return 0;
}