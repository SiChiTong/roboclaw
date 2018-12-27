/**
 *
 * Copyright (c) 2018 Carroll Vance.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef PROJECT_ROBOCLAWDRIVER_H
#define PROJECT_ROBOCLAWDRIVER_H

#include <string>
#include <map>
#include <exception>

#include <boost/asio.hpp>

namespace roboclaw {

    class driver {

    public:
        driver(std::string &port);

        ~driver();

        void set_baud(unsigned int baudrate);

        std::string get_version(unsigned char address);
        std::pair<int, int> get_encoders(unsigned char address);
        std::pair<int, int> get_velocity(unsigned char address);

        void set_speed(unsigned char address, std::pair<int, int> speed);

        void reset_encoders(unsigned char address);

        static unsigned char BASE_ADDRESS;
        static unsigned int DEFAULT_BAUDRATE;

    private:
        std::shared_ptr<boost::asio::serial_port> serial;
        boost::asio::io_service io;

        void init_serial(std::string &port, unsigned int baudrate = DEFAULT_BAUDRATE);

        static unsigned int crc16(unsigned char *packet, size_t nBytes);

        size_t txrx(unsigned char address, unsigned char command, unsigned char *tx_data, size_t tx_length,
                           unsigned char *rx_data, size_t rx_length, bool tx_crc = false, bool rx_crc = false);


    };

    class timeout_exception : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

// trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

// trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
}
#endif //PROJECT_ROBOCLAWDRIVER_H
