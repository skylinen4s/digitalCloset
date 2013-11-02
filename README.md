digitalCloset
This project is for 全國微電腦應用系統設計製作競賽 with STUST  (2013.11.1)

Version OpneCV 2.4.2
---------------------------------
compile:
./compileOpenCV2_4_2.sh

execute:
./facedetect

---------------------------------
About:
 At first, it only capture image from the camera.
 And show the video on "result" window, two clothes image on closet1 window seperately.
 After click mouse left button on the closet window to put on the clothes in video.
 And right button to open the closet.
 After several seconds it would close.
 
---------------------------------
Code:

 facedetect.cpp:
  Used to detect face and draw the cloth image to the video with corresponding position.

 digitalCloset_Motorcontrol.cpp:
  Used to control motor under the closet.

 motor_test.c:
  control motor by input:"time layer in/out" (ex:5 1 2).

---------------------------------
Hardware:
 -Beagleboard xM
 -L298-Based Stepper/DC Motor Driver Board
 -Webcam/Camera
 -Clothes closet
 -Motor

---------------------------------
problem:
 "Segmentation Fault" when detect face and paste the clothes.


