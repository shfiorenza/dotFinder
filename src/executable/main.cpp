#include "dotFinder/curator.hpp"

int main(int argc, char *argv[]) {
  // Initialize curator (essentially our interface to OpenCV)
  Curator image_manager_(argc, argv);
  return 0;
}
