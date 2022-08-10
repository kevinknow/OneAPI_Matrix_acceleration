#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;
class Matrix {


public:
    int n;
    double* data;
    Matrix(int n):n(n) {
        data = new double[n*n];
    }

    Matrix(int n, double* p):n(n), data(p) {
    }

    void init_random(){     //// 取随机数
        for(int i=0;i<n*n;++i) {
            data[i] =1.0*rand()/RAND_MAX;
        }
    }
    ~ Matrix() {delete data;}
};
void mul(const Matrix& a, const Matrix& b, Matrix& res) {
    for(int i=0;i<a.n;++i) {
        for(int j=0;j<a.n;++j) {
            res.data[i*res.n+j] = 0;
            for(int k=0;k<a.n;++k){
                res.data[i*res.n+j] += a.data[i*a.n+k]*b.data[k*b.n+j];
            }
        }
    }
}
double test(int n) {
    Matrix a(n);
    a.init_random();
    Matrix b(n);
    b.init_random();
    Matrix c(n);
    clock_t start = clock();  /// 开始计时
    mul(a,b,c);
    clock_t end = clock();    /// 结束计时
    return 1000.0*(end-start)/CLOCKS_PER_SEC;
}

int main() {
    int n=64;
    cout << n << "*" << n << " cost " << test(n)  << "ms" << std::endl;
    n=128;
    cout << n << "*" << n << " cost " << test(n)  << "ms" << std::endl;
    n=256;
    cout << n << "*" << n << " cost " << test(n)  << "ms" << std::endl;  
    n=512;
    cout << n << "*" << n << " cost " << test(n)  << "ms" << std::endl;
    n=1024;
    cout << n << "*" << n << " cost " << test(n)  << "ms" << std::endl;
}
