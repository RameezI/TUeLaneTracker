# TUeLaneTracker

This is a software application that detects and tracks lanes on the road. The underlying algorithm is a probabilistic classifier which is originally develope, under the strategic area of Smart Mobility, at Eindhoven University of Technology (TU/e). The algorithm exploits the concept of hierarchical classification from deep learning, however, unlike deep learning, classification at each hierarchical level is engineered instead of being trained through images. This make it more predictable as well as verifiable. The software application is completely object oriented and follows various software design principles recommended by the safety standard ISO26262. 

This application provide a loose coupling between the software control flow and the algorithm, making it possible to generate various target specific implementations of the algorithm. This software applicataion is developed in cooperation with NXP Semiconductors. An accelrated version of the algorithm is also available for NXP-BlueBox, which makes use of the hardware accelrators in the platform. NXP-BlueBox is an energy efficient and high perfomance embedded platform, designed by NXP to enable highly automated driving. The BlueBox provides various hardware accelrators to speed-up the vision processing. However, this repository provides only a generic implementation of the algorithm using OpenCV library.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

```
* OpenCv-3.1.0
* Boostv-1.62.0
```
[OpenCV Installation Guide](http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html)

[Boost Library Sources](http://www.boost.org/users/history/version_1_62_0.html)

Ubuntu 17.10 Installation:
```
sudo apt-get install libboost-dev-all
```
[Ubuntu 16.04 Installation Guide](https://askubuntu.com/questions/859333/how-to-install-libboost-version1-59-or-newer-on-ubuntu16-04)


### Build, Install and Run the Application

Follow the following steps to run the software application

1. clone this repository 

```
git clone https://github.com/RameezI/TUeLaneTracker.git
```


2. Go to the parent directory of the cloned repository:
```
cd TUeLaneTracker
```

3. Create a build subdirectory and invoke cmake
```
mkdir build
cd build
cmake ..
```

4. Build and Install the executeable
```
make install
```

5. Run the executeable <sup>*</sup>. 
```
cd ../install/bin
./TUeLaneTracker
```

* Executeable must be invoked from within the bin directory.       


### Modifying the Frame Source for the Application

The TUeLaneDetector has three options for recieving the input frames, defined in the header *TUeLDT/include/State.h* :
1. DIRECTORY [Default]
2. STREAM
3. GMSL [IN PROGRESS]
   
To update the frame source use the command line arguments, for available command line options use the *--help* switch
```
./TUeLanetracker --help
```

### Callibrarting TUeLaneTracker

Currently, the callibration is parameterized in the corresponding header files. To callibrate the lane tracker for a new input source,  following hearder files must be updated:

1. Camera intrinsic and extrinsic parameters can be found in *Camera* constructor.

   "TUeLaneTracker/TUeLDT/include/Camera.h"
   
2. The size and location of the region of interest is defined, based on the location of the horizon and the vertical range of vanishing point, in Templates constructor.

       "TUeLaneTracker/TUeLDT/include/Templates.h"
       
 3. Furhtermore, the following files provides parameters for the 1D lane filter and the 2D vanishing point filter. The position of the 	  base and purview line is also defined by the constructors of *LaneFilter* and *VanishingPtFilter* class in VP coordinate system.
 
	"TUeLaneTracker/TUeLDT/include/LaneFilter.h" 
	
	"TUeLaneTracker/TUeLDT/include/VanishingPtFilter.h"
  
       
## Youtube Videos
   These videos showcase the functional performance of the algorithm:
   * [Eindhoven](https://youtu.be/7D1vBPrcPk0)



## Built With

* [OpenCV3.1](http://docs.opencv.org/3.1.0/index.html) - Copmputer vision library
* [CMake](https://maven.apache.org/) - Dependency management and makefiles generation
* [Eigen 3.3.3](http://eigen.tuxfamily.org/index.php?title=Main_Page) - Linear Algebra  library
* [Boost 1.62.0](http://www.boost.org/users/history/version_1_62_0.html) - Provides Program Options


## Contributing

Please read [CONTRIBUTING.md](TO BE DEFINED) for details on our code of conduct, and the process for submitting pull requests to us.


## Authors
* **Rameez Ismail**
See also the list of [contributors](https://github.com/RameezI/TUeLaneTracker/graphs/contributors) who participated in this project.


## License

This project is licensed under the MIT License - see the [LICENSE.md] (LICENSE.md) file for details


## Acknowledgments

This research and design project was supported by NXP Semiconductors and Eindoven University of Technology. 
