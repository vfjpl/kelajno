#include "player.hpp"
#include <iostream>

Player::Player() {}

Player::Player(sf::IpAddress ip, unsigned short port)
{
    m_ip = ip;
    m_port = port;
}

void Player::set_ip_port(sf::IpAddress ip, unsigned short port)
{
    m_ip = ip;
    m_port = port;
}

void Player::set_nickname(const std::wstring& nickname)
{
    m_nickname = nickname;
}

void Player::set_ready_status(bool ready_status)
{
    m_ready_status = ready_status;
}

const std::wstring& Player::get_nickname() const
{
    return m_nickname;
}

sf::IpAddress Player::get_ip() const
{
    return m_ip;
}

unsigned short Player::get_port() const
{
    return m_port;
}

bool Player::get_ready_status() const
{
    return m_ready_status;
}

bool Player::compare(sf::IpAddress ip, unsigned short port) const
{
    if(m_port == port)
        if(m_ip == ip)
            return true;

    return false;
}

void Player::debug_show_size() const
{
    //keep up to date!
    std::cout << sizeof(m_nickname) << "\n"
              << sizeof(m_ip) << "\n"
              << sizeof(m_port) << "\n"
              << sizeof(m_ready_status) << "\n";
}
