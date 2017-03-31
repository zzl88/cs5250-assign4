#include <fstream>

int main(int argc, char* argv[]) {
  std::ofstream fs("/dev/bytes");
  for (int i = 0 ; i < 5 * 1024 * 1024; i++) {
    fs << (char) (i % 10 + '0');
  }
  fs.close();
}
