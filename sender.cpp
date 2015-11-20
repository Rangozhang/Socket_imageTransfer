//
//  main.cpp
//  Socket_poco
//
//  Created by 张雨 on 11/19/15.
//  Copyright © 2015 Zhang, Yu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>
#include<errno.h>

using namespace cv;
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void image_receiver(){
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    
    portno = 8089;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("128.237.215.105");
    serv_addr.sin_port = htons(8089);
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    //capture videos
    cv::VideoCapture cap;
    cap.open(0);
    
    if (!cap.isOpened()){
        std::cerr << "Could not open the build-in camera" << endl;
        return;
    }
    
    Mat image;
    
    while(1){
        
        cap >> image;
        //cv::imshow("", image);
        //cv::waitKey(10);
        cv::Mat template_ = Mat::zeros(480, 960, CV_8UC3);
        cv::resize(image, image, template_.size());
        image = (image.reshape(0,1));
        ssize_t imgSize = image.total()*image.elemSize();
        if(! image.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return;
        }
        // Send data here
        send(sockfd, image.data, imgSize, 0);
    }
    
    close(sockfd);
    return;
}

int main(int argc, char *argv[])
{
    image_receiver();
    return 0;
}
