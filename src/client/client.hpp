//
// Created by skill on 11/15/2025.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <winsock.h>

#define CLIENT_SOCKET SOCKET
class client {

    public:
    client(const std::string& ip, int port);
    ~client();
    void run();
    bool get_status() const;
    void set_status(bool var);

    void update();
    const char* lastUpdate();
    #pragma pack(push, 1)
    struct KeyEventPacket {
        //the type we send to the sever will allow us to differnate from key and mouse paket for the send packet event
        uint8_t type = 0x01;
        uint32_t scankey;
        uint32_t keycode;
        bool alt;
        bool control;
        bool shift;
        bool pressed;
        // windows has the win button and mac has command and ect that is what this var is for
        bool system;

    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct MouseButtonPaket {
        //mouse event
        uint8_t type = 0x02;
        uint8_t button;
        int32_t x;
        int32_t y;
        bool pressed;

    };
    #pragma pack(pop)
    #pragma pack(push, 1)
    struct MouseMovePaket {
        uint8_t type = 0x03;
        int32_t x;
        int32_t y;

    };
    #pragma pack(pop)
    #pragma pack(push, 1)
    struct MouseWheelPaket {
        uint8_t type = 0x04;
        uint8_t wheel;
        //amount of scrolls
        float delta;

    };
    #pragma pack(pop)
    #pragma pack(push, 1)
    struct TextInputEventPacket {
        uint8_t type = 0x05;
        uint32_t unicode;
    };
    #pragma pack(pop)



    private:
    //connection
    int port;
    const std::string& ip;
    bool status;
    CLIENT_SOCKET socket2;
    //window
    sf::Window window;
    sf::RenderWindow renderWindow;
    sf::Clock clock;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image image;



    // packet methods
    bool packet_handler();
    void send_packet(const void* data, size_t size);

    //mouse event
    void send_mouse_button_event (sf::Mouse::Button button, sf::Vector2i postion, bool pressed);
    void send_mouse_move_event(sf::Vector2i postion);
    void send_mouse_wheel_event(float delta, sf::Mouse::Wheel wheel);

    //keyboard & text event
    void send_text_input_event(uint32_t unicode);
    void send_key_event(sf::Keyboard::Key code, sf::Keyboard::Scancode scancode,
                            bool pressed, bool alt, bool control, bool shift, bool system);



    //core methods
    bool connect(const char* name);
    bool decrompess_image();
    void sfml_handler();
    int recieve_all_pakets(char* buffer, size_t len);
    bool edit_mode = false;
    std::vector<char> frame_buffer;



    std::map<std::string, std::string> connected_devices();

    bool get_socket_status() const;
};



#endif //CLIENT_HPP
