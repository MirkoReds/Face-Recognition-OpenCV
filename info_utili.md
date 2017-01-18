# StageEyeTracking
Eye tracking, tool utili per il mio stage

IP CAMERA ACCESS
In the browser of your choice (I'm using Chrome), navigate to the camera's IP address. Provide credentials as necessary.
You should be looking at a live stream from your camera. Right-click on the video stream and select "Inspect Element" (or its equivalent in non-Chrome browsers).
You should see a variable called src - this is what you can use within OpenCV to access the camera directly. Mine is /mjpg/video.mjpg, and I bet yours will be similar.
HAAR CASCADE FILE
The available Haar-Cascades for face detection are located in the data folder of your OpenCV installation! One of the available Haar-Cascades for face detection is for example /path/to/opencv/data/haarcascades/haarcascade_frontalface_default.xml.



DOCUMENTAZIONE OPEN CV http://docs.opencv.org/3.2.0/index.html

TUTORIAL OPEN CV http://docs.opencv.org/3.2.0/d9/df8/tutorial_root.html

VIDEO CAPTURE OPEN CV http://docs.opencv.org/3.2.0/d8/dfe/classcv_1_1VideoCapture.html

BACKGROUND SUBTRACTURE http://docs.opencv.org/3.2.0/d1/dc5/tutorial_background_subtraction.html

FACE TRACKING TUTORIAL http://synaptitude.me/blog/smooth-face-tracking-using-opencv/

CMAKE TUTORIAL https://www.youtube.com/watch?v=mKNGH58tcYA



