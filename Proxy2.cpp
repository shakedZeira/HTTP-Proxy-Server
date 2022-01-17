// Proxy2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vbuffer(1 * 1024);

void GrabData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vbuffer.data(), vbuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";
                for (int i = 0; i < length; i++)
                {
                    std::cout << vbuffer[i];
                }
                GrabData(socket);
            }
        }
    );
}

int main()
{
    asio::error_code ec;

    asio::io_context context;

    asio::io_context::work fakework(context);
    
    std::thread threadContext = std::thread([&]() { context.run(); });

    asio::ip::tcp::endpoint Server(asio::ip::make_address("127.0.0.1", ec),4200);

    asio::ip::tcp::socket socket(context);

    socket.connect(Server, ec);

    if (!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Couldnt connect\n" << ec.message() << std::endl;
    }
    if (socket.is_open())
    {
        GrabData(socket);
        std::string sRequest = "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

   
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);

        context.stop();
        if (threadContext.joinable())
        {
            threadContext.join();
        }
       
    }
    return 0;
}
