# StereoMatching


A simple C++ algorithm to perform Stereo block matching with OpenCV .

### Installation

```sh
$ git clone https://github.com/y3n11/StereoMatching.git
$ cd StereoMatching/src
$ make
```
### Usage
```sh
$ ./StereoMatching <Left image> <Right image> <Method> <Window_Size> <Disp>
```

### Methods
- SAD : Sum of absolute differences
- SSD : sum of squared differences
- LBP : Local binary pattern

### Example:

```sh
./StereoMatching ../images/left.tif ../images/right.tif SAD 5 40
```
### Left Image
![alt tag](https://0x0.st/dIz.jpg)

### Right Image
![alt tag](https://0x0.st/dIi.jpg)

### Disparity Image
![alt tag](https://0x0.st/dIs.jpg)

