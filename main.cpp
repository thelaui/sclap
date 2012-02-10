////////////////////////////////////////////////////////////////////////////////
// sclap - a simple command line argument parser
//
// Copyright (c) 2012 by Felix Lauer
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
/// \file
/// \brief A simple example how to use the sclap.
////////////////////////////////////////////////////////////////////////////////

#include "sclap.hpp"

/// Assume your program has to be able to cope with information concerning
/// parent-children relationships. Therefore it would be useful to have the names
/// of mother, father and, lets say up to a number of 5, children. That's how
/// you would solve this using the sclap:

int main(int argc, const char** argv) {

    /// First of all get an instance of the parser class.
    sclap::Parser parser;

    /// Then add parameters to the database and specify how many values they
    /// shall expect.
    parser.add_parameter("--mother", 1);
    parser.add_parameter("--father", 1);
    parser.add_parameter("--children", 5, true);

    /// Now set some information about already specified parameters.
    /// If the parameters do not exist in the database, an error will be printed!
    parser.set_parameter_info("--mother", "The name of the family's mother.");
    parser.set_parameter_info("--father", "The name of the family's father.");
    parser.set_parameter_info("--children", "The names of the family's children.");

    /// Set the trigger for printing the parameter information.
    parser.set_print_info_trigger("--help");

    /// Parse the arguments given from the program call.
    parser.parse_parameters(argc, argv);

    /// Now the data is parsed and checked concerning your specified parameters.
    /// Occuring errors are printed to the standard console output stream.

    /// You can now work with the values stored for your parameters:

    /// Check whether the value of a parameter has been set and if so, print it.
    if (parser.is_set("--mother"))
        std::cout << "The mother's name is "
                  << parser.get_value_of<std::string>("--mother")
                  <<std::endl;;

//    /// Get multiple values.
//    if (parser.is_set("--children"))
//        if (parser.get_values_of<std::string>("--children").size() > 3)
//            std::cout<<"This is a pretty big family!"<<std::endl;

    /// Print the values for all parameters.
    parser.print_values();

    return 0;
}
