#include "sclap.hpp"

int main(int argc, const char** argv) {

    sclap::Parser parser;
    parser.add_parameter("--multisample", 1);
    parser.set_parameter_info("--multisample", "The multisampling state. Accepts values 4, 9, 16 and 42.");

    parser.set_print_info_trigger("--help");

    parser.parse_parameters(argc, argv);

    return 0;
}
