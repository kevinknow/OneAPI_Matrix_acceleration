Acceleration of General Matrix Multiplicatio on Intel OneAPI
============================================================

Background
----------
SYCL is a programming model for heterogeneous computing that builds on pure C++. SYCL implementation for CPUs can be implemented without requiring a dedicated compiler. 英特尔提供的基于SYCL的OneAPI平台，对异构硬件进行一定程度上编程语言的统一，提高了可移植性，并且可以在完全不同的设备之间分配工作负载以利用它们的硬件功能。
GEMM（General Matrix Multiplication，通用矩阵乘法）是并行计算中经典的计算密集型应用, 本实验设计将两者结合，通过Intel提供的Devcloud平台对General Matrix Multiplication 进行了优化加速并进一步适配到OneAPI平台，验证对比了不同大小的矩阵在CPU，GPU设备上运行的性能以及代码的可移植性。紧接着通过利用SYCL语言特有的 Shared Memory 来减少重复访存， 添加Block Tile 来对GEMM代码进行进一步并行化优化，并对前后的数据进行了整理与对比，


Team and Project Information
----------------------------
University name: the University of Edinburgh
Supervisor: Dr Danial Chitnis (email: d.chitnis@ed.ac.uk)
Student: Zhenchao Qi (email: s2325590@ed.ac.uk)
Lab: Intel Devcloud 
GPU  Device：Intel(R) UHD Graphics P630
CPU  Device：Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz
Video link: https:
Report: 

Experimental Results
-----------------------
<img width="745" alt="image" src="https://user-images.githubusercontent.com/60191124/184537958-d36f61e5-8bfe-4652-9598-dd565f16ccc3.png">

Then fix the size of the matrix, take a matrix of size 512*512 as an example, change the size of X and Y  corresponding to the tile (GEMM), and compare the time required to perform matrix operations.

<img width="431" alt="image" src="https://user-images.githubusercontent.com/60191124/184538056-7ad7223e-8062-4200-b251-77e1af7764a6.png">





![8601660346904_.pic_hd.jpg](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/6b4fe922-e2fa-4758-972f-8d298253d5c4/8601660346904_.pic_hd.jpg)
![8611660347138_.pic_hd.jpg](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/46114c40-778a-4c3b-afa3-bce66f54caf0/8611660347138_.pic_hd.jpg)
![8591660346587_.pic_hd.jpg](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/a7da340e-4586-4924-b035-55a09d5bf75b/8591660346587_.pic_hd.jpg)
![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/dbaaefcc-7f4c-4b5a-8f3f-8548d7c71baf/Untitled.png)
![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/634ce5f5-6757-4cba-bc43-66718a50787a/Untitled.png)
![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/c83d6030-866a-4d2b-8075-6b79e404ecf0/Untitled.png)
