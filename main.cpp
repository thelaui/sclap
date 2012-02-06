#include "CLAP.hpp"

int main(int argc, const char** argv) {

    CLAP parser;
    parser.add_argument("--help", 2);
    parser.parse(argc, argv);

    parser.print_values();


    if (parser.is_set("--help"))
        std::cout<<"haha"<<std::endl;

    return 0;
}
