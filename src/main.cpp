/**
 * @file main.cpp
 * @author 
 * @brief 
 * @version 1
 * @date 2023-09-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <csignal>
#include <thread>
#include <queue>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "ros_if.h"
#include "serial_if.hpp"

void readerThread(boost::asio::io_context &ctx, boost::lockfree::spsc_queue<std::vector<uint8_t>> *queue){
    serial_if serial(ctx,*queue);
    serial.start("/dev/ttyS3");
    ctx.run();
}

void displayThread(boost::lockfree::spsc_queue<std::vector<uint8_t>> *queue){

    std::vector<uint8_t> msg;

    while(1){
       if(!queue->empty()){
            msg = queue->front();
            queue->pop();
            std::cout << msg.data() << std::endl;
       }
    }
    return;
}

int main (void){

    // define variables for controller agent
    boost::asio::io_context ctx;
    boost::condition_variable cond;
    boost::lockfree::spsc_queue<std::vector<uint8_t>> queue(100);
        
    boost::thread thread1(readerThread, std::ref(ctx), &queue);
    boost::thread thread2(displayThread, &queue);
    
    thread1.join();
    thread2.join();

    return 0; 
}
