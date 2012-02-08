#include "sclap.hpp"

int main(int argc, const char** argv) {

    sclap::Parser parser;
    parser.add_argument("--help");
    parser.parse_arguments(argc, argv);

    if (parser.is_set("--help"))
        std::cout<<parser.get_value_of<int>("--help")<<std::endl;

    return 0;
}
