# group-project-team1998

This project uses OpenMP to write a parallel version of Conway's "Game of Life", and print the results of the iteration as images. In addition, this project also provides the function that generates the video based on images in jpg format.

## Installation
Compiler: visual studio 2019  
OpenMP: 3.0  
Operating sys: Win 10  
OpenCV: **<font color=red> opencv_world451.lib</font>**    


## Execution
1.	Download the openCV and add it to the visual studio  
(1)	The URL to download the OpenCV  
https://sourceforge.net/projects/opencvlibrary/  
(2)	Add opencv’s bin directory to path.  
(3)	Set platform target to x64  
(4)	Add to Include Directories  
   <font size=1>For example:  
   C:\Program Files\opencv\build\include\opencv2  
   C:\Program Files\opencv\build\include</font>  
(5)	Add to Library Directories  
  <font size=1>For example:  
   C:\Program Files\opencv\build\x64\vc14\lib</font>  
(6)	Add Additional Dependencies  
   <font size=1>For example:  
   opencv_world451.lib</font>  
2.	Change the debug mode to release mode  
3.	Create folders named img and img_test  
4.	Set the “imax”, “jmax” and “max_steps” in ConwaysGame_P_new.cpp  
5.	Run the ConwaysGame_P_new.cpp  
6. If you want to generate the video in test_correct.cpp, please run test_correct.cpp in **release mode** and delete the "//" in the line shown below.   
   ** //cout << "test2 past: " << testcase2() << endl;**  

## Documentation
The documentation.pdf contains documentation of this project

## More information
For more information on the project specification, see team1998group coursework report.pdf.