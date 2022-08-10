
%%writefile lab/simple-vector-incr.cpp
//Patri Zhao:  patric.zhao@intel.com

#include <chrono>
#include <iostream>
#include <CL/sycl.hpp>
// 定义随机数生成函数random_float()
#define random_float() (rand() / double(RAND_MAX))

using namespace std;
using namespace sycl;
// 定义Y矩阵和X矩阵的缓存行或者列的大小
#define tileY 4 
#define tileX 8

// A为运算矩阵，B为运算矩阵，C为结果矩阵，M为A的行大小，N为B的行大小，K是A和B的列大小
// BLOCK为GPU并行运算时要进行分组的大小，q为提交GPU运算的job队列
// return execution time GPU
double gpu_kernel(float *A, float *B, float *C, 
                  int M, int N, int K, 
                  int BLOCK, sycl::queue &q) {

  // define the workgroup size and mapping
  // grid_rows grid_cols分别表示对M和N进行缓存后需要进行分组的次数
  auto grid_rows = M / tileY;
  auto grid_cols = N / tileX;
  // 根据BLOCK, grid_rows和grid_cols定义局部和全局分组
  auto local_ndrange  = range<2>(BLOCK, BLOCK);
  auto global_ndrange = range<2>(grid_rows, grid_cols);
// 执行时间
  double duration = 0.0f;
// 利用q进行job队列的提交
  auto e = q.submit([&](sycl::handler &h) {
      h.parallel_for<class k_name_t>(
          sycl::nd_range<2>(global_ndrange, local_ndrange), [=](sycl::nd_item<2> index) {
              // 获取需要计算的row和col
              int row = tileY * index.get_global_id(0);
              int col = tileX * index.get_global_id(1);
            // 根据缓存大小tileY，tileX定义该缓存计算的结果sum
            // 定义A的缓存数组subA，B的缓存数组subB
              float sum[tileY][tileX] = {0.0f};
              float subA[tileY] = {0.0f};
              float subB[tileX] = {0.0f};


               // core computation，对于A矩阵的每一列进行遍历计算
              for (int k = 0; k < N; k++) {
                // read data to register
                for(int m = 0; m < tileY; m++) {
                    /*将A中对应位置元素加入到subA中*/
                    subA[m] = A[(row + m) * N + k];
                } 

                for(int p = 0; p < tileX; p++) {
                    /*同样将B中对应位置元素加入到subB中*/
                    subB[p] = B[k * N + p + col];
                } 

                /*依次遍历subA和subB求解出sum*/
                for (int m = 0; m < tileY; m++) {
                  for (int p = 0; p < tileX; p++) {
                    sum[m][p] += subA[m] * subB[p];
                  }
                }

              } //end of K

              // write results back
              for (int m = 0; m < tileY; m++) {
                for (int p = 0; p < tileX; p++) {
                    /*遍历sum将数据写入到最终结果矩阵C中*/
                  C[(row + m) * N + col + p] = sum[m][p];
                }
              }

          });
    });
    e.wait();

    /*计算执行的时间*/
    duration += (e.get_profiling_info<info::event_profiling::command_end>() -
    e.get_profiling_info<info::event_profiling::command_start>()) /1000.0f/1000.0f;

    /*返回执行的时间*/
    return(duration);
}

/*利用CPU进行计算并返回计算时间*/
// return execution time
/*参数同上个函数*/
double cpu_kernel(float *cA, float *cB, float *cC, int M, int N, int K) {
    /*定义执行时间的变量*/
    double duration = 0.0;
    /*s和e用于记录运行的时间*/
    std::chrono::high_resolution_clock::time_point s, e;

    // Single Thread Computation in CPU
//    获得运行开始的时间s
    s = std::chrono::high_resolution_clock::now();
    /*对矩阵A和矩阵B进行遍历*/
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            /*定义A的行乘以B的列的结果sum*/
            float sum = 0.0f;
//            遍历
            for(int k = 0; k < K; k++) {
                /*求解sum的结果*/
                sum +=  cA[i * K + k] * cB[k * N  + j];
            }
            /*将最终的sum加入到结果矩阵cC中*/
            cC[i * N + j] = sum;
        }
    }
//    获得运行结束的时间e
    e = std::chrono::high_resolution_clock::now();
//    计算执行时间
    duration = std::chrono::duration<float, std::milli>(e - s).count();
