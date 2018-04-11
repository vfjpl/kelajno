#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#ifdef linux
    #include <getopt.h>
#endif // linux

void print_help()
{
    std::cout<<"-p --port       default 7000\n"
               "-l --local_port default 8000\n"
               "-i --ip         default localhost\n"
               "-h --help       this message\n";
}

int main(int argc, char** argv)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //zmienne które można modyfikować argumentami z konsoli
    unsigned short port = 8000;
    unsigned short remote_port = 7000;//port do którego się łączymy
    sf::IpAddress remote_ip = "localhost";//ip do którego się łączymy

    #ifdef linux
    {
        static struct option longopts[] =
        {
            {"port",       required_argument, NULL, 'p'},
            {"local_port", required_argument, NULL, 'l'},
            {"ip",         required_argument, NULL, 'i'},
            {"help",       no_argument,       NULL, 'h'},
        };

        int c;
        while( (c = getopt_long( argc, argv, "p:l:i:h", longopts, NULL )) != -1 )
        {
            switch (c)
            {
            case 'p':
                remote_port = std::stoul( optarg, NULL, 0 );
                break;
            case 'l':
                port = std::stoul( optarg, NULL, 0 );
                break;
            case 'i':
                remote_ip = optarg;
                break;
            case 'h':
                std::cout<<argv[0]<<"\n";
                print_help();
                return 0;
            case '?':
                return 0;
            case ':':
                return 0;
            }
        }
    }
    #endif // linux
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool quit;
    sf::UdpSocket socket;
    socket.bind( port );

    sf::Packet send_packet;
    sf::Packet receive_packet;
    sf::IpAddress incomming_ip;

	int map_width = 32, map_height = 32;
	int number_of_chunks = map_width * map_height;

	sf::RenderWindow oknoAplikacji(sf::VideoMode(1920, 1080), "Kelajno", sf::Style::Fullscreen);//to opcja fullscreen

	sf::Texture tekstura1;//sluzy do wczytywania tekstury bo jest texture i image
	tekstura1.loadFromFile("Textures/Grunt.png");//zwraca true lub false
	sf::Texture tekstura2;
	tekstura2.loadFromFile("Textures/Grunt2.png");
	sf::Texture tekstura3;
	tekstura3.loadFromFile("Textures/Drzewko.png");


	sf::Sprite *obrazek = new sf::Sprite[number_of_chunks];
	for (int i = 0; i < number_of_chunks; i++)
	{
		if (i % 2 == 0)
		{
			obrazek[i].setTexture(tekstura1);
		}
		else
		{
			obrazek[i].setTexture(tekstura2);
		}
	}
	sf::Sprite drzewo;
	drzewo.setTexture(tekstura3);


	sf::FloatRect chunk_size = obrazek[0].getGlobalBounds();//biore info wymiarow chunka na przykladzie pierwszego
	for (int i = 0; i < map_height; i++)
	{
		for (int g = 0; g < map_width; g++)
		{
			obrazek[g + (i*map_width)].setPosition( (oknoAplikacji.getSize().x * 0.5+250) - (g*chunk_size.width),
                                                    (oknoAplikacji.getSize().y * 0.5-250) + (i*chunk_size.height) );
			//podwojna petla "i" dla nr wiersza i "g" dla kolumny
			//do szerokosci dodaje wielokrotnosci kolumnowe i wierszowe dla wysokosci
			//250 na korekte
		}
	}
	drzewo.setPosition((oknoAplikacji.getSize().x * 0.5), (oknoAplikacji.getSize().y * 0.5));

	sf::View v = oknoAplikacji.getDefaultView();//widok ma byc taki jak okno tak jakby ciagnie z niego dane
	v.setSize(v.getSize().x, v.getSize().y * 2);//tak jak przy teksturze skalujemy 2 wieksza wysoksoc
	v.setRotation(45);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    sf::Thread network_thread( [&]()
        {
            while( !quit )
            {
                socket.receive( receive_packet, incomming_ip, port );
            }
        });
    sf::Thread input_thread( [&]()
        {
            sf::Event zdarzenie;
            while( !quit )
            {
                oknoAplikacji.waitEvent(zdarzenie);
                //tu obs�uga zdarze�
                switch (zdarzenie.type)
            	{
            	case sf::Event::Closed:
                    quit = true;
                    break;
            	default:
                    break;
            	}

                switch(zdarzenie.key.code)
                {
            	case sf::Keyboard::Escape:
                    quit = true;
                    break;
            	default:
                    break;
                }

                switch(zdarzenie.mouseButton.button)
                {
                case sf::Mouse::Middle:
                    quit = true;
                    break;
            	default:
                    break;
            	}
            }
        });
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    sf::Time time;
    sf::Clock clock;
    input_thread.launch();//odpalenie obsługi urządzeń wejścia
    network_thread.launch();//odpalenie przyjmowania pakietów

    //tymczasowe
    //wysylanie prosby o dolaczenie do gry
    send_packet << (sf::Uint8)0;//command
    socket.send( send_packet, remote_ip, remote_port );
    send_packet.clear();

	while ( !quit )
	{
        time = clock.restart();//pobranie czasu

		oknoAplikacji.clear();
		oknoAplikacji.setView(v);//ustawia widok
		for (int i = 0; i < number_of_chunks; i++)
		{
			oknoAplikacji.draw(obrazek[i]);//wyswietla chunki
		}
		oknoAplikacji.setView(oknoAplikacji.getDefaultView());//zeby zrzutowac drzewo prosto
		oknoAplikacji.draw(drzewo);
		oknoAplikacji.display();

		socket.send( send_packet, remote_ip, remote_port);//wyslanie pakietu
		send_packet.clear();//czyszczenie pakietu
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    delete[] obrazek;
	oknoAplikacji.close();
	input_thread.terminate();
	network_thread.terminate();

    return EXIT_SUCCESS;
}
