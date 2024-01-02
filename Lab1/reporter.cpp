#include <iostream>
#include <fstream>
#include <string> 

struct employee
{
    int num;        
    char name[10];  
    double hours;   
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Using: " << argv[0] << " binaryFile reportName moneyPerHour" << std::endl;
        return 1;
    }

    const char* binaryFile = argv[1];
    const char* reportName = argv[2];
    double doubleMoneyPerHour = std::stod(argv[3]);

    std::ifstream in(binaryFile, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "The binary file could not be opened." << std::endl;
        return 1;
    }

    std::ofstream out(reportName);
    if (!out.is_open()) {
        std::cerr << "The report file could not be created." << std::endl;
        return 1;
    }

    while (true) {
        employee emp;
        in.read((char*)&emp, sizeof(employee));
        if (in.eof())
            break;

        out << emp.num << " " << emp.name << " " << emp.hours << " " << doubleMoneyPerHour * emp.hours << '\n';
    }

    in.close();
    out.close();

    return 0;
}