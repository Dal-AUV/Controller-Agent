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
#include <condition_variable>

#include <boost/asio.hpp>

#include "defs.h"
#include "ros_if.h"
#include "serial_if.hpp"

volatile bool SliverBullet = false;
std::queue<std::string> dataQueue;

/* Lets go Werewolves Hunting */
void signalHandler(int signum){
    std::cout << "Exit Signal Caught" << std::endl;
    SliverBullet = 1;
}


int main (void){
    
    std::string port("/dev/ttyS3");
    uint8_t byte = 0;

    std::condition_variable cv;
    std::mutex mtx;

    serial_if serial(port);
    serial.run();
    
    while(!SliverBullet){   
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [&]{return !serial.readerQueue.empty();});

        if(serial.readerQueue.read_available()){
            byte = serial.readerQueue.pop();
            std::cout << "Byte Received: >>" << byte << "<<" << std::endl; 
        }
    }
    serial.end();
    return 0; 
}
// /* Reader Thread */
// void readFromSerial(boost::asio::serial_port& serial, boost::asio::io_service& io) {
//     using namespace std;
//     try {
//         char data[256];
//         boost::system::error_code error;

//         while (!SliverBullet) {
//             boost::asio::async_read(serial,boost::asio::buffer(data,sizeof(data)),
//                 [&](const boost::system::error_code& ec, size_t bytes_transferred){
//                     if(!ec){
//                         string receivedData(data, bytes_transferred);
//                         dataQueue.push(receivedData);
//                     }
//                 });
//             io.run();
//         }
//     }catch (std::exception& e) {
//         cerr << "Exception in read thread: " << e.what() << endl;
//     }
// }

/* Writer Thread */
// void writeToSerial(boost::asio::serial_port& serial) {
//     using namespace std;
//     try {
//         while (true) {
//             string message = "Hello, Serial Port!\n";
//             boost::asio::write(serial, boost::asio::buffer(message));

//             // Sleep for a while before sending the next message
//             this_thread::sleep_for(std::chrono::seconds(1));
//         }
//     } catch (std::exception& e) {
//         cerr << "Exception in write thread: " << e.what() << std::endl;
//     }
// }

// int main() {

//     using namespace boost::asio;
//     using namespace std;

//     signal(SIGINT, signalHandler);

//     io_service io;
//     serial_port serial(io, "/dev/ttyS3");  // Adjust the serial port device path
//     thread readerThread(readFromSerial, ref(serial), ref(io));
//     //std::thread writerThread(writeToSerial, std::ref(serial));

//     while(!SliverBullet){
//         if(!dataQueue.empty()){
//             string receivedData = dataQueue.front();
//             dataQueue.pop();
//             cout << "Received: " << receivedData << endl;
//         }
//     }
//     readerThread.join();
//     //writerThread.join();
//     return 0;
// }