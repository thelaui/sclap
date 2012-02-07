#include "sclap.hpp"

int main(int argc, const char** argv) {

    sclap::Parser parser;
    parser.add_argument("--help", 3);
    parser.parse_arguments(argc, argv);

    if (parser.is_set("--help"))
        std::cout<<"haha"<<std::endl;

    return 0;
}
