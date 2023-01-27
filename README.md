First, install dependencies. The easy ones are CMake, GSL, and GTK:

	sudo apt-get install cmake

	sudo apt-get install libgsl-dev

	sudo apt-get install libgtk2.0-dev

The difficult one is OpenCV, which you must install manually.
Note: You MUST install GTK before the following steps. Otherwise you will have to do it over again. 

(Below quoted from OpenCV docs)
 
Install minimal prerequisites (Ubuntu 18.04 as reference)

	sudo apt update && sudo apt install -y cmake g++ wget unzip

Download and unpack sources

	wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip

	unzip opencv.zip

Create build directory

	mkdir -p build && cd build

Configure

	cmake  ../opencv-4.x

Build

	cmake --build .

(End quote from OpenCV) 

Next, simply call the install script:

	./install.sh 

You can now run the interactive launcher by calling the executable:

	./dot_finder.exe

Launch options will be described within the launcher. 

After the program runs, it will output "threshold.jpg" and "fit.jpg" in addition to the appropriate data file. 
These are used for visualization/debugging of the algorithm. 

Docker containerization is currently nonfunctional. 
