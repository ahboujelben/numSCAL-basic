numSCAL - Open Source Edition
Copyright: Ahmed Hamdi Boujelben 2018-2021
Licence: MIT
Author: Ahmed Hamdi Boujelben (ahmed.hamdi.boujelben@gmail.com)
Developed at: Heriot-Watt University - Edinburgh, UK

Build instructions:
Make sure you have already insalled Qt (mingw32 or mingw64 version) with a mingw compiler that supports C++14.
1- Extract libs.zip in this folder
2- Extract prerequisites.zip to the chosen build folder.
3- build using Qt Creator or by running:
    cd path_to_build_folder
    qmake path_to_this_folder/numSCAL.pro (qmake.exe for Windows)
    make (mingw32-make.exe for Windows)
    for windows, you can deploy by copying numSCAL.exe from release/ to the deployment folder and then copying the following 
    file from QT folder:
    -libgcc_s_seh-1.dll
    -libstdc++-6.dll
    -libwinpthread-1
    -Qt5Core.dll
    -Qt5Gui.dll
    -Qt5OpenGL.dll
    -Qt5PrintSupport.dll
    -Qt5Widgets.dll
    -platforms/qwindows.dll
    -printsupport/windowsprintersupport.dll
    -styles/qwindowsvistastyle.dll
    and from glew
    -glew32.dll

For enquiries, contact the author of the code.
Ahmed Hamdi Boujelben (ahmed.hamdi.boujelben@gmail.com)

Copyright notice:
The extracted networks provided with numSCAL package are downloaded from the Imperial College database (https://goo.gl/dccvF9). The author does not claim any ownership of these networks.