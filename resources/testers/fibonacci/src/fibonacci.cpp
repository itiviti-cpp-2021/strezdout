#include "fibonacci.h"
#include <vector>

// int fibonacci(int n) {
//   if (n <= 0) {
//     return 0;
//   }
//   if (n == 1 || n == 2) {
//     return 1;
//   }
//   return fibonacci(n - 1) + fibonacci(n - 2);
// }

int fibonacci(int n) {
  if (n <= 0) {
    return 0;
  }
  if (n == 1 || n == 2) {
    return 1;
  }
  std::vector<int> fib(n + 1);
  fib[1] = fib[2] = 1;
  for (int i = 3; i <= n; i++) {
    fib[i] = fib[i - 1] + fib[i - 2];
  }
  return fib[n];
}
