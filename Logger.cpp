#include <iostream>
#include <fstream>
enum Log {
            WARNING,
            ERROR,
            INFO
        };

template <typename T>
class Logger{    

    Log level = INFO;
    std::string path = "errors.txt";
    public: 

    void log(Log Level, T message){
        this->setLevel(Level);
        this->log(message);
    };
    void log(T message){
        switch (this->level)
        {
        case INFO:
            std::cout << message << std::endl;
            break;
        case WARNING:
            std::cerr << message << std::endl;
            break;
        case ERROR:
            std::cerr << message << std::endl;
            break;
        default:
            std::cout<< message << std::endl;
            break;
        }
    };
    void setLevel(Log l){
        this->level = l;
    };
    void setWritePath(std::string p){
        this->path = p;
    }
    Log getLevel() {
        return this->level;
    }
    std::string getPath(){
        return this->path;
    }

    void write(T message) {
        std::ofstream f; 
        f.open(this->path, std::ios_base::app);
        f << message << std::endl;
        f.close();

    }
};
