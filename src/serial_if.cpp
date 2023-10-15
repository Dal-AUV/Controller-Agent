/**
 * @file serial_if.cpp
 * @author Matthew Cockburn 
 * @brief This file contains the implementation for the 
 * serial interface class, this class is capable of 
 * asynchronously reading and writing data to a serial 
 * interface
 * @version 1
 * @date 2023-10-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>

#include "defs.h"
#include "serial_if.hpp"

/* Local Prototypes*/
void readCallback(serial_if * serial);
static void writer(serial_if * serial);

/* Serial Interface Public Methods */
serial_if::serial_if(std::string port_name):
    port(port_name),io(),serial(io, port_name),
    readerQueue(SERIAL_IF_QUEUE_SIZE)//, writerQueue(50)
{
    using namespace std;
    using namespace boost::asio;
    // Acquire the start semaphore 
    sem_init(&startSem,0,0);   
    // Start the threads for reading and writing to the serial port
    readerThread = thread([this] { reader(); });
   // writerThread = thread(writer,this);
}

serial_if::~serial_if(void){
    return;
}

void serial_if::run(void){
    sem_post(&startSem);
    return;
}

void serial_if::end(void){
    
    readerThread.join();

    return;
}

bool serial_if::write(uint8_t * buf, size_t size){    
    assert(buf);

    if(size == 0){
        return false;
    }

    if(boost::asio::write(serial, boost::asio::buffer(buf, size)) != size){
        return false;
    }else{
        return true;
    }
}
/* Serial Interface Private Methods*/
/**
 * @brief This is the reader thread for the serial interface
 * @note This thread will wait until the run method of this 
 * class is called before reading data from the serial port
 * @note Data will be sent using the readQueue in a uint8_t 
 * format
 * @warning Only the SIGINT signal will gracefully end this thread 
 */
void serial_if::reader(void){
    using namespace std;
    using namespace boost::asio;

    constexpr size_t max_buffer_size = 256;
    uint8_t data[max_buffer_size];
    std::cout << "Starting Reader Thread" << std::endl;
    try{
        /* Wait until the serial interface is started */
        sem_wait(&startSem);
        /* Loop until we catch a werewolf */
        while(!SliverBullet){
            async_read(serial, buffer(data, max_buffer_size),
                [&](const boost::system::error_code& ec, size_t bytes){
                    if(!ec){
                        std::vector<uint8_t> receivedData(data, data+ bytes);
                        for( size_t i = 0; i <bytes; ++i){
                            readerQueue.push(data[i]);
                            cout << "Read byte: " << data[i] << endl;
                        }
                    }else{
                        std::cerr << "Error in async_read: " << ec.message() << std::endl;
                    }
                }
            );
            io.run();
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in reader thread: " << e.what() << endl;
    }
    return;
}
/* Local Methods */