//返回执行时间
    return(duration);
}
//用于验证CPU和GPU执行的结果是否一致
int verify(float *cpu_res, float *gpu_res, int length){
    /*定义错误的个数*/
    int err = 0;
    /*对结果进行遍历*/
    for(int i = 0; i < length; i++) {
        /*若2者相差大于0.001，认为结果出错了*/
       if( fabs(cpu_res[i] - gpu_res[i]) > 1e-3) {
           /*累加err*/
          err++;
          /*输出错误的位置*/
          printf("\n%lf, %lf", cpu_res[i], gpu_res[i]);
       } 
    }
    return(err);
}
//进行矩阵CPU和GPU运算，并测试结果
//M，N，K和block_size同上，iterations为计算执行时间时要重复的次数，然后求其平均
int gemm(const int M, 
         const int N, 
         const int K, 
         const int block_size,
         const int iterations, 
         sycl::queue &q) {
//输出信息
  cout << "Problem size: c(" << M << "," <<  N << ") ="
       << " a(" << M << "," << K << ") *" 
       << " b(" << K << "," << N << ")\n";
//对4个矩阵分配空间
  auto A = malloc_shared<float>(M * K, q);
  auto B = malloc_shared<float>(K * N, q);
  auto C = malloc_shared<float>(M * N, q);
  auto C_host = malloc_host<float>(M * N, q);

  // init the A/B/C随机数初始化
  for(int i=0; i < M * K; i++) {
      A[i] = random_float();
  }

  for(int i=0; i < K * N; i++) {
      B[i] = random_float();
  }

  for(int i=0; i < M * N; i++) {
      C[i] = 0.0f;
      C_host[i] = 0.0f;
  }
//定义矩阵乘法所要执行的浮点运算规模
  double flopsPerMatrixMul
      = 2.0 * static_cast<double>(M) * static_cast<double>(N) * static_cast<double>(K);
//定义GPU和CPU执行时间
  double duration_gpu = 0.0f;
  double duration_cpu = 0.0f;

  // GPU compuation and timer 
  int warmup = 10;/*warmup用于对GPU进行热身使用，所以下面的run会多执行warmup次*/
  for (int run = 0; run < iterations + warmup; run++) {
      /*执行并获取时间*/
    float duration = gpu_kernel(A, B, C, M, N, K, block_size, q);
    /*当前执行次已经超过warmup的次数了，为正常统计的时间，累加到duration_gpu*/
    if(run >= warmup) duration_gpu += duration;
  }
  /*求解平均gpu运行时间*/
  duration_gpu = duration_gpu / iterations;

  // CPU compuation and timer 
  warmup = 2;/*定义warmup次数*/
  for(int run = 0; run < iterations/2 + warmup; run++) {
      /*执行并获取时间*/
      float duration = cpu_kernel(A, B, C_host, M, N, K);
      /*当前执行已经超过warmup了，累加到duration_cpu*/
      if(run >= warmup) duration_cpu += duration;
  }
  /*求解平均cup运行时间*/
  duration_cpu = duration_cpu / iterations/2;

  // Compare the resutls of CPU and GPU 
  int errCode = 0;
  /*计算cpu和gpu错误次数*/
  errCode = verify(C_host, C, M*N);
  /*打印错误信息*/
  if(errCode > 0) printf("\nThere are %d errors\n", errCode);

  /*打印矩阵规模及其执行时间*/
  printf("\nGEMM size M = %d, N = %d, K = %d", M, N, K);
  printf("\nWork-Group size = %d * %d, tile_X = %d, tile_Y = %d", block_size, block_size, tileX, tileY);
  printf("\nPerformance Flops = %lf, \n" 
          "GPU Computation Time = %lf (ms); \n"
          "CPU Computaiton Time = %lf (ms); \n", 
          flopsPerMatrixMul, duration_gpu, duration_cpu);
/*释放空间*/
  free(A, q);
  free(B, q);
  free(C, q);
  free(C_host, q);

  return(errCode);
}

int main() {

    /*定义gpu运行的queue*/
  auto propList = cl::sycl::property_list {cl::sycl::property::queue::enable_profiling()};
  queue my_gpu_queue( cl::sycl::gpu_selector{} , propList);
/*调用矩阵运算*/
  int errCode = gemm(512, 512, 512, /* GEMM size, M, N, K */
                     4,             /* workgroup size */ 
                     10,            /* repeat time */   
                     my_gpu_queue);

  return(errCode);
}
