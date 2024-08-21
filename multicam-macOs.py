import numpy as np
import cv2

placeholder_image_path = '/Users/hikmetaydogan/camera_0_frame_1724103857.jpg' # Change the path to your dir
placeholder_image = cv2.imread(placeholder_image_path)


if placeholder_image is None:
    raise FileNotFoundError(f"Image not found at path: {placeholder_image_path}")

# Capture video
video_capture_0 = cv2.VideoCapture(0)
video_capture_1 = cv2.VideoCapture(1)
video_capture_2 = cv2.VideoCapture(2)


frame_width = int(video_capture_0.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(video_capture_0.get(cv2.CAP_PROP_FRAME_HEIGHT))

placeholder_image = cv2.resize(placeholder_image, (frame_width, frame_height))

while True:
    ret0, frame0 = video_capture_0.read()
    ret1, frame1 = video_capture_1.read()
    ret2, frame2 = video_capture_2.read()

    if not ret0:
        frame0 = placeholder_image
    if not ret1:
        frame1 = placeholder_image
    else:
        frame1 = cv2.resize(frame1, (frame0.shape[1], frame0.shape[0]))  
    if not ret2:
        frame2 = placeholder_image
    else:
        frame2 = cv2.resize(frame2, (frame0.shape[1], frame0.shape[0])) 

    top_row = np.hstack((frame0, frame1))
    bottom_row = np.hstack((frame2, placeholder_image))
    combined_frame = np.vstack((top_row, bottom_row))

    # Display part
    cv2.imshow('MultiCamera Display', combined_frame)

    # q for exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

video_capture_0.release()
video_capture_1.release()
video_capture_2.release()
cv2.destroyAllWindows()
