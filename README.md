Code Guidelines
1. generalnie stosuj się do stylu kodu który jest już napisany
2. nazwy plików małymi literami
3. w nazwach stosujemy podkreślniki by oddzielić wyrazy
4. używamy raczej typów występujących w sfml
5. nazwy zmiennych w klasach piszemy w stylu m_nazwa_zmiennej

Instrukcja jak skonfigurować projekt pod Code::Blocks
1. Sklonuj projekt
2. Code::Blocks - Create a new project
3. Z listy wybierz SFML project
4. Next - SFML 2.0 - Next
5. Folder to create project in: wybierz folder RTS
6. Project title: wpisz server albo client
7. Next - Finish
8. Code::Blocks spyta czy nadpisać plik main.cpp - kliknij No
9. Project - Add files…
10. Dodaj pliki .cpp i .hpp
11. Project - Add files…
12. Dodaj pliki .cpp i .hpp z folderu commons
13. Project - Build options…
14. Po lewej stronie wybierz nazwę projeku(server albo client)
15. Linker settings - Link libraries: dodaj sfml-network
