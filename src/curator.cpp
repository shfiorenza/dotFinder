#include "dotFinder/curator.hpp"

void Curator::CheckArgs(int argc, char *argv[]) {
  using namespace Sys;
  // First, check for correct number of input arguments
  if (argc > 3 or argc == 2) {
    printf("Error. Correct useage syntax is:\n\n");
    printf("  ./dot_finder.exe                                 for "
           "interactive launcher, OR\n\n");
    printf("  ./dot_finder.exe [image filename] [team index]   for quick "
           "launch (must build locally)\n\n");
    exit(1);
  }
  // With no other input arguments, run interactive launcher
  if (argc == 1) {
    printf("Running interactive launcher. You can also quick-launch via:\n\n");
    printf("  ./dot_finder.exe [image filename] [team index]\n\n");
    printf("where each team index is given by the following list:\n");
    for (int i_team{0}; i_team < teams_.size(); i_team++) {
      printf("  %i. %s\n", i_team, teams_[i_team].c_str());
    }
    printf("(Note that you must build dotFinder locally to use quick launch "
           "syntax)\n");
    printf("First, please input filename of image to be analyzed.\n");
    std::getline(std::cin, Sys::img_filename_);
    printf("Next, please input team index to choose desired team.\n");
    printf("(Must be an integer between 0 and %zu)\n", teams_.size() - 1);
    std::cin >> i_team_;
    std::cin.ignore(); // Use this to ignore newline character in buffer
    // Validate chosen list index
    if (!std::cin) {
      printf("Error; input value must be an integer. Exiting\n");
      exit(1);
    }
    // If user has input arguments, we must check their validity
  } else if (argc == 3) {
    Sys::img_filename_ = argv[1];
    Str second_input{argv[2]};
    bool second_input_is_int{true};
    for (int pos{0}; pos < second_input.length(); pos++) {
      if (!isdigit(second_input.at(pos))) {
        second_input_is_int = false;
      }
    }
    if (second_input_is_int) {
      i_team_ = std::stoi(argv[2]);
    } else {
      printf("Error; integer index must be used to select teams.\n");
      exit(1);
    }
  }
  // Check that chosen team index is valid
  if (i_team_ < 0 or i_team_ > teams_.size() - 1) {
    printf("Error; team index value must be between 0 and %zu\n",
           teams_.size() - 1);
    exit(1);
  }
}

void Curator::FitImage() {
  using namespace cv;
  // Check if image filename exists
  if (!std::filesystem::exists(Sys::img_filename_)) {
    printf("Error; image '%s' not found. Exiting.\n",
           Sys::img_filename_.c_str());
    printf("(Image name must include extension and relative path if not in "
           "same directory as executable.)\n");
    exit(1);
  }
  // Open image data and log it
  Mat img_raw{imread(Sys::img_filename_, 0)};
  printf("Loaded image '%s'; resolution is %ix%i pixels.\n",
         Sys::img_filename_.c_str(), img_raw.cols, img_raw.rows);
  img_center_[0] = double(img_raw.cols) / 2.0;
  img_center_[1] = double(img_raw.rows) / 2.0;
  // Turn image into a binary using thresholding
  Mat img_thresh;
  threshold(img_raw, img_thresh, 100, 255, cv::ThresholdTypes::THRESH_BINARY);
  // Write threshold image to file for debugging/visualization purposes
  imwrite("threshold.jpg", img_thresh);
  // Set SimpleBlobDetector parameters to best detect the dots we seek
  SimpleBlobDetector::Params params;
  // Look for darker blobs
  params.filterByColor = true;
  params.blobColor = 0;
  // Set thresholding values to utilize the binary image we created
  params.minThreshold = 128;
  params.maxThreshold = 129;
  params.thresholdStep = 1;
  params.minRepeatability = 1;
  // Look for relatively circular blobs (can be distorted by thresholding)
  params.filterByCircularity = true;
  params.minCircularity = 0.6;
  params.maxCircularity = 1.0;
  // We do not consider convexity or inertia currently
  params.filterByConvexity = false;
  params.filterByInertia = false;
  // Set range on blob size so that we ignore thresholding artifacts
  params.filterByArea = true;
  params.minArea = M_PI * 1.0f * 1.0f; // min: circle w/ r = 1 pixel
  params.maxArea = M_PI * 5.0f * 5.0f; // max: circle w/ r = 5 pixels
  auto detector{SimpleBlobDetector::create(params)};
  Sys::Vec<KeyPoint> keypoints;
  detector->detect(img_thresh, keypoints);
  // Convert keypoints (objects found by detector) into x,y coordinates
  dot_coords_.resize(keypoints.size());
  printf("%zu dots found!\n", keypoints.size());
  for (int i_dot{0}; i_dot < keypoints.size(); i_dot++) {
    dot_coords_[i_dot].resize(2);
    dot_coords_[i_dot][0] = keypoints[i_dot].pt.x;
    dot_coords_[i_dot][1] = keypoints[i_dot].pt.y;
  }
  // Overlay fit onto original raw img and write to file
  Mat img_with_keypoints;
  drawKeypoints(img_raw, keypoints, img_with_keypoints, Scalar(0, 0, 255),
                DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  imwrite("fit.jpg", img_with_keypoints);
}

void Curator::OutputData() {

  std::ofstream data_file;
  if (teams_[i_team_] == "Albus") {
    data_file.open("data.out");
    for (auto const &coords : dot_coords_) {
      double x_pix{std::fabs(coords[0] * pixel_size_ - img_center_[0])};
      double y_pix{std::fabs(coords[1] * pixel_size_ - img_center_[1])};
      data_file << x_pix << x_pix;
    }
    printf("Distance to center (in mm) stored in data.out; unravel with dims = "
           "n_dots x 2\n");
  } else if (teams_[i_team_] == "Bellatrix") {
    data_file.open("data.csv");
    data_file << "X (mm), Y (mm) \n";
    for (auto const &coords : dot_coords_) {
      double x_pix{coords[0] * pixel_size_};
      double y_pix{coords[1] * pixel_size_};
      data_file << std::to_string(x_pix) << ", " << std::to_string(y_pix)
                << "\n";
    }
    printf("Coordinates of center of dots (in mm) output to data.csv.\n");

  } else if (teams_[i_team_] == "Cedric") {
    data_file.open("data.tsv");
    data_file << "X (mm)\tY (mm) \n";
    for (auto const &coords : dot_coords_) {
      double x_pix{coords[0] * pixel_size_};
      double y_pix{coords[1] * pixel_size_};
      data_file << std::to_string(x_pix) << "\t" << std::to_string(y_pix)
                << "\n";
    }
    printf("Coordinates of center of dots (in mm) output to data.tsv.\n");
  }
  data_file.close();
}