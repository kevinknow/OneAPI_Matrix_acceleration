#include <chrono>
#include <iostream>
#include <CL/sycl.hpp>

#define random_float() (rand() / double(RAND_MAX))

using namespace std;
using namespace sycl;

// return execution time
double gpu_kernel(float *A, float *B, float *C, int M, int N, int K, int block_size, sycl::queue &q) {

  // define the workgroup size and mapping
  auto grid_rows = (M + block_size - 1) / block_size * block_size;
  auto grid_cols = (N + block_size - 1) / block_size * block_size;
  auto local_ndrange  = range<2>(block_size, block_size);
  auto global_ndrange = range<2>(grid_rows, grid_cols);

  double duration = 0.0f;
  auto e = q.submit([&](sycl::handler &h) {
      h.parallel_for<class k_name_t>(
          sycl::nd_range<2>(global_ndrange, local_ndrange), [=](sycl::nd_item<2> index) {

//               int row = index.get_global_id(0);
//               int col = index.get_global_id(1);
              
              /*先定位group编号，再确定group内（即local）编号*/
              int row = index.get_group(0)*block_size+index.get_local_id(0);
              int col = index.get_group(1)*block_size+index.get_local_id(1);
              
              float sum = 0.0f;

              for (int i = 0; i < K; i++) {
                sum += A[row * K + i] * B[i * N  + col];
              }
              C[row * N + col] = sum;  
          });
    });
    e.wait();

    duration += (e.get_profiling_info<info::event_profiling::command_end>() -
    e.get_profiling_info<info::event_profiling::command_start>()) /1000.0f/1000.0f;

    return(duration);
}

// return execution time
double cpu_kernel(float *cA, float *cB, float *cC, int M, int N, int K) {
    
    double duration = 0.0;
    std::chrono::high_resolution_clock::time_point s, e;

    // Single Thread Computation in CPU 
    s = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            float sum = 0.0f;
            for(int k = 0; k < K; k++) {
                sum +=  cA[i * K + k] * cB[k * N  + j];
            }
            cC[i * N + j] = sum;
        }
    }
    e = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration<float, std::milli>(e - s).count();

    return(duration);
}

int verify(float *cpu_res, float *gpu_res, int length){
    int err = 0;
    for(int i = 0; i < length; i++) {
       if( fabs(cpu_res[i] - gpu_res[i]) > 1e-3) {
          err++;
          printf("\n%lf, %lf", cpu_res[i], gpu_res[i]);
       } 
    }
    return(err);
}

int gemm(const int M, 
         const int N, 
         const int K, 
         const int block_size,
         const int iterations, 
         sycl::queue &q) {

  cout << "Problem size: c(" << M << "," <<  N << ") ="
       << " a(" << M << "," << K << ") *" 
       << " b(" << K << "," << N << ")\n";

  auto A = malloc_shared<float>(M * K, q);
  auto B = malloc_shared<float>(K * N, q);
  auto C = malloc_shared<float>(M * N, q);
  auto C_host = malloc_host<float>(M * N, q);

  // init the A/B/C
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

  double flopsPerMatrixMul
      = 2.0 * static_cast<double>(M) * static_cast<double>(N) * static_cast<double>(K);

  double duration_gpu = 0.0f;
  double duration_cpu = 0.0f;

  // GPU compuation and timer 
  int warmup = 10;
  for (int run = 0; run < iterations + warmup; run++) {
    float duration = gpu_kernel(A, B, C, M, N, K, block_size, q);
    if(run >= warmup) duration_gpu += duration;
  }
  duration_gpu = duration_gpu / iterations;

  // CPU compuation and timer 
  warmup = 2;
  for(int run = 0; run < iterations/2 + warmup; run++) {
      float duration = cpu_kernel(A, B, C_host, M, N, K);
      if(run >= warmup) duration_cpu += duration;
  }
  duration_cpu = duration_cpu / (iterations/2);  //（疑似错误）应该除以上面循环里实际迭代的次数iteration/2

  // Compare the resutls of CPU and GPU 
  int errCode = 0;
  errCode = verify(C_host, C, M*N);
  if(errCode > 0) printf("\nThere are %d errors\n", errCode);

  printf("\nPerformance Flops = %lf, \n" 
          "GPU Computation Time = %lf (ms); \n"
          "CPU Computation Time = %lf (ms); \n",     /////tion<-iton
          flopsPerMatrixMul, duration_gpu, duration_cpu);

  free(A, q);
  free(B, q);
  free(C, q);
  free(C_host, q);

  return(errCode);
}

class my_device_selector : public device_selector {
public:
    my_device_selector(std::string vendorName) : vendorName_(vendorName){};
    int operator()(const device& dev) const override {
    int rating = 0;
    //We are querying for the custom device specific to a Vendor and if it is a GPU device we
    //are giving the highest rating as 3 . The second preference is given to any GPU device and the third preference is given to
    //CPU device.
    if (dev.is_cpu() & (dev.get_info<info::device::name>().find(vendorName_) != std::string::npos))
        rating = 3;
    else if (dev.is_gpu() & (dev.get_info<info::device::name>().find(vendorName_) != std::string::npos)) rating = 2;
    else if (dev.is_cpu()) rating = 1;
    return rating;
    };
    
private:
    std::string vendorName_;
};


int main() {

  auto propList = cl::sycl::property_list {cl::sycl::property::queue::enable_profiling()};
  queue my_gpu_queue( cl::sycl::gpu_selector{} , propList);

  //pass in the name of the vendor for which the device you want to query 
    std::string vendor_name = "Intel";
    //std::string vendor_name = "AMD";
    //std::string vendor_name = "Nvidia";
    my_device_selector selector(vendor_name);
    queue q(selector);
    std::cout << "Device: "
    << q.get_device().get_info<info::device::name>() << "\n";

  int errCode = gemm(2000, 2000, 2000, 4, 10, my_gpu_queue); //改为2000

  return(errCode);
}
