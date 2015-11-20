//
//  main.cpp
//  Socket_poco_receive
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
#include <netinet/in.h>
#include <netdb.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void socket_sender(){
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8089;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8089);
    
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
    Mat  img = Mat::zeros(480, 960, CV_8UC3);
    namedWindow( "Server", CV_WINDOW_AUTOSIZE );// Create a window for display.
    
    while(1){
        if (newsockfd < 0)
            error("ERROR on accept");
        bzero(buffer,1024);
        
        ssize_t  imgSize = img.total()*img.elemSize();
        uchar sockData[imgSize];
        ssize_t bytes = 0;
        
        for (int i = 0; i < imgSize; i += bytes) {
            if ((bytes = recv(newsockfd, sockData +i, imgSize-i, 0)) == -1) {
                cout << "recv failed";
                return;
            }
        }
        
        int ptr=0;
        for (int i = 0;  i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                img.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
                ptr=ptr+3;
            }
        }
        
        imshow("Server", img);
        cv::waitKey(10);
    }
    
    close(newsockfd);
    close(sockfd);
    return;
}

int main(int argc, char *argv[])
{
    socket_sender();
    return 0;
}