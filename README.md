# General Program for Edge detection

This project has always kept in mind that large images take a lot of time to process. 
We therefore have always kept the code as simple and clean as possible to maximize the speed at which the segmentation is done. This allows us to provide a low cost, immediate results, edge detection solution.

Detailed explanations of how the program works can be found in ```.pdf``` file. <br>
Performance analysis and data structures discussions can be found in the same file. <br>

### How to run

At the root of the project type :
```𝑚𝑎𝑘𝑒 𝑐𝑙𝑒𝑎𝑛 && 𝑚𝑎𝑘𝑒 && 𝑐𝑙𝑒𝑎𝑟 && . /𝑏𝑖𝑛/𝑚𝑎𝑖𝑛 𝑎 𝑏 𝑐 𝑑 𝑒 𝑓 𝑔```<br> <br>
> a : input path to image (img/...) (works with png, jpg, jpeg) <br>
> b : 0 < x < 5 : Gaussian blur kernel size (0, 3 , 5)<br>
> c : kernel choice (0 : Prewitt, 1 : Sobel, 2 : Kirsch)<br>
> d : time to display openCV windows when executing<br>
> e : 0 : bi-directionnal, 1 : multi-directional<br>
> f : high threshold<br>
> g : low threshold<br>

A good example is : ```make clean && make && ./bin/main img/bugatti.jpg 5 2 100 1 40 90```

### Output

![colored borders](img/color_multi.png?raw=true "Colored border")
![refined borders](img/affinage_multi.png?raw=true "Refined borders") <br>

> We can precisely see the borders in 4 colors on the first image. Each color depending on the orientation of the borders (4 possible). The second image represents a refinement of the borders. <br>

### Performance + detailed explanations

> Check the ```.pdf``` file.


### Dependencies 

This code uses the OpenCV C++ library.




