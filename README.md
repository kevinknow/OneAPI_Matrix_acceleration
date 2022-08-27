Acceleration of General Matrix Multiplicatio using SYCL and OneAPI
============================================================

Background
----------

The OneAPI platform provided by Intel provides a degree of programming language unification based on SYCL for heterogeneous hardware, improving portability and allowing workloads to be distributed between completely different devices to take advantage of their hardware capabilities.
GEMM (General Matrix Multiplication) is a classical computationally intensive application in parallel computing, and this project combines the two by optimising the acceleration of General Matrix Multiplication through the Devcloud platform provided by Intel and further adapting it to the The performance of different matrix sizes on CPU and GPU devices and the portability of the code are verified and compared. The performance of the different matrix sizes on CPU and GPU devices and the portability of the code were verified and compared. By using the shared memory unique to the SYCL language to reduce repeated memory access, adding Block Tile to further parallelize and optimize the GEMM code, and organize and compare the data in different methods.


Team and Project Information
----------------------------
University name: the University of Edinburgh <br>
Supervisor: Dr Danial Chitnis (email: d.chitnis@ed.ac.uk) <br>
Student: Zhenchao Qi (email: s2325590@ed.ac.uk) <br>
Lab: Intel Devcloud  <br>
GPU  Device：Intel(R) UHD Graphics P630  <br>
CPU  Device：Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz <br>
Video link: https: <br>
Report:  <br>

Experimental Results
-----------------------
<img width="745" alt="image" src="https://user-images.githubusercontent.com/60191124/184537958-d36f61e5-8bfe-4652-9598-dd565f16ccc3.png">

Then fix the size of the matrix, take a matrix of size 512*512 as an example, change the size of X and Y  corresponding to the tile (GEMM), and compare the time required to perform matrix operations.

<img width="431" alt="image" src="https://user-images.githubusercontent.com/60191124/184538056-7ad7223e-8062-4200-b251-77e1af7764a6.png">




<img width="666" alt="image" src="https://user-images.githubusercontent.com/60191124/184547488-06376f80-c086-46af-acab-687585883a8d.png">
<img width="666" alt="image" src="https://user-images.githubusercontent.com/60191124/184547499-3f3aeaa3-b4aa-45c2-8315-2d651856cc1c.png">

<img width="368" alt="image" src="https://user-images.githubusercontent.com/60191124/187027622-886259c4-868e-48e5-b2c9-1cc4bb0408ff.png">

<img width="932" alt="image" src="https://user-images.githubusercontent.com/60191124/187026178-42af5d6d-7cf1-4571-858b-7ecb4200eaa2.png">
By comparing the experimental results, it can be seen that the SYCL architecture significantly improves the performance of matrix multiplication. By sorting out the above results, the relative GPU acceleration ratio is obtained through CPU Computation Time and GPU Computation Time.


After using OneAPI to accelerate the general matrix，When the size of the matrix row and column are around 512, the acceleration ratio is 2.95，and when the size of the matrix row and column are around 1024, CPU Computation Time is 394.1 millisecond and GPU Computation Time is 137.2 millisecond, GPU acceleration ratio is 2.87；When the row and column of the matrix reaches 1024, CPU Computation Time is 6152.3 millisecond and GPU Computation Time is 1146.9 millisecond, GPU acceleration ratio is 5.36. When the size of the matrix is small and the change of the row or column is not too large, the effect of GPU parallel acceleration is not obvious. However, when the matrix size becomes larger and exceeds the critical point of 1024, the GPU acceleration effect is significantly improved. 
<img width="580" alt="image" src="https://user-images.githubusercontent.com/60191124/187043816-be8c2dae-72d3-4e99-ac18-e08d8424aae2.png">

Through the horizontal comparison test, it can be seen that for the matrix of block_size=512, after using OneAPI for parallel calculation of Tile optimization, when the values of tile_x and tile_y are both 1，CPU Computation Time is 50.5 millisecond and GPU Computation Time is 17.1 millisecond, GPU acceleration ratio is 3.0；
When the values of tile_x and tile_y are both 2，CPU Computation Time is 60.7 millisecond and GPU Computation Time is 8.6 millisecond, GPU acceleration ratio is 7.1；When one of the parameters of X and Y is changed from 2 to 4, the acceleration rate of GPU is slightly improved, and the acceleration rate is only increased from 7.1 to 8.2 and 8.6 respectively. Besides, when the values of tile_x and tile_y reached 4, The performance of GPU has been significantly improved, the GPU Computation Time has been reduced by almost half, and the CPU Computation Time has also been reduced to a certain extent, but the change has not changed much. The acceleration ratio increased from 7.1 to 11.4, However, this value is close to saturation and remains around 11.5. When the tile_x and y values are changed to 8, the acceleration rate is 11.6. When tile_x and tile_y continue to be increased, the Computation Time of GPU and CPU is not reduced, and the acceleration rate is also decline. 
<img width="506" alt="image" src="https://user-images.githubusercontent.com/60191124/187043828-9e507dfc-6fcd-46fa-88bc-ff1d927975a0.png">

As tile_x or tile_y increases, the GPU acceleration ratio will increase significantly, but when the block_size is exceeded, the boosting effect will decrease significantly. Increasing tile_x or increasing tile_y by the same magnitude has basically the same effect.


