#include <thread>
#include <iostream>
#include <queue>
#include <utility>


class client {
    private:
        std::thread runningThread;
        bool mute;
        int ID;
        bool messageSent;
        bool disconnect;
    public:
        bool isMuted() {
            return mute;
        } 
        int getID() {
            return ID;
        }
        bool ismessageSent() {
            return messageSent;
        }
        void setMuted(bool change) {
            mute = change;
        }
        void setID(int newID) {
            ID = newID;
        }
        void setmessageSent(bool change) {
            messageSent = change;
        }
        void disconnect(bool change) {
            disconnect = change;
        }
        void run() {
            while(!disconnect) {

            }
        }
        client(int newID) {
            setMuted(false);
            setmessageSent(false);
            setID(newID);
            runningThread = std::thread(&client::run, this)

        }
    
}
