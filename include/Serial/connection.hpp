//
// Created by mateusz on 28.01.2020.
//

#ifndef PROTOCOLCONVERTER_S_CONNECTION_HPP
#define PROTOCOLCONVERTER_S_CONNECTION_HPP

#include <string>
#include <stdexcept>
#include <sstream>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cerrno>
#include <poll.h>

#include "modbusUtils.hpp"
#include "modbusResponse.hpp"
#include "modbusRequest.hpp"
#include "modbusException.hpp"

namespace MB::Serial
{
class Connection
{
public:
    const unsigned int DefaultSerialTimeout = 1000;

private:
    termios _termios;
    int _fd;

    int _timeout = DefaultSerialTimeout;

    /**
     * @brief Sends data through the serial
     * @param data - Vectorized data
     */
    void send(std::vector<uint8_t>&& data);

public:
    constexpr explicit Connection() : _termios(), _fd(-1) {}
    explicit Connection(const std::string& path);
    ~Connection();

    void connect();


    void sendRequest(const MB::ModbusRequest& request);
    void sendResponse(const MB::ModbusResponse& response);
    void sendException(const MB::ModbusException& exception);

    [[nodiscard]] MB::ModbusResponse awaitResponse();
    [[nodiscard]] MB::ModbusRequest awaitRequest();

    void setParity(const bool parity)
    {
        if (parity)
            getTTY().c_iflag |= INPCK;
        else
            getTTY().c_iflag &= ~INPCK;
    }

    void setTwoStopBits(const bool two)
    {
        if (two)
        {
            getTTY().c_oflag |= CSTOPB;
        }
        else
        {
            getTTY().c_oflag &= ~CSTOPB;
        }
    }


#define setBaud(s) case s: speed = B ## s; break;
    void setBaudRate(speed_t speed)
    {
        switch (speed)
        {
            setBaud(0);
            setBaud(50);
            setBaud(75);
            setBaud(110);
            setBaud(134);
            setBaud(150);
            setBaud(200);
            setBaud(300);
            setBaud(600);
            setBaud(1200);
            setBaud(1800);
            setBaud(2400);
            setBaud(4800);
            setBaud(9600);
            setBaud(19200);
            setBaud(38400);
            setBaud(57600);
            setBaud(115200);
            setBaud(230400);
            default:
                throw std::runtime_error("Invalid baud rate");
        }
        cfsetospeed(&_termios , speed);
    }
#undef setBaud

    termios & getTTY()
    {
        return _termios;
    }

    int getTimeout() const
    {
        return _timeout;
    }

    void setTimeout(int timeout)
    {
        _timeout = timeout;
    }
};
}

#endif //PROTOCOLCONVERTER_S_CONNECTION_HPP
