#ifndef _DOTFINDER_CURATOR_HPP_
#define _DOTFINDER_CURATOR_HPP_

#include "dotFinder/system_definitions.hpp"
#include "opencv2/opencv.hpp"

class Curator {
private:
  int i_team_{-1}; // Index of chosen optics team
  Sys::Vec<Sys::Str> teams_{"Albus", "Bellatrix", "Cedric"}; // Optics teams
  double pixel_size_{0.01};                                  // 10mm; in m
  Sys::Vec<double> r_expected_{5, 50, 5}; // Expected dot radius for each team
  Sys::Vec<double> img_center_{0.0, 0.0};
  Sys::Vec2D<double> dot_coords_{}; // Extracted (x,y) coords from image

public:
private:
  void CheckArgs(int argc, char *argv[]);
  void FitImage();
  void OutputData();

public:
  Curator(int argc, char *argv[]) {
    CheckArgs(argc, argv);
    FitImage();
    OutputData();
  }
};

#endif