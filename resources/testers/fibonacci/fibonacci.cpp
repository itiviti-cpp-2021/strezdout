#include <iostream>
#include "fibonacci.h"

int main() {
  int n;
  std::cin >> n;
  std::cout << "STARTTEST" << std::endl;
  std::cout << "INITIALIZED" << std::endl;
  int result = fibonacci(n);
  std::cout << "ENDTEST" << std::endl;
  std::cout << result;
  return 0;
}
