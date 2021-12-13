#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using std::vector;

void extract_frames(const std::string &videoPath, vector<Mat> &frames) {
    try {
        VideoCapture cap(videoPath);
        if (!cap.isOpened()) {
            throw std::runtime_error("Could not open video file: " + videoPath);
        }
        for (int numFrames = 0; numFrames < cap.get(CAP_PROP_FRAME_COUNT); numFrames++) {
            Mat frame;
            cap >> frame;
            frames.push_back(frame);
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
}

void save_frames(vector<Mat> &frames, const std::string& outDir, bool png) {
    vector<int> compressionParams;
    if (png) {
        compressionParams.push_back(IMWRITE_PNG_COMPRESSION);
        compressionParams.push_back(9);
    } else {
        compressionParams.push_back(IMWRITE_JPEG_QUALITY);
        compressionParams.push_back(100);
    }

    for (std::vector<Mat>::iterator frame = frames.begin(), frameNum = 0; frame != frame.end(); ++frame) {
        std::string filePath = outDir + std::to_string(static_cast<long long>(frameNum)) + png ? ".png" : ".jpg";
        imwrite(filePath, *frame, compressionParams);
    }
}

int main() {
    std::cout << "Are you splitting or joining? (s/j): " << std::endl;
    char input;
    std::cin >> input;
    if (input == 's') {
        std::cout << "Enter the name of the mp4 file: ";
        std::string fileName;
        std::cin >> fileName;
        std::cout << "Enter the name of the output directory: ";
        std::string outputDir;
        std::cin >> outputDir;
        
        std::cout << "What type of file format? .png or .jpg: ";
        std::string fileFormat;
        std::cin >> fileFormat;
        

        std::ofstream config(outputDir + "config.ini");

        vector<Mat> frames;
        extract_frames(fileName, frames);
        save_frames(frames, outputDir, true);
        std::cout << "Finished. Press any key to exit..." << std::endl;
        config.close();
        return 0;
    } else if (input == 'j'){
        std::cout << "Enter the name of the directory: ";
        std::string dirName;
        std::cin >> dirName;
        std::cout << "Enter the name of the output file: ";
        std::string outputFile;
        std::cin >> outputFile;

        //Loop through directory and put file names into vector
        vector<Mat> images;
        for (const auto &entry : std::filesystem::directory_iterator(dirName)) {
            if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png") {
                images.push_back(imread(entry.path().string()));
            }
        }

        Size S = vector[0].getSize();

        //int ex = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));

        VideoWriter outputVideo;  // Open the output
        outputVideo.open(outputFile, -1, 30, S, true);  //30 for 30 fps

        if (!outputVideo.isOpened()){
            std::cout  << "Could not open the output video for write: "<< std::endl;
            return -1;
        }

        for(int i=0; i<images.size(); i++){
            outputVideo << images[i];
        }

        std::cout << "Finished writing" << std::endl;
        return 0;
    }
}

