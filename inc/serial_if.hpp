/**
 * @file serial_if.hpp
 * @author Matthew Cockburn 
 * @brief 
 * @version 1
 * @date 2023-10-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/thread/condition_variable.hpp>

#define SERIAL_IF_BUFFER_MAX 2000
#define SERIAL_IF_QUEUE_SIZE 500

class CountingSemaphore{
    private:
    uint8_t myCount;
    std::mutex myMutex;
    std::condition_variable myCondition;

    public:
    /**
     * @brief Construct a new Counting Semaphore object
     * 
     * @param count 
     */
    CountingSemaphore(uint8_t count);
    /**
     * @brief 
     * 
     */
    void acquire(void);
    /**
     * @brief 
     * 
     */
    void release(void);
    /**
     * @brief 
     * 
     * @return uint8_t 
     */
    uint8_t count(void);
};

class serial_if
{
private:
    boost::asio::io_context& m_ctx;
    boost::lockfree::spsc_queue<std::vector<uint8_t>>& m_queue;
    //boost::condition_variable &m_cond;
    boost::asio::serial_port m_port;
    boost::asio::streambuf m_serialData;
    CountingSemaphore readCount;

    void processData(const boost::system::error_code &ec, const uint8_t *buf, std::size_t size);
    void read(void);
public:
    /**
     * @brief Construct a new serial if object
     * @param ctx the boost::asio:io_context
     */
    serial_if(boost::asio::io_context &ctx,boost::lockfree::spsc_queue<std::vector<uint8_t>>& queue);
    /**
     * @brief Configure and start the serial port
     * @param device 
     */
    void start(const std::string &device);
    /**
     * @brief function call to write to the serial interface
     * @warning this function does not and are return character
     * @param buffer of data to be written
     */
    void write(const std::string &buffer);

};
