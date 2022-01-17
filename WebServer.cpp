#include "WebServer.h"
#include <string>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <vector> 
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#define BufSize MAX_PATH


void WebServer::onClientConnected(int clientSocket)
{
}

void WebServer::onClientDisconnected(int clientSocket)
{

}
void WebServer::onMessageRecived(int clientSocket, const char* msg, int length)
{
	
	// GET /index.html HTTP/1.1
	//parse out the document requested
	std::istringstream iss(msg);
	std::cout << "msg from client :  \r\n" << msg;
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	std::string content = "<h2>404 Not Found</h2>";
	SetCurrentDirectoryA("D:\\angular\\my-app\\src\\app\\");
	std::string HtmlFile = "/app.component.html";
	int errorcode = 404;
	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		//check current directory
		/*
		char Buffer[BufSize];
		DWORD dret;
		GetCurrentDirectoryA(BufSize, Buffer);
		std::cout << "direcotryyyyy" << Buffer << std::endl;
		*/
		//open the document in the local file system
		HtmlFile = parsed[1];
		std::cout << "parsed[1]: " << parsed[1] << "\r\n";
		if (HtmlFile == "/")
		{
			HtmlFile = "/app.component.html";
		}
	}
	
	std::ifstream f("..\\app" + HtmlFile);
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		content = str;
		errorcode = 200;
	}
	f.close();

	std::string type = "text/html";
	if (parsed[1] == "/styless.css")
	{
		type = "text/css";
	}
	if (parsed[1] == "/main.js")
	{
		type = "text/javascript";
	}
	if (parsed[1] == "/image.png")
	{
		type = "image/png";
	}

	
	if (parsed.size() >= 3)
	{
		if (parsed[0] == "GET")
		{
			//write the document back to the client
			std::ostringstream oss;
			oss << "HTTP/1.1 " << errorcode << "OK\r\n";
			oss << "Cache-Control: no-cache , private\r\n";
			oss << "Content-Type: "<< type << "\r\n";
			oss << "Content-Length: " << content.size() << "\r\n";
			//to work on::::::::::::::::::::::::::::::::::::: all
			oss << "\r\n";
			oss << content;
			std::string output = oss.str();
			std::cout << "output: " << output;
			int size = output.size() + 1;
			std::cout << "content: \r\n" << content;
			sendtoClient(clientSocket, output.c_str(), size);
		}
		else if (parsed[0] == "POST" && parsed[1] == "/button")
		{
			std::string input;
			std::cout << "please enter what you would like to post: ";
			std::cin >> input;
			std::ostringstream postreq;
			postreq << "<h1>" << input << "</h1>";
			std::string str_postreq;
			str_postreq = postreq.str();
			//write the document back to the client
			std::ostringstream oss;
			oss << "HTTP/1.1 " << errorcode << "OK\r\n";
			oss << "Cache-Control: no-cache , private\r\n";
			oss << "Content-Type: text/" << type << "\r\n";
			oss << "Content-Length: " << str_postreq << "\r\n";
			//to work on::::::::::::::::::::::::::::::::::::: all
			oss << "\r\n";
			oss << str_postreq;
			std::string output = oss.str();
			std::cout << "output: " << output;
			int size = output.size() + 1;
			std::cout << "content: \r\n" << content;
			sendtoClient(clientSocket, output.c_str(), size);
		}
	}
	
}
