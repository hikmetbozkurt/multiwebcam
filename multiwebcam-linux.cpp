#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <opencv2/opencv.hpp>

using namespace cv;

// Path to the placeholder image
const char* image_path = "/home/pi/Downloads/photo1.jpeg";  // Change the path to your dir
Mat static_image = imread(image_path);
Mat static_image_resized;

Mat capture_frame(VideoCapture cam) {
    Mat frame;
    if (cam.read(frame)) {
        Mat resized_frame;
        resize(frame, resized_frame, Size(320, 240));
        return resized_frame;
    }
    else {
        return static_image_resized;
    }

}

int main() {
    static_image_resized = Mat();
    resize(static_image, static_image_resized, Size(320, 240));

    // Specify the camera paths statically
    unsigned int camera_paths[] = { 0, 1, 2 };
    VideoCapture cams[3];
    VideoWriter out_writers[3];

    // Initialize cameras and corresponding video writers
    for (int i = 0; i < 3; i++) {
        cams[i].open(camera_paths[i]);
        if (cams[i].isOpened()) {
            // Set the camera to use the MJPEG codec
            cams[i].set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));

            // Create a separate VideoWriter for each camera
            VideoWriter out;
            out.open("output_camera_" + std::to_string(i) + ".avi", VideoWriter::fourcc('X', 'V', 'I', 'D'), 20.0, Size(320, 240));
            out_writers[i] = out;
        }
        else {
            cams[i] = VideoCapture();
            out_writers[i] = VideoWriter();
        }
    }

    time_t start_time = time(NULL);
    const int recording_duration = 10 * 60;  // 10 minutes

    while (true) {
        Mat frames[4];

        for (int i = 0; i < 3; i++) {
            if (cams[i].isOpened()) {
                frames[i] = capture_frame(cams[i]);
            }
            else {
                frames[i] = static_image_resized;
            }
        }
        frames[3] = static_image_resized;

        Mat top_row, bottom_row, combined_frame;
        hconcat(frames[0], frames[1], top_row);
        hconcat(frames[2], frames[3], bottom_row);
        vconcat(top_row, bottom_row, combined_frame);

        imshow("Multi-Camera View", combined_frame);

        // Save each camera's frame to its respective file
        for (int i = 0; i < 3; i++) {
            if (out_writers[i].isOpened()) {
                out_writers[i].write(frames[i]);
            }
        }

        if (difftime(time(NULL), start_time) > recording_duration) {
            printf("Recording complete.\n");
            break;
        }

        if (waitKey(1) == 'q') {
            printf("Recording stopped by user.\n");
            break;
        }
    }

    // Release all cameras and video writers
    for (int i = 0; i < 3; i++) {
        if (cams[i].isOpened()) {
            cams[i].release();
        }
        if (out_writers[i].isOpened()) {
            out_writers[i].release();
        }
    }

    destroyAllWindows();
    return 0;
}