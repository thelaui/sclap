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
/// \brief Declaration and definition of the Parser class.
////////////////////////////////////////////////////////////////////////////////


#ifndef SCLAP_HPP
#define SCLAP_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

namespace sclap {

////////////////////////////////////////////////////////////////////////////////
/// \brief Parsers arguments and allows access to the parsed data.
///
////////////////////////////////////////////////////////////////////////////////

class Parser {
    public:

        ////////////////////////////////////////////////////////////////////////
        ///\brief Constructor.
        ///
        /// This constructs a new Parser.
        ///
        ////////////////////////////////////////////////////////////////////////
        Parser():
            values_(),
            parameter_info_(),
            print_info_trigger_() {}

        ////////////////////////////////////////////////////////////////////////
        ///\brief Parse arguments from given char buffer.
        ///
        /// This will parse all values stored in the given char buffer and search
        /// for corresponding entries in the parameter database. If no match is
        /// found or less arguments than specified for a parameter are given,
        /// error messages are printed.
        ///
        /// NOTE: The name of the program (which is normally stored in the first
        /// entry of the char buffer) is written to a field you can refer to with
        /// "program_name".
        ///
        ///\param argc The number of arguments stored in the given char buffer
        ///            (i.e. its size).
        ///\param argv The char buffer containing the data to be parsed.
        ////////////////////////////////////////////////////////////////////////
        void parse_parameters(int argc, const char** argv) {
            values_["program_name"].first = 1;
            values_["program_name"].second.first = false;
            values_["program_name"].second.second.push_back(argv[0]);

            for (int i(1); i<argc; ++i) {
                if (std::string(argv[i]) == print_info_trigger_) {
                    print_parameter_information();
                    break;
                } else {
                    std::map<std::string, std::pair<unsigned, std::pair<bool, std::vector<std::string> > > >::iterator match(values_.find(argv[i]));
                    if (match != values_.end()) {
                        for (unsigned j(0); j < match->second.first; ++j) {
                            if (++i < argc) {
                                if (values_.find(argv[i]) != values_.end() && !match->second.second.first) {
                                    std::cout << "ERROR: Number of values given for parameter \""<< match->first << "\" is less than expected! Cancelling..."<<std::endl;
                                    return;
                                }
                                match->second.second.second.push_back(argv[i]);

                            } else if (j < match->second.first && !match->second.second.first){
                                std::cout << "ERROR: Number of values given for parameter \""<< match->first << "\" is less than expected! Cancelling..."<<std::endl;
                                return;
                            }
                        }
                    } else {
                        std::cout << "ERROR: parameter \""<< argv[i] << "\" is not a valid parameter!"<<std::endl;
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Add a new parameter to the database.
        ///
        /// This adds a new parameter to the database. Every parameter you want
        /// to have access on later must be specified using this method. Only
        /// exception is the print_info_trigger.
        ///
        ///\param parameter_name The name of the parameter you want to specify.
        ///\param value_count The number of values expected for this parameter.
        ///\param less_allowed Specifies whether less values than the given
        ///                    number are exepted, too.
        ////////////////////////////////////////////////////////////////////////
        void add_parameter(std::string const& parameter_name, unsigned value_count = 1, bool less_allowed = false) {
            if (values_.find(parameter_name) == values_.end()) {
                values_.insert(std::make_pair(parameter_name, std::make_pair(value_count, std::make_pair(less_allowed, std::vector<std::string>()))));
                parameter_info_.insert(std::make_pair(parameter_name, std::string()));
            } else {
                std::cout << "WARNING: parameter \""<< parameter_name << "\" has already been set!"<<std::endl;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Sets information concerning a given parameter.
        ///
        /// This sets information concerning a given parameter. Parameter infor-
        /// mation can be print using the method print_parameter_information().
        ///
        ///\param parameter_name The name of the parameter you want to add
        ///                      information to.
        ///\param info The information string you want to add.
        ////////////////////////////////////////////////////////////////////////
        void set_parameter_info(std::string const& parameter_name, std::string const& info) {
            if (values_.find(parameter_name) != values_.end())
                parameter_info_[parameter_name] = info;
            else std::cout << "ERROR: parameter \""<< parameter_name << "\" has not been added to the list of possible parameters! Doing nothing..."<<std::endl;
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Sets the trigger for printing the specified information.
        ///
        /// This sets the trigger for printing the specified information. If
        /// this trigger is found in the data buffer whilst parsing, the parsing
        /// process is aborted and print_parameter_information() is called.
        ///
        ///\param info_trigger The name of the info_trigger (e.g. --help).
        ////////////////////////////////////////////////////////////////////////
        void set_print_info_trigger(std::string const& info_trigger) {
            print_info_trigger_ = info_trigger;
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Checks whether a given parameter is set.
        ///
        /// This sets checks wheter the parsed char buffer contained the given
        /// parameter.
        ///
        ///\param parameter_name The name of the parameter to be checked.
        ////////////////////////////////////////////////////////////////////////
        bool is_set(std::string const& parameter_name) const{
            std::map<std::string, std::pair<unsigned, std::pair<bool, std::vector<std::string> > > >::const_iterator searched(values_.find(parameter_name));
            if (searched != values_.end())
                return !searched->second.second.second.empty();

            std::cout << "ERROR: parameter \""<< parameter_name << "\" has not been added to the list of possible parameters! Returnig default value..."<<std::endl;
            return false;
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Returns a single value for a given parameter.
        ///
        /// This returns a single value for a given parameter. The template
        /// argument specifies the type of the value you want to have in return.
        /// If more than one values are specified for this parameter, this
        /// function will return only the first one of these.
        ///
        ///\param parameter_name The name of the parameter you want to get the
        ///                      value for.
        ///\return value The value for the given parameter, hopefully in the
        ///              type you specified with the template parameter.
        ////////////////////////////////////////////////////////////////////////
        template <typename T>
        T const get_value_of(std::string const& parameter_name) const{
            T return_value = T();
            std::vector<T> return_values(get_values_of<T>(parameter_name));
            if (!return_values.empty())
                return_value = return_values.front();
            return return_value;
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Returns a vector of values for a given parameter.
        ///
        /// This returns a vector of values for a given parameter.
        /// The template argument specifies the type of the values
        /// you want to have in return.
        ///
        ///\param parameter_name The name of the parameter you want to get the
        ///                      values for.
        ///\return values The value for the given parameter, hopefully in the
        ///               type you specified with the template parameter.
        ////////////////////////////////////////////////////////////////////////
        template <typename T>
        std::vector<T> const get_values_of(std::string const& parameter_name) const{
            std::vector<T> return_values;
            std::map<std::string, std::pair<unsigned, std::pair<bool, std::vector<std::string> > > >::const_iterator searched(values_.find(parameter_name));

            if (searched != values_.end()) {
                if (searched->second.first) {
                    for (std::vector<std::string>::const_iterator it(searched->second.second.second.begin()); it != searched->second.second.second.end(); ++it) {
                        std::stringstream sstr(*it);
                        T new_value;
                        sstr >> new_value;
                        return_values.push_back(new_value);
                    }
                } else {
                    std::cout << "WARNING: parameter \""<< parameter_name << "\" has not been set! Returnig default value..."<<std::endl;
                }
            } else {
                std::cout << "ERROR: parameter \""<< parameter_name << "\" has not been added to the list of possible parameters! Returnig default value..."<<std::endl;
            }

            return return_values;
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Prints information about parameters.
        ///
        /// This prints all specified information about parameters. If no
        /// information was set using set_parameter_info(), an empty string
        /// will be printed instead.
        ////////////////////////////////////////////////////////////////////////
        void print_parameter_information() const {
            for (std::map<std::string, std::string>::const_iterator it(parameter_info_.begin()); it!=parameter_info_.end(); ++it)
                std::cout << it->first << ":\t" << it->second << std::endl;
        }

        ////////////////////////////////////////////////////////////////////////
        ///\brief Prints the values of all parameters.
        ///
        /// This prints the values of all parameters. Useful for debugging.
        ////////////////////////////////////////////////////////////////////////
        void print_values() const {
            for (std::map<std::string, std::pair<unsigned, std::pair<bool, std::vector<std::string> > > >::const_iterator it(values_.begin()); it!=values_.end(); ++it) {
                std::cout<<it->first << ": ";
                for (std::vector<std::string>::const_iterator val(it->second.second.second.begin()); val!=it->second.second.second.end(); ++val)
                    std::cout << *val << " ";
                std::cout<<std::endl;
            }
        }

    private:
        //name, expected number of values, less allowed, values
        std::map<std::string, std::pair<unsigned, std::pair<bool, std::vector<std::string> > > > values_;
        std::map<std::string, std::string> parameter_info_;
        std::string print_info_trigger_;
};

}

#endif //SCLAP_HPP
