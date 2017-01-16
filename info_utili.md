# StageEyeTracking
Eye tracking, tool utili per il mio stage

IP CAMERA ACCESS
In the browser of your choice (I'm using Chrome), navigate to the camera's IP address. Provide credentials as necessary.
You should be looking at a live stream from your camera. Right-click on the video stream and select "Inspect Element" (or its equivalent in non-Chrome browsers).
You should see a variable called src - this is what you can use within OpenCV to access the camera directly. Mine is /mjpg/video.mjpg, and I bet yours will be similar.


DOCUMENTAZIONE OPEN CV http://docs.opencv.org/3.2.0/index.html
TUTORIAL OPEN CV http://docs.opencv.org/3.2.0/d9/df8/tutorial_root.html



