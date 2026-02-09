import cv2
import numpy as np

user_interface = "annotate_face"

# region Constants
source = cv2.imread("./data/images/pexels-hillaryfox-1595391.jpg")

banner_height = int(0.1 * source.shape[0])
font_size = int(banner_height / 40)
print(font_size)
font_face = cv2.FONT_HERSHEY_PLAIN
instructions_l1 = "Left-click and drag to select a face."
instructions_l2 = "Hit 'R' to reset, 'Q' to quit, 'Enter' to complete selections."
pos_line1 = (int(0.2 * banner_height), int(0.4 * banner_height))
pos_line2 = (int(0.2 * banner_height), int(0.9 * banner_height))

# endregion

img = None
flag_drawing_box = False
pt_button_down = None
boxes = []


def cbMouseEvent(event, x, y, flags, param):

    global flag_drawing_box, boxes, pt_button_down, img

    if (not flag_drawing_box) and (event == cv2.EVENT_LBUTTONDOWN):

        if (pt_button_down is not None):
            print("[WARN] Point not reset correctly.")

        print(f"[INFO] Drawing from <{x}, {y}>")
        flag_drawing_box = True
        pt_button_down = (x, y)
        return

    if (flag_drawing_box) and (event == cv2.EVENT_LBUTTONUP):
        flag_drawing_box = False

        box = (
            (min(x, pt_button_down[0]),
             min(y, pt_button_down[1])),
            (max(x, pt_button_down[0]),
             max(y, pt_button_down[1]))
        )

        boxes.append(box)

        line_thickness = int((0.005 * img.shape[0]))
        cv2.rectangle(img=img,
                      pt1=boxes[-1][0],
                      pt2=boxes[-1][1],
                      color=(0, 255, 0),
                      thickness=line_thickness,
                      lineType=cv2.LINE_4)

        pt_button_down = None


def resetWindow():
    print("[INFO] Resetting the UI.")

    global img

    # Build an image
    banner = np.full(shape=(banner_height, source.shape[1], source.shape[2]),
                     fill_value=0,
                     dtype="uint8")
    img = np.vstack((banner, source.copy()))

    # Add line 1 of instructions
    cv2.putText(img=img,
                text=instructions_l1,
                org=pos_line1,
                fontFace=font_face,
                fontScale=font_size,
                color=(255, 255, 255),
                thickness=font_size,
                lineType=cv2.LINE_AA)

    # Add line 2 of instructions
    cv2.putText(img=img,
                text=instructions_l2,
                org=pos_line2,
                fontFace=font_face,
                fontScale=font_size,
                color=(255, 255, 255),
                thickness=font_size,
                lineType=cv2.LINE_AA)

    return


def processBoxes():
    print("[INFO] Input completed. Processing boxes.")
    for i, box in enumerate(boxes):
        cv2.imwrite(f"outputs/face_{i}.jpg",
                    img=img[box[0][1]:box[1][1], box[0][0]:box[1][0]])


resetWindow()

cv2.namedWindow(winname=user_interface, flags=cv2.WINDOW_NORMAL)
cv2.setMouseCallback(window_name=user_interface,
                     on_mouse=cbMouseEvent, param=img)

while True:
    cv2.imshow(winname=user_interface,
               mat=img)

    keypress = cv2.waitKey(1)

    if (keypress == ord('R')) or (keypress == ord('r')):
        resetWindow()
        boxes.clear()
        pt_button_down = None
        flag_drawing_box = False
    elif (keypress == ord('Q')) or (keypress == ord('q')):
        break
    elif (keypress == 13):
        processBoxes()
        break
    elif (keypress == -1):
        continue
    else:
        print("[ERROR] Bad keypress. Please use 'R', 'r', 'Q', 'q' or 'Enter'")

cv2.destroyAllWindows()
