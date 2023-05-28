#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#include <iostream>
#include "../Packet/Packet.h"
#include <locale.h>
#include "../Packet/Note.h"
#include <sstream>
#include <ios>
#include <cstring>

int main() 
{
    SetConsoleCP(1251);
    char* locale = setlocale(LC_ALL, "Russian");
    // Инициализируем DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // Создаем сокет
    // Инициируем запрос к серверу
    const char* menuString = "1.Добавить заметку\n2.Просмотр заметок\n3.Удалить заметку\n4.Редактировать заметку\n5.Логи\n0.Выход\n\nВыберите пункт из меню: ";
    int nSize;
    // Получение данных, возвращаемых сервером
    char menu;
    bool running = true;
    char data[252] = {0};
    while (running)
    {
        char szBuffer[260] = { 0 };
        SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr)); // Каждый байт заполняется 0
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sockAddr.sin_port = htons(1234);
        connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        system("cls");
        std::cout << menuString;
        std::cin >> menu;
        switch (menu)
        {
        case'1':
        {            
            char fc = 65;
            int y;
            int m;
            int d;
            std::cout << "Введите год: ";
            std::cin >> y;
            std::cout << "Введите месяц(1-12): ";
            std::cin >> m;
            std::cout << "Введите день: ";
            std::cin >> d;
            std::cout << "Введите заметку: ";
            char message[240];
            std::cin.ignore();
            std::cin.getline(message,240);

            std::cout << message;
            system("pause");
            Note note(y,m,d, message);
            char ymd[252] = { 0 };
            system("cls");
            _itoa(note.date.year, ymd, 10);
            strcpy(data, ymd);
            strcat(data, " ");
            _itoa(note.date.month, ymd, 10);
            strcat(data, ymd);
            strcat(data, " ");
            _itoa(note.date.day, ymd, 10);
            strcat(data, ymd);
            strcat(data, " ");
            strcat(data, message);
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            std::cout << pack.Data << std::endl;
            system("pause");
            break;
        }
        case '2':
        {
            char fc = 66;
            
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            int n;
            n = atoi(pack.Data);

            if (n == 0)
            {
                std::cout << "Заметок не найдено!" << std::endl;
            }
            else {
                for (int i = 0; i < n; i++)
                {

                    recv(sock, szBuffer, 260, NULL);
                    pack.Deserialize(szBuffer);
                    std::stringstream s;
                    s << pack.Data;
                    Note note(s);
                    std::cout << i + 1 << ")" << (int)note.date.day << "." << (int)note.date.month << "." << (int)note.date.year << ": " << note.note << std::endl;

                }
            }
            system("pause");
            break;
        }
        case '3':
        {
            char fc = 67;
            int y;
            int m;
            int d;
            std::cout << "Введите год: ";
            std::cin >> y;
            std::cout << "Введите месяц(1-12): ";
            std::cin >> m;
            std::cout << "Введите день: ";
            std::cin >> d;
            char ymd[252] = { 0 };
            _itoa(y, ymd, 10);
            strcpy(data, ymd);
            strcat(data, " ");
            _itoa(m, ymd, 10);
            strcat(data, ymd);
            strcat(data, " ");
            _itoa(d, ymd, 10);
            strcat(data, ymd);
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            int n;
            n = atoi(pack.Data);

            if (n == 0)
            {
                std::cout << "Заметок не найдено!" << std::endl;
            }
            else {
                for (int i = 0; i < n; i++)
                {

                    recv(sock, szBuffer, 260, NULL);
                    pack.Deserialize(szBuffer);
                    std::stringstream s;
                    s << pack.Data;
                    Note note(s);
                    std::cout << i + 1 << ")" << (int)note.date.day << "." << (int)note.date.month << "." << (int)note.date.year << ": " << note.note << std::endl;
                }

                std::cout << "Выберите запись для удаления или нажмите 0, чтобы ничего не удалять: ";
                int choice;
                std::cin >> choice;
                char ch[5];
                _itoa(choice, ch, 10);
                strcpy(pack.Data, ch);
                send(sock, pack.Serialize(), 260, NULL);
                recv(sock, szBuffer, 260, NULL);
                pack.Deserialize(szBuffer);
                std::cout << pack.Data << std::endl;
            }
            system("pause");
            break;
        }
        case '4':
        {
            char fc = 68;
            int y;
            int m;
            int d;
            std::cout << "Введите год: ";
            std::cin >> y;
            std::cout << "Введите месяц(1-12): ";
            std::cin >> m;
            std::cout << "Введите день: ";
            std::cin >> d;
            char ymd[252] = { 0 };
            _itoa(y, ymd, 10);
            strcpy(data, ymd);
            strcat(data, " ");
            _itoa(m, ymd, 10);
            strcat(data, ymd);
            strcat(data, " ");
            _itoa(d, ymd, 10);
            strcat(data, ymd);
            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            int n;
            n = atoi(pack.Data);;

            if (n == 0)
            {
                std::cout << "Заметок не найдено!" << std::endl;
            }
            else {
                for (int i = 0; i < n; i++)
                {

                    recv(sock, szBuffer, 260, NULL);
                    pack.Deserialize(szBuffer);
                    std::stringstream s;
                    s << pack.Data;
                    Note note(s);
                    std::cout << i + 1 << ")" << (int)note.date.day << "." << (int)note.date.month << "." << (int)note.date.year << ": " << note.note << std::endl;
                }
                std::cout << "Выберите запись для редактирования или нажмите 0, чтобы ничего не удалять: ";
                int choice;
                std::cin >> choice;
                char ch[5];
                _itoa(choice, ch, 10);
                strcpy(pack.Data, ch);
                send(sock, pack.Serialize(), 260, NULL);
                recv(sock, szBuffer, 260, NULL);
                pack.Deserialize(szBuffer);
                if (strcmp(pack.Data, "Успешно") == 0)
                {
                    std::cout << "Введите новую заметку: ";
                    char message[240];
                    std::cin.ignore();
                    std::cin.getline(message, 240);
                    strcpy(pack.Data, message);
                    send(sock, pack.Serialize(), 260, NULL);
                }
                else
                {
                    std::cout << pack.Data << std::endl;
                }
            }
            system("pause");
            break;
        }
        case '5':
        {
            char fc = 69;

            Packet pack(fc, data);
            send(sock, pack.Serialize(), 260, NULL);
            system("cls");
            std::cout << "Отправлен пакет: " << std::endl << std::endl;
            pack.Print();
            system("pause");
            system("cls");
            recv(sock, szBuffer, 260, NULL);
            pack.Deserialize(szBuffer);
            int n;
            n = atoi(pack.Data);


            for (int i = 0; i < n; i++)
            {

                recv(sock, szBuffer, 260, NULL);
                pack.Deserialize(szBuffer);
                std::stringstream s;
                std::cout << i + 1 << ")" << pack.Data << std::endl;

            }
            system("pause");
            break;
        }
        case'0':
        {
            running = false;
            closesocket(sock);
            break;
        }
        }
    }
    // Закрываем сокет
    // Прекращаем использование DLL
    WSACleanup();
    system("pause");
    return 0;
}
