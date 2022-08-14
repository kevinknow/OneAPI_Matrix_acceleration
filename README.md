#Background#

SYCL is a programming model for heterogeneous computing that builds on pure C++. SYCL implementation for CPUs can be implemented without requiring a dedicated compiler. 英特尔提供的基于SYCL的OneAPI平台，对异构硬件进行一定程度上编程语言的统一，提高了可移植性，并且可以在完全不同的设备之间分配工作负载以利用它们的硬件功能。
GEMM（General Matrix Multiplication，通用矩阵乘法）是并行计算中经典的计算密集型应用, 本实验设计将两者结合，通过Intel提供的Devcloud平台对General Matrix Multiplication 进行了优化加速并进一步适配到OneAPI平台，验证对比了不同大小的矩阵在CPU，GPU设备上运行的性能以及代码的可移植性。紧接着通过利用SYCL语言特有的 Shared Memory 来减少重复访存， 添加Block Tile 来对GEMM代码进行进一步并行化优化，并对前后的数据进行了整理与对比，

<img width="745" alt="image" src="https://user-images.githubusercontent.com/60191124/184537958-d36f61e5-8bfe-4652-9598-dd565f16ccc3.png">


#Team and Project Information#

University name: the University of Edinburgh
Supervisor: Dr Danial Chitnis (email: d.chitnis@ed.ac.uk)
Student: Zhenchao Qi (email: s2325590@ed.ac.uk)
Lab: Intel Devcloud 
GPU  Device：Intel(R) UHD Graphics P630
CPU  Device：Intel(R) Xeon(R) E-2176G CPU @ 3.70GHz
Video link: https:
Report: 
