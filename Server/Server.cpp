#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include "../Packet/Packet.h"
#include <locale.h>
#include <string.h>
#include <cstring>
#include <sstream>
#include "../Packet/Note.h"
#include <ios>
#include <fstream>
#include <vector>
#pragma comment (lib, "ws2_32.lib") 

int main() {
    char* locale = setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));

    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    listen(servSock, 20);
    const char* filename = "notes.bin";
    const char* filename1 = "logs.bin";
    std::ifstream input(filename, std::ios_base::in | std::ios_base::binary);
    std::ifstream input1(filename1, std::ios_base::in | std::ios_base::binary);
    Note temp;
    std::vector<Note> notes;
    std::vector<Log> logs;
    while (input.read((char*)&temp, sizeof(Note)))
    {
        notes.push_back(temp);
    }
    Log tempLog;
    while (input1.read((char*)&tempLog, sizeof(Log)))
    {
        logs.push_back(tempLog);
    }
    input.close();
    input1.close();
    SOCKADDR clntAddr;
    int nSize;
    SOCKET clntSock;
    bool running = true;
    while (running)
    { 
        nSize = sizeof(SOCKADDR);
        clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        char szBuffer[260] = { 0 };
        recv(clntSock, szBuffer, 260, NULL);
        Packet pack;
        std::cout << std::endl;
        pack.Deserialize(szBuffer);
        std::cout << "Получен пакет: " << std::endl << std::endl;
        
        pack.Print();

        switch (pack.FC)
        {
        case 65:
        {
            char str[30] = "Добавление";
            logs.push_back(Log(str));
            std::ofstream ofs1(filename1, std::ios::binary | std::ios::out);
            for (int i = 0; i < logs.size(); i++)
            {
                logs[i].write(ofs1);
            }
            char buffer[252] = { 0 };
            //strcpy(pack.Data, _itoa(n, buffer, 10));
            //send(clntSock, pack.Serialize(), 260, NULL);
            std::stringstream s;
            char word[252] = {0};
            s << pack.Data;
            
            Note note(s);
            std::cout << note.date.day<<"."<<note.date.month <<"."<< note.date.year << ": "<<note.note;
            std::ofstream ofs(filename, std::ios::binary | std::ios::out);
            notes.push_back(note);
            for (int i = 0; i < notes.size(); i++)
            {
                notes[i].write(ofs);
            }
            const char* str1 = "Успешно добавлена запись!";
            strcpy(pack.Data, str1);
            send(clntSock, pack.Serialize(), 260, NULL);
            break;
        }
        case 66:
        {
            char str[30] = "Просмотр";
            logs.push_back(Log(str));
            std::ofstream ofs1(filename1, std::ios::binary | std::ios::out);
            for (int i = 0; i < logs.size(); i++)
            {
                logs[i].write(ofs1);
            }
            int n = notes.size();
            char nChar[10];
            _itoa(n, nChar, 10);
            strcpy(pack.Data, nChar);
            send(clntSock, pack.Serialize(), 260, NULL);
            for (int i = 0; i < n; i++)
            {
                char ymd[252] = { 0 };
                char data[252] = { 0 };
                _itoa(notes[i].date.year, ymd, 10);
                strcpy(data, ymd);
                strcat(data, " ");
                _itoa(notes[i].date.month, ymd, 10);
                strcat(data, ymd);
                strcat(data, " ");
                _itoa(notes[i].date.day, ymd, 10);
                strcat(data, ymd);
                strcat(data, " ");
                strcat(data, notes[i].note);
                strcpy(pack.Data, data);
                std::cout << pack.Data << std::endl;
                send(clntSock, pack.Serialize(), 260, NULL);
            }
            break;
        }
        case 67:
        {
            char str[30] = "Удаление";
            logs.push_back(Log(str));
            std::ofstream ofs1(filename1, std::ios::binary | std::ios::out);
            for (int i = 0; i < logs.size(); i++)
            {
                logs[i].write(ofs1);
            }
            std::stringstream s;
            
                pack.Deserialize(szBuffer);
                int num = atoi(pack.Data);
                char message[30];

                if (num > notes.size())
                {
                    strcpy(message, "Ошибка!");
                }
                else
                {
                    strcpy(message, "Удаление успешно!");
                    auto it = std::remove(notes.begin(), notes.end(), notes.at(num - 1));
                    std::cout << (*it).note;
                    notes.erase(notes.begin() + num - 1);
                    std::ofstream ofs(filename, std::ios::binary | std::ios::out);
                    for (int i = 0; i < notes.size(); i++)
                    {
                        notes[i].write(ofs);
                    }
                }
                strcpy(pack.Data, message);
                send(clntSock, pack.Serialize(), 260, NULL);
           
            break;
        }
        case 68:
        {
            char str[30] = "Редактирование";
            logs.push_back(Log(str));
            std::ofstream ofs1(filename1, std::ios::binary | std::ios::out);
            for (int i = 0; i < logs.size(); i++)
            {
                logs[i].write(ofs1);
            }
                int num = atoi(pack.Data);
                std::cout << num << std::endl;
                char message[30];
                if (num > notes.size())
                {
                    strcpy(message, "Ошибка!");
                }
                else
                {
                    strcpy(message, "Успешно");
                }
                strcpy(pack.Data, message);
                send(clntSock, pack.Serialize(), 260, NULL);
                recv(clntSock, szBuffer, 260, NULL);
                pack.Deserialize(szBuffer);
                auto it = notes.begin()+num-1;
                if (it != notes.end())
                {
                    strcpy(it->note, pack.Data);
                }
                std::ofstream ofs(filename, std::ios::binary | std::ios::out);
                for (int i = 0; i < notes.size(); i++)
                {
                    notes[i].write(ofs);
                }
                break;
        }

        case 69:
        {
            char str[30] = "Просмотр обращений";
            logs.push_back(Log(str));
            std::ofstream ofs1(filename1, std::ios::binary | std::ios::out);
            for (int i = 0; i < logs.size(); i++)
            {
                logs[i].write(ofs1);
            }
            int n = logs.size();
            char nChar[10];
            _itoa(n, nChar, 10);
            strcpy(pack.Data, nChar);
            send(clntSock, pack.Serialize(), 260, NULL);
            for (int i = 0; i < n; i++)
            {
                char ymd[252] = { 0 };
                char data[252] = { 0 };
                strcpy(data, logs[i].getTime());
                strcat(data, ": ");
                strcat(data, logs[i].call);
                strcpy(pack.Data, data);
                send(clntSock, pack.Serialize(), 260, NULL);
            }
            break;
        }
        default:
        {
            const char* str = "Неверный код функции!\n";
            send(clntSock, str, strlen(str) + sizeof(char), NULL);
            break;
        }
        }
    }

        closesocket(clntSock);
        closesocket(servSock);
    WSACleanup();
    

    return 0;
}
