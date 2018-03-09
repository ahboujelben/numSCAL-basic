numSCAL copyright: Attribution-NonCommercial 4.0 International
Author: Ahmed Hamdi Boujelben (a.boujelben@hw.ac.uk / ahmed.hamdi.boujelben@gmail.com)
Developed at: Heriot-Watt University

***How to compile numSCAL using Qt Creator***

To compile and run numSCAL, do the following steps:

1- Extract libs.7z and lib2.7z in the current folder. 

2-Open the project using Qt Creator (MinGW version). Refer to numSCAL.pro to open the project.

3-Go to 'Projects' tab (the vertical bar on the left) to configure the compiler. Make sure 'Shadow build' is checked, and take note of the 'build folder' (usually numSCAL-***-Debug). The build folder should be located in the same parent folder of numSCAL project folder.

4-Extract "ffmpeg.7z" and "Extracted Networks.7z" in the 'build folder' (NOT IN THE CURRENT FOLDER).

5-Open the .pro file and keep only the libraries relevent to your OS

6-Compile and run the project by clicking on the green arrow in the vertical bar (bottom left), or simply tap Ctrl+R.

For enquiries, contact the author of the code.

Ahmed Hamdi Boujelben
(a.boujelben@hw.ac.uk / ahmed.hamdi.boujelben@gmail.com)

***Important Notice***
The extracted networks provided with numSCAL package are downloaded from the Imperial College database (https://goo.gl/dccvF9). The author does not claim any ownership of these networks.