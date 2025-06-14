#include <thread>
#include <iostream>
#include "message.hpp"
#include <queue>
#include <utility>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <mutex>

class client {
    private:
        std::thread receiveThread;
        std::thread sendThread; 
        bool mute;
        SOCKET serverSocket;
        std::string ID;
        bool newMessages;
        bool disconnect;
        sockaddr_in IP;
    public:
        std::queue<message> receiveQueue;
        std::queue<message> sendQueue; 
        SOCKET clientSocket;
        bool isMuted() {
            return mute;
        } 
        std::string getID() {
            return ID;
        }
        bool isNewMessages() {
            return newMessages;
        }
        void setMuted(bool change) {
            mute = change;
        }
        void setID(std::string newID) {
            ID = newID;
        }
        void setNewMessages(bool change) {
            newMessages = change;
        }
        void setdisconnect(bool change) {
            disconnect = change;
        }
        void setIP(sockaddr_in newIP) {
            IP = newIP;
        }
        void receivefromclient() {
            char buffer[1024];

            
            while(!disconnect) {
                memset(buffer, 0, sizeof(buffer));
                int bytesReceived = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
                if(bytesReceived > 0) {
                    std::vector<int> toList;
                    int header_index;
                    for(header_index=1; header_index < bytesReceived; header_index++) {
                        if(buffer[header_index] == '&') { //Header format toList (space separated) finished with &
                            break;
                        }
                        if(buffer[header_index] == ' ') {
                            continue;
                        }
                        toList.push_back(std::stoi(std::string(1, buffer[header_index])));
                    }
                    header_index = header_index+1;
                    int messageLength = bytesReceived - header_index+1;
                    char newMessage[messageLength];
                    for(int i = 0; i < messageLength; i++) {
                        newMessage[i] = buffer[header_index+i];
                    }

                    message* newptr = new message(toList, ID,std::string(newMessage));
                    if(buffer[0] == '@') {
                        std::cout << buffer[0] << std::endl;
                        newptr->group = true;
                    }
                    sendQueue.push(*newptr);
                }
                if(sendQueue.size() > 0) {
                    setNewMessages(true);
                }
                else {
                    setNewMessages(false);
                }
            }   
        }

        void startClient() {
            receiveThread.join();
            sendThread.join();
        }

        void sendtoclient() {
            
            message sendmessage;
            while(!disconnect) {
                if(!receiveQueue.empty()) {
                    sendmessage = receiveQueue.front();
                    receiveQueue.pop();
                    if (send(clientSocket, (sendmessage.Message).c_str(), sendmessage.Message.size(), 0) == -1) {
                        perror("send failed");
                    }
                }
            }
        }

        client(std::string newID, sockaddr_in newIP, SOCKET serversocket) {
            setMuted(false);
            setNewMessages(false);
            setID(newID);
            setIP(newIP);
            clientSocket = serversocket;
            serverSocket = serversocket;
            receiveThread = std::thread(&client::receivefromclient, this);
            sendThread = std::thread(&client::sendtoclient, this);

        }
    
};
