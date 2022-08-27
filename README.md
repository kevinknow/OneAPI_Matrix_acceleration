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


<img width="932" alt="image" src="https://user-images.githubusercontent.com/60191124/187026178-42af5d6d-7cf1-4571-858b-7ecb4200eaa2.png">

