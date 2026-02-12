import cv2
import numpy as np

user_interface = "annotate_face"

# region Constants
source = cv2.imread("./data/images/pexels-hillaryfox-1595391.jpg")
img = source.copy()
scale_input = 0
max_scale = 100
scale_type = 0
# endregion


def updateScaleType(*args):
    global scale_type
    scale_type = args[0]


def updateImage(*args):

    scale = args[0]

    if scale_type:
        global max_scale
        scale = 1 - (scale / (max_scale + 1))
    else:
        scale = 1 + (scale / 100)

    interpolation_fn = cv2.INTER_LINEAR if scale > 1 else cv2.INTER_AREA

    global img
    img = cv2.resize(src=source,
                     dsize=None,
                     fx=scale,
                     fy=scale,
                     interpolation=interpolation_fn)

    if scale_type:
        padding_tb = int((source.shape[0] - img.shape[0]) / 2)
        padding_lr = int((source.shape[1] - img.shape[1]) / 2)

        img = cv2.copyMakeBorder(src=img,
                                top=padding_tb,
                                bottom=padding_tb,
                                left=padding_lr,
                                right=padding_lr,
                                borderType=cv2.BORDER_CONSTANT,
                                value=(200, 200, 200))


cv2.namedWindow(winname=user_interface, flags=cv2.WINDOW_AUTOSIZE)
cv2.createTrackbar("Scaling Type \n0: Scale Up \n1: Scale Down",
                   user_interface,
                   0, 1,
                   updateScaleType)

cv2.createTrackbar("Image Scale (%)",
                   user_interface,
                   scale_input,
                   max_scale,
                   updateImage)

while True:
    cv2.imshow(winname=user_interface,
               mat=img)

    keypress = cv2.waitKey(1)

    if (keypress == ord('R')) or (keypress == ord('r')):
        pass  # Reset
    elif (keypress == ord('Q')) or (keypress == ord('q')):
        break
    elif (keypress == -1):
        continue
    else:
        print("[ERROR] Bad keypress. Please use 'R/r' to Reset or 'Q/q' to Quit")

cv2.destroyAllWindows()
