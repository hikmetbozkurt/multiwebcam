#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

const char* image_path = "/home/pi/Downloads/new.jpeg"; // Update with your path
Mat static_image;

void initializeStaticImage() {
    // Load the static image
    static_image = imread(image_path);
    if (static_image.empty()) {
        cerr << "Error loading static image from " << image_path << endl;
        exit(-1);
    }

    // Resize the static image to match the frame size
    int desired_width = 256;  // Match the desired width of frames
    int desired_height = 144;  // Match the desired height of frames
    resize(static_image, static_image, Size(desired_width, desired_height));
}

void showFrames(const Mat& frame1, const Mat& frame2, const Mat& frame3, VideoWriter& video_writer) {
    // Ensure all frames are resized to the target size
    int target_width = 256;
    int target_height = 144;
    Mat resized_frame1, resized_frame2, resized_frame3;
    Mat resized_static_image;

    if (!frame1.empty()) {
        resize(frame1, resized_frame1, Size(target_width, target_height));
    } else {
        resized_frame1 = static_image.clone(); // Use static image if frame is empty
    }

    if (!frame2.empty()) {
        resize(frame2, resized_frame2, Size(target_width, target_height));
    } else {
        resized_frame2 = static_image.clone(); // Use static image if frame is empty
    }

    if (!frame3.empty()) {
        resize(frame3, resized_frame3, Size(target_width, target_height));
    } else {
        resized_frame3 = static_image.clone(); // Use static image if frame is empty
    }

    // Resize static image to match the height of the frames
    resize(static_image, resized_static_image, Size(target_width, target_height));

    // Concatenate frames
    Mat top_row, bottom_row, combined_frame;

    // Horizontal concatenation for the top row
    hconcat(resized_frame1, resized_frame2, top_row);

    // Horizontal concatenation for the bottom row
    hconcat(resized_frame3, resized_static_image, bottom_row);

    // Vertical concatenation for the final output
    vconcat(top_row, bottom_row, combined_frame);

    // Show combined frame
    imshow("Multi-Camera Display", combined_frame);

    // Write combined frame to video
    video_writer.write(combined_frame);
}

int main() {
    initializeStaticImage();

    VideoCapture cap1("/dev/video0"); // Change to your camera indices
    VideoCapture cap2("/dev/video4");
    VideoCapture cap3("/dev/video2");

    // Check if cameras are opened
    bool cap1_opened = cap1.isOpened();
    bool cap2_opened = cap2.isOpened();
    bool cap3_opened = cap3.isOpened();

    if (!cap1_opened) {
        cerr << "Error opening camera 0" << endl;
    }
    if (!cap2_opened) {
        cerr << "Error opening camera 1" << endl;
    }
    if (!cap3_opened) {
        cerr << "Error opening camera 2" << endl;
    }

    // Set frame size to 128x72
    int frame_width = 256;
    int frame_height = 144;

    if (cap1_opened) {
        cap1.set(CAP_PROP_FRAME_WIDTH, frame_width);
        cap1.set(CAP_PROP_FRAME_HEIGHT, frame_height);
    }
    if (cap2_opened) {
        cap2.set(CAP_PROP_FRAME_WIDTH, frame_width);
        cap2.set(CAP_PROP_FRAME_HEIGHT, frame_height);
    }
    if (cap3_opened) {
        cap3.set(CAP_PROP_FRAME_WIDTH, frame_width);
        cap3.set(CAP_PROP_FRAME_HEIGHT, frame_height);
    }

    // Set up VideoWriter to save the combined video
    string video_filename = "/home/pi/Desktop/multiwebcam/cam/record.avi";
    VideoWriter video_writer(video_filename, VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, Size(2 * frame_width, 2 * frame_height));
    if (!video_writer.isOpened()) {
        cerr << "Error opening video writer: " << video_filename << endl;
        return -1;
    }

    while (true) {
        Mat frm1, frm2, frm3;

        if (cap1_opened) {
            cap1 >> frm1;
        }
        if (cap2_opened) {
            cap2 >> frm2;
        }
        if (cap3_opened) {
            cap3 >> frm3;
        }

        showFrames(frm1, frm2, frm3, video_writer);

        // Exit on 'q' key press
        if (waitKey(1) == 'q') {
            break;
        }
    }

    cap1.release();
    cap2.release();
    cap3.release();
    video_writer.release();
    destroyAllWindows();

    return 0;
}
