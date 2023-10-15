/**
 * @file serial_if.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <string>
#include <thread>
#include <queue>
#include <semaphore.h>
#include <boost/asio.hpp>
#include <boost/lockfree/spsc_queue.hpp>

#define SERIAL_IF_BUFFER_MAX 2000
#define SERIAL_IF_QUEUE_SIZE 500

class serial_if
{
private:
    std::string port;
    std::thread readerThread;
    std::thread writerThread;
    sem_t startSem;
    boost::asio::io_service io;
    boost::asio::serial_port serial;

    //boost::lockfree::spsc_queue<uint8_t> writerQueue;
    void reader(void);

public:
    boost::lockfree::spsc_queue<uint8_t> readerQueue;
    /**
     * @brief Construct a new serial_if::serial_if object
     * @param port_name - std::string name of the serial port 
     */
    serial_if(std::string port_name);
    /**
     * @brief Destroy the serial_if object
     */
    ~serial_if();
    /**
    * @brief Start the serial interface backend services
    * @warning This will spawn an additional threads
    */
    void run(void);
    /**
    * @brief End the serial interface backend services
    */
    void end(void);
    /**
     * @brief Write a buffer to the serial interface
     * @param buffer - ptr of uint8_t buffer
     * @param size - size of the buffer
     * @return true - success
     * @return false - failure
     */
    bool write(uint8_t * buffer, size_t size);
};
