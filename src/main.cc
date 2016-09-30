#include "asio.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <chrono>
#include <utility>
#include <cstdint>

using asio::ip::tcp;

class session
   : public std::enable_shared_from_this<session> {
public:
  session(tcp::socket socket) : socket_(std::move(socket)) {}

  void start() { do_read(); }

private:
   void do_read() {
      auto self(shared_from_this());
      socket_.async_read_some(asio::buffer(data_, max_length),
         [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
               std::istringstream stream(data_);
               auto read_uint16_t = [&stream]() {
                  std::uint16_t result(0);
                  stream >> result >> std::ws;
                  return result;
               };

               auto read_float = [&stream, &read_uint16_t]() {
                  std::uint16_t temp(read_uint16_t());
                  const uint8_t *ptr = reinterpret_cast<uint8_t *>(&temp);
                  float result = ptr[0];
                  result *= 256;
                  result += ptr[1];
                  result *= 0.1;
                  return result;
               };

               std::uint16_t co2(read_uint16_t()), brightness(read_uint16_t());
               float temperature(read_float()), humidity(read_float()), led_brightness(read_float());
               auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

               std::cout << "[" << timestamp << "] Full string: \"" <<  data_ << "\"\n"; 
               std::cout << "CO2: " << co2 << "\n";
               std::cout << "Brightness: " << brightness << "\n";
               std::cout << "Temperature: " << temperature << "\n";
               std::cout << "Humidity: " << humidity << "\n";
               std::cout << "LED Brightness: " << led_brightness << "\n";
            }
         });
   }

   tcp::socket socket_;
   enum { max_length = 32 };
   char data_[max_length];
};

class server {
public:
   server(asio::io_service &io_service, short port)
      : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
      , socket_(io_service) {
      do_accept();
   }

private:
   void do_accept() {
      acceptor_.async_accept(socket_, [this](std::error_code ec) {
      if (!ec) {
         std::make_shared<session>(std::move(socket_))->start();
      }

      do_accept();
      });
   }

   tcp::acceptor acceptor_;
   tcp::socket socket_;
};

int main(int argc, char *argv[]) {
   try {
      if (argc != 2) {
         std::cerr << "Usage: async_tcp_echo_server <port>\n";
         return 1;
      }

      asio::io_service io_service;

      server s(io_service, std::atoi(argv[1]));
      io_service.run();
   } catch (std::exception &e) {
      std::cerr << "Exception: " << e.what() << "\n";
   }

   return 0;
}
