// File Name: main.cpp
// Author: yunzhongke
// Created Time: 2017年11月13日 星期一 10时08分59秒
// application: 单线程与线程组在解析视频上花费的时间 


#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "opencv2/opencv.hpp"

void test1(const char *video , int start , int end) 
{
    cv::VideoCapture *capture = new cv::VideoCapture(video);
    if (!capture->isOpened()){
        capture->release();
        delete capture;
        return ;
    }
    cv::Mat *frame = new cv::Mat();
    int num = start;
    char tmpName[10];
    std::string imgName;
    std::cout << "test1 work .." << std::endl;
    if (num != 0)
        capture->set(CV_CAP_PROP_POS_FRAMES , num);//设置视频帧索引,也就是从哪帧开始
    while(true){
        if (num == end || !capture->read(*frame)){
            std::cout << "num" << num <<  std::endl;
            break;
        }
        memset(tmpName , 0 , sizeof(tmpName));
        sprintf(tmpName , "%08d" , num);
        imgName = "./" + std::string(tmpName) + ".jpg";
        cv::imwrite(imgName , *frame);
        ++num;
    }
    std::cout << "finish!\n";
    delete frame;
    capture->release();
    delete capture;
    capture = nullptr;
}

void test2(const char *video)
{
    cv::VideoCapture *capture = new cv::VideoCapture(video);
    if (!capture->isOpened()){
        capture->release();
        delete capture;
        return ;
    }
    cv::Mat *frame = new cv::Mat();
    int num = 0;
    char tmpName[10];
    std::string imgName;
    std::cout << "test2 work .." << std::endl;
    while(true){

        *(capture) >> *frame;
        if (frame->empty())
            break;
        memset(tmpName , 0 , sizeof(tmpName));
        sprintf(tmpName , "%08d" , num);
        imgName = "./" + std::string(tmpName) + ".jpg";
        cv::imwrite(imgName , *frame);
        ++num;
    }
    std::cout << "finish!\n";
    delete frame;
    capture->release();
    delete capture;
    capture = nullptr;
}

void joinable(std::vector<std::thread> &v)
{
    for (size_t i = 0; i < v.size(); ++i){
        if (v[i].joinable())
            v[i].join();
    }
}

int main(int argc , char** argv)
{
    if (argc != 3){
        std::cout << "Usage: ./a.out videopath options\n";
        return -1;
    }

    if (atoi(argv[2]) == 1){ // 线程组
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        cv::VideoCapture * capture = new cv::VideoCapture(argv[1]);
        if (!capture->isOpened()){
            capture->release();
            delete capture;
            return -1;
        }

        long totalFrameCount = capture->get(CV_CAP_PROP_FRAME_COUNT);
        std::cout << "totalFrameCount: " << totalFrameCount << std::endl;
        int thread_count = std::thread::hardware_concurrency() * 2;
        int part = totalFrameCount / thread_count ;
        std::vector<std::thread> v;
        try{
            for (int i = 0; i < thread_count; ++i){
                if (i == thread_count - 1)
                    v.push_back(std::thread(&test1 , argv[1] , part * i , totalFrameCount + 1));
                else 
                    v.push_back(std::thread(&test1 , argv[1] , part * i , part * (i + 1))); 
            }
        }
        catch(...){
            joinable(v);
            throw ;
        }
        joinable(v);
        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "time : " << diff.count() << std::endl;
    }
    else{ //单线程
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        test2(argv[1]);
        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "time: " << diff.count() << std::endl;
    }

    return 0;
}
