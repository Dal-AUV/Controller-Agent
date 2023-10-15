/**
 * @file controller_agent.hpp
 * @author Matthew Cockburn 
 * @brief This header file contains the definitions 
 * for the controller agent class
 * @version 1
 * @date 2023-10-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "serial_if.hpp"

typedef 

class controller_agent
{
private:
    serial_if serial;
public:
    controller_agent(std::string port);
    ~controller_agent();
};

