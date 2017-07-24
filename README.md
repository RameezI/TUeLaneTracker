# TUeLaneTracker

This is a software application that detects and tracks lanes on the road. The underlying algorithm is a probabilistic algorithm which is originally developed at, under the strategic area of Smart Mobility, Eindhoven University of Technology (TU/e). The algorithm exploits the concept of hierarchical classification from deep learning however, unlike deep learning, classification at each hierarchical level is engineered instead of being trained through images. This make it more predictable as well as verifiable. The software application is completely object oriented and follows various software design principles recommended by safety standard ISO26262. 

This application provide a loose coupling between the software control flow and actual realisation of the algorithm, making it possible to generate various target specific implementations of the algorithm. Current version of the software applicataion is developed in cooperation with NXP Semiconductors. This repository provides real-time generic implementation of the algorithm using OpenCV library. A real time embedded implementation, using NXP Bluebox, is underdevelopment.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

```
* OpenCv-3.1.0
```
[OpenCv Installaion Guide](http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html)

### Building and Installing the Application

A step by step series of examples that tell you have to get a development env running

clone this repository 

```
git clone https://github.com/RameezI/TUeLaneTracker.git
```

Go to the parent directory of the cloned repository:
```
cd TUeLaneTracker
```
Open main.cpp file in an editor:
```
vim  +24 LaneTrackerApp/main.cpp
```
Update the location or the DataSet<sup>1</sup>. by editing the folllowing lines in main.cpp:
```
cv::String folder = "/media/rameez/Linux-Extended/DataSet/eindhoven/PNG_imgs";
```
Create a build subdirectory and invoke cmake
```
mkdir build
cd build
cmake ..
```

Build and Install the executeable
```
make install
```
Run the executeable <sup>2</sup>. 
```
cd ../install/bin
./TUeLaneTracker
```
1. The DataSet must have PNG images of resolution 640x480.

2. Executeable must be invoked from within the bin directory.
       
## Youtube Video
   This movie shocases the functional performance of the algorithm
   
   [Eindhoven](https://www.youtube.com/)


## Built With

* [OpenCV3.1](http://docs.opencv.org/3.1.0/index.html) - Copmputer vision library
* [CMake](https://maven.apache.org/) - Dependency management and makefiles generation
* [Eigen 3.3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page) - Linear Algebra  library


## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.


## Authors
* **Rameez Ismail**
See also the list of [contributors](https://github.com/RameezI/TUeLaneTracker/graphs/contributors) who participated in this project.


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details


## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc

