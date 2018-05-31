#include "menu_client.hpp"
#include "input_client.hpp"
#include "network_client.hpp"
#include "../common/network_opcodes.hpp"

void menu_temp(sf::RenderWindow& win, sf::UdpSocket& socket, std::vector<Player>& players,
               const sf::IpAddress& remote_ip, unsigned short remote_port, bool& quit, bool& quit_game)
{
    bool quit_menu = false;
    sf::Event event;
    sf::Packet send_packet;
    sf::Packet receive_packet;
    sf::IpAddress incomming_ip;
    unsigned short incomming_port;

    //temp wysyłanie prośby o dołączenie do gry
    send_packet<<(sf::Uint8)CLIENT_JOIN_GAME;
    socket.send( send_packet, remote_ip, remote_port );
    send_packet.clear();

    while( !quit_menu )
    {
        while ( !socket.receive( receive_packet, incomming_ip, incomming_port ) )
            network_menu_packet_receive( receive_packet, players, quit_menu );

        while( win.pollEvent( event ) )
            input_menu_receive( event, quit_menu, quit, quit_game );

        win.clear();
        win.display();

        socket.send( send_packet, remote_ip, remote_port );
        send_packet.clear();
    }
}