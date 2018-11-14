#include "menu.hpp"
#include "engine.hpp"
#include "resources_manager.hpp"
#include "network_data.hpp"
#include <iostream>

extern Client_Engine engine;
extern Resources_Manager resources_manager;
extern Network_Data server;

void Menu::init()
{
    m_background.setTexture(resources_manager.get_texture(7));
}

void Menu::clear()
{
    m_state = 0;
    m_buttons.clear();
    m_texts.clear();
}

void Menu::main_menu()
{
    m_state = 1;
    m_buttons.clear();
    m_texts.clear();

    m_buttons.emplace_back(L"CONNECT", resources_manager.get_font());
    m_buttons.emplace_back(L"OPTIONS", resources_manager.get_font());
    m_buttons[1].move(0, 30);
    m_buttons.emplace_back(L"AUTHORS", resources_manager.get_font());
    m_buttons[2].move(0, 60);
    m_buttons.emplace_back(L"QUIT", resources_manager.get_font());
    m_buttons[3].move(0, 90);
}

void Menu::connect_menu()
{
    m_state = 2;
    m_buttons.clear();
    m_texts.clear();

    m_buttons.emplace_back(server.get_ip().toString(), resources_manager.get_font());
    m_buttons.emplace_back(L"CONNECT", resources_manager.get_font());
    m_buttons[1].move(0, 30);
    m_buttons.emplace_back(L"BACK", resources_manager.get_font());
    m_buttons[2].move(0, 60);
}

void Menu::options_menu()
{
    m_state = 3;
    m_buttons.clear();
    m_texts.clear();

    m_buttons.emplace_back(L"FULLSCREEN", resources_manager.get_font());
    m_buttons.emplace_back(L"WINDOWED", resources_manager.get_font());
    m_buttons[1].move(0, 30);
    m_buttons.emplace_back(L"BACK", resources_manager.get_font());
    m_buttons[2].move(0, 60);
}

void Menu::authors_menu()
{
    m_state = 4;
    m_buttons.clear();
    m_texts.clear();

    m_texts.emplace_back(L"Kacper Piwiński", resources_manager.get_font());
    m_texts.emplace_back(L"Radosław Wojdak", resources_manager.get_font());
    m_texts[1].move(0, 30);

    m_buttons.emplace_back(L"BACK", resources_manager.get_font());
    m_buttons[0].move(0, 60);
}

void Menu::mouse_click(const sf::Event& event)
{
    sf::Uint8 button_id = get_button_id_from_press(event);
    switch(m_state)
    {
    case 1://main menu
    {
        switch(button_id)
        {
        case 0://connect
        {
            connect_menu();
            break;
        }
        case 1://options
        {
            options_menu();
            break;
        }
        case 2://authors
        {
            authors_menu();
            break;
        }
        case 3://quit
        {
            engine.quit_engine();
            break;
        }
        }//end switch
        break;
    }
    case 2://connect menu
    {
        switch(button_id)
        {
        case 0://IpAddress
        {
            m_texts[0].setString(std::wstring());
            break;
        }
        case 1://connect
        {
            server.set_ip(sf::IpAddress(m_texts[0].getString()));
            engine.connect_to_lobby();
            break;
        }
        case 2://back
        {
            main_menu();
            break;
        }
        }//end switch
        break;
    }
    case 3://options
    {
        switch(button_id)
        {
        case 0://fullscreen
        {
            engine.setup_window(true);
            break;
        }
        case 1://windowed
        {
            engine.setup_window(false);
            break;
        }
        case 2://back
        {
            main_menu();
            break;
        }
        }//end switch
        break;
    }
    case 4://authors menu
    {
        switch(button_id)
        {
        case 0://back
        {
            main_menu();
            break;
        }
        }//end switch
        break;
    }
    }//end switch
}

void Menu::mouse_move(const sf::Event& event)
{
    for(sf::Uint8 i = 0; i < m_buttons.size(); ++i)
        if(m_buttons[i].getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
            m_buttons[i].setFillColor(sf::Color::Red);
        else
            m_buttons[i].setFillColor(sf::Color::White);
}

void Menu::text_entered(const sf::Event& event)
{
    if(m_state == 2)
    {
        std::wstring str(m_texts[0].getString());
        str.push_back(event.text.unicode);
        m_texts[0].setString(str);
    }
}

void Menu::logic(sf::Vector2i mouse_position)
{
    for(sf::Uint8 i = 0; i < m_buttons.size(); ++i)
        if(m_buttons[i].getGlobalBounds().contains(mouse_position.x, mouse_position.y))
            m_buttons[i].setFillColor(sf::Color::Red);
        else
            m_buttons[i].setFillColor(sf::Color::White);
}

void Menu::draw(sf::RenderWindow& window)
{
    window.draw(m_background);
    for(sf::Uint8 i = 0; i < m_buttons.size(); ++i)
        window.draw(m_buttons[i]);
    for(sf::Uint8 i = 0; i < m_texts.size(); ++i)
        window.draw(m_texts[i]);
}

sf::Uint8 Menu::get_button_id_from_press(const sf::Event& event) const
{
    for(sf::Uint8 i = 0; i < m_buttons.size(); ++i)
        if(m_buttons[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
            return i;

    return m_buttons.size();
}

void Menu::debug_show_size() const
{
    //keep up to date!
    std::wcout << sizeof(m_background) << L'\n'
               << sizeof(m_buttons)<< L'\n'
               << sizeof(m_texts)<< L'\n'
               << sizeof(m_state) << L'\n';
}
