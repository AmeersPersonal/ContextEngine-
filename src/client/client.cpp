//
// Created by skill on 11/15/2025.
//
#pragma once

#include "client.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <vector>
#include <thread>
#include <ws2tcpip.h>
#include <SFML/Graphics.hpp>
#include<SFML/Window/Event.hpp>

#pragma comment (lib, "Ws2_32.lib")


const int MAX_CONNECTIONS = 10;
const int FPS = 30;



//constructor & de constructor
//sets up the GLI
client::client(const std::string & ip, int port):
ip(ip),
port(port),
socket2(INVALID_SOCKET),
window(sf::VideoMode(), "Client"),
texture(),
sprite(texture),
status(false)

{
    renderWindow.create(sf::VideoMode::getDesktopMode(), "Client");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed" << std::endl;
        exit(1);
    }

}


client::~client() {
    WSACleanup();
}

//runs the client
void client::run() {
}
//updates the frames
void client::update() {
}
//todo meant for debugging and letenct checking
const char * client::lastUpdate() {
    return "";
}

bool client::packet_handler() {
    const std::string frame_req = "REQUEST_FRAME";
    // send error handling
    if(send(socket2, frame_req.c_str(), frame_req.length(), 0) == SOCKET_ERROR) {
        return false;
    }
    uint32_t image_size;
    if(recv(socket2, reinterpret_cast<char*>(&image_size), sizeof(image_size), 0) != sizeof(image_size)) {
        return false;
    }

    image_size = ntohl(image_size);
    std::vector<char> buffer(image_size);

    uint32_t total_recived=0;

    while(total_recived < image_size) {
        int bytes = recv(socket2, buffer.data() + total_recived, image_size - total_recived, 0);
        if(bytes <=0) {
            return false;
        }
        total_recived += bytes;

    }
    return true;

}


void client::send_packet(const void *data, size_t size) {
    if(this->get_socket_status()) {
        std::cerr << "socket not connected" << std::endl;
    }

    int results = send(socket2, static_cast<const char*>(data), static_cast<int>(size), 0);
    if(results == SOCKET_ERROR) {
        std::cerr << "send failed" << std::endl;
    }


}

void client::send_mouse_button_event(sf::Mouse::Button button, sf::Vector2i postion, bool pressed) {
    client::MouseButtonPaket packet;
    packet.type = 0x02;
    packet.button = static_cast<uint8_t>(button);
    packet.x = postion.x;
    packet.y = postion.y;
    packet.pressed = pressed;
    client::send_packet(&packet, sizeof(packet));

}

void client::send_mouse_move_event(sf::Vector2i postion) {
    client::MouseMovePaket packet;
    packet.type = 0x03;
    packet.x = postion.x;
    packet.y = postion.y;
    client::send_packet(&packet, sizeof(packet));

}

void client::send_mouse_wheel_event(float delta, sf::Mouse::Wheel wheel) {
    client::MouseWheelPaket packet;
    packet.type = 0x04;
    packet.delta = delta;
    packet.wheel = static_cast<uint8_t>(wheel);
    client::send_packet(&packet, sizeof(packet));
}

void client::send_text_input_event(uint32_t unicode) {
    client::TextInputEventPacket packet;
    packet.type = 0x05;
    packet.unicode = unicode;
    client::send_packet(&packet, sizeof(packet));

}

void client::send_key_event(sf::Keyboard::Key code, sf::Keyboard::Scancode scancode, bool pressed, bool alt,
    bool control, bool shift, bool system) {
    client::KeyEventPacket packet;
    packet.type = 0x01;
    packet.keycode = static_cast<uint32_t>(code);
    packet.scankey = static_cast<uint32_t>(scancode);
    packet.pressed= pressed;
    packet.control = control;
    packet.alt = alt;
    packet.shift = shift;
    packet.system = system;
    client::send_packet(&packet, sizeof(packet));


}

bool client::decrompess_image() {
    return true;
}

int client::recieve_all_pakets(char* buffer, size_t len) {
    size_t total_size=0;

    while (total_size < len) {
        int current_size = recv(socket2, buffer+total_size, len-total_size, 0);
        if (current_size <= 0) {
            if (current_size == 0)
                return 0;
            if(current_size = -1) {
                return -1;
            }
        }
        total_size += current_size;

        

    }
    return total_size;
}

// std::map<std::string, std::string> client::connected_devices() {
//     //TODO: will implement when I sucessfully connect to the pi %
//
// }

// client-end status might change it to an int to show on/off/suspened/disconnected

bool client::get_socket_status() const {
    return socket2 != INVALID_SOCKET;
}

bool client::get_status() const {
    return this->status;
}
void client::set_status(bool val) {
    this->status = val;
}

bool client::connect(const char *name) {
    struct addrinfo;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "socket() failed" << std::endl;
        client::set_status(false);
        return false;

    }
    struct sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    if(::connect(sock, reinterpret_cast<sockaddr *>(&server), sizeof(server)) == SOCKET_ERROR) {
        client::set_status(false);
        return false;

    }
    client::set_status(true);
    return true;


}

void client::sfml_handler() {
    while(window.isOpen())// only runs if the windows is open
    {
        while (const std::optional<sf::Event> event = window.pollEvent()) { //loops through events
            if (event->is<sf::Event::Closed>()){ //close event
                window.close();
            }
            //TODO: still have to add the packet handlers
            //keyboard events

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                continue;
                /*TODO: experminet with this when the server side is finished
                 *
                 *
                */sf::Keyboard::Key pressed = keyPressed->code;

            }
        }
    }

}



