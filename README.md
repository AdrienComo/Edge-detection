# General Program for Edge detection

This project has always kept in mind that large images take a lot of time to process. 
We have therefore always kept the code as simple and clean as possible to maximize the speed at which the segmentation is done. This allows us to
provide a low cost, immediate results, segmentation solution.

Detailed explanations of how the program works can be found in ```.pdf``` file. <br>
Performance analysis and data structures discussions can be found in the same file. <br>

### How to run

At the root of the project type :
```𝑚𝑎𝑘𝑒 𝑐𝑙𝑒𝑎𝑛 && 𝑚𝑎𝑘𝑒 && 𝑐𝑙𝑒𝑎𝑟 && . /𝑏𝑖𝑛/𝑚𝑎𝑖𝑛 𝑎 𝑏 𝑐 𝑑 𝑒 𝑓 𝑔```<br> <br>
a : input path to image (img/...) (works with png, jpg, jpeg)
b : 0 < x < 5 : Gaussian blur kernel size (0, 3 , 5)
c : kernel choice
- 0 : Prewitt
- 1 : Sobel
- 2 : Kirsch
d : time to display openCV windows when executing
e : 0 : bi-directionnal, 1 : multi-directional
f : high threshold
g : low threshold

A good example is : ```make clean && make && ./bin/main img/bugatti.jpg 5 2 100 1 40 90```

### Output

![colored borders](img/color_multi.png?raw=true "Colored border")
![refined borders](img/affinage_multi.png.jpg?raw=true "Refined borders") <br>

> We can precisely see the borders of the regions in 4 different colors. Each color depending on the rientation of the border (4 possible) on the first image. the second image represents a refinement of the borders. <br>

### Performance + detailed explanations

> Check the ```.pdf``` file.


### Dependencies 

This code uses the OpenCV C++ library.




