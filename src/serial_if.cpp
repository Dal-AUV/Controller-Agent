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
#include <iomanip>
#include "defs.h"
#include "serial_if.hpp"


CountingSemaphore::CountingSemaphore(uint8_t count){
    myCount = count;
}

void CountingSemaphore::acquire(void){
    
    std::unique_lock<std::mutex> lock(myMutex);
    
    while(myCount == 0){
        myCondition.wait(lock);
    }
    
    myCount--;
    
    return;
}

void CountingSemaphore::release(void){
    
    std::unique_lock<std::mutex> lock(myMutex);
    myCount++;
    myCondition.notify_one();

    return;
}

uint8_t CountingSemaphore::count(void){
    return myCount;
}

/* Serial Interface Public Methods */
serial_if::serial_if(boost::asio::io_context &ctx, boost::lockfree::spsc_queue<std::vector<uint8_t>> &queue):
    m_ctx(ctx),
    m_port(ctx),
    m_queue(queue),
    readCount(0)
{
    return;
}

void serial_if::start(const std::string &device)
{
    m_port.open(device);
    m_port.set_option(boost::asio::serial_port::baud_rate(115200));
    m_port.set_option(boost::asio::serial_port::character_size(8));
    m_port.set_option(boost::asio::serial_port::parity(boost::asio::serial_port_base::parity::none));
    m_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    read();
    return;
}

void serial_if::read()
{
    boost::asio::async_read(
        m_port,
        m_serialData,
        boost::asio::transfer_at_least(1),
        [this](const boost::system::error_code &ec,std::size_t size) 
        { processData(ec, boost::asio::buffer_cast<const uint8_t*>(m_serialData.data()), size); }
    );
}

void serial_if::processData(const boost::system::error_code &ec, const uint8_t *buf, std::size_t size)
{
    // TODO [M.C]: Determine how to properly error handle
    if (ec) return;
    
    // Print or handle the received data
    std::vector<uint8_t> data(buf, buf +  size);
    std::cout << "Data: " << data.data() << " Size: " << size << std::endl;

    for (const auto& value : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(value) << " ";
    }

    std::cout << std::endl;


    m_serialData.consume(m_serialData.size());

    read();

}
