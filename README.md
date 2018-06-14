# TUeLaneTracker

This is a software application that detects and tracks lane boundaries. The underlying algorithm is a probabilistic classifier which was originally developed, under the strategic area of Smart Mobility, at Eindhoven University of Technology (TU/e). The algorithm exploits the concept of hierarchical classification from deep learning, however, unlike deep learning, classification at each hierarchical level is engineered instead of being trained through images. The software application is completely object oriented and follows various software design principles recommended by the safety standard ISO26262. 

This application provides a loose coupling between the software control flow and the algorithm, making it possible to generate various target specific implementations for the algorithm. Besides this generic implementation, an accelrated version for the NXP-BlueBox (s32v) is also available. The s32v specific implementation makes use of the APEX accelrators to speed-up the vision processing. The APEX architecture blends scalar and vector processing capabilities within the two fully programable cores, achieving an effective 5-10x speed-up of the algorithm. 


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What librariesyou need, in order to buiild and install the TUeLaneTracker and 

```
* OpenCv-3.1.0
* Boost-1.62.0
```
How to install them:

[OpenCV Installation Guide](http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html)

[Boost Library Sources](http://www.boost.org/users/history/version_1_62_0.html)
  

### Build, Install and Run the Application

Follow the following steps to run the software application

1. clone this repository: 

```
git clone https://github.com/RameezI/TUeLaneTracker.git
```


2. Go to the parent directory of the cloned repository:
```
cd TUeLaneTracker
```

3. Create a build subdirectory and invoke cmake:
```
mkdir build
cd build
cmake ..
```

4. Build and Install the executeable:
```
make install
```

5. Run the executeable: 
```
cd ../install/bin
./TUeLaneTracker
```

### Modifying the Frame Source for the Application

The TUeLaneTracker has three modes for recieving the input frames :
1. imgstore  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [Images in a directory]
2. stream    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [NetworkStream, VideoFile, V4LCapture]
3. gmsl      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [High-Speed LVDS Camera Interface] [In-Progress] 
   
For usage instructions use the *--help* switch
```
./TUeLanetracker --help
```

### Callibrarting Camera for TUeLaneTracker

- Camera intrinsic and extrinsic parameters can be found in [install/ConfigFiles/Camera](https://github.com/RameezI/TUeLaneTracker/tree/master/install/ConfigFiles/Camera) directory.

- To select a specific configuration for the TUeLaneTracker, you only need to update the CAMERA_NAME define in the [Config.h](https://github.com/RameezI/TUeLaneTracker/blob/master/include/Config.h)

- To generate a new camera callibration use [CameraCalibrator](https://github.com/RameezI/TUeLaneTracker/tree/master/CameraCalibrator) cmake project.
       
       
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

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/RameezI/TUeLaneTracker/blob/master/LICENSE.md) file for details


## Acknowledgments

This research and design project was supported by NXP Semiconductors and Eindoven University of Technology. 
