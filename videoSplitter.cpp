#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iterator>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using std::vector;

void find(std::vector<int>::iterator& iter, std::vector<int>::iterator end, int test)
{
    while (iter != end) { 
        if (*iter == test)
            return;
        ++iter;
    }
}

/*
This functions opens a video file and extracts the frames and put them into a vector of Mat(its the class for representing an img)
*/
void extract_frames(const std::string &videoFilePath,vector<Mat>& frames){
	
	try{
		//open the video file
  	VideoCapture cap(videoFilePath); // open the video file
  	if(!cap.isOpened())  // check if we succeeded
  		std::cout << "Cannot open video file" << std::endl;
	
  	//cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
  	for(int frameNum = 0; frameNum < cap.get(CAP_PROP_FRAME_COUNT);frameNum++)
  	{
  		Mat frame;
  		cap >> frame; // get the nCV_Errorext frame from video
  		frames.push_back(frame);
  	}
  }
  catch( cv::Exception& e ){
    std::cerr << e.msg << std::endl;
    exit(1);
  }
}

/*
It saves a vector of frames into jpg images into the outputDir as 1.jpg,2.jpg etc where 1,2 etc represents the frame number
*/
void save_frames(std::vector<Mat>& frames, const std::string& outputDir, bool png) {
  std::vector<int> compression_params;
  compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
    int frameNumber=0;
	for(std::vector<Mat>::iterator frame = frames.begin(); /*find(frame, frames.end(), 5)*/ frame != frames.end() ; ++frame){
	  std::string filePath = outputDir + std::to_string(static_cast<long long>(frameNumber))+ ".jpg";
	  imwrite(filePath,*frame,compression_params);
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
    } /*else if (input == 'j'){
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
    }*/
}

