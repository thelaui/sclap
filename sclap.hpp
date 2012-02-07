#ifndef SCLAP_HPP
#define SCLAP_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

namespace sclap {

class Parser {
    public:
        Parser():
            values_(){}

        void parse_arguments(int argc, const char** argv) {
            values_["program_name"].first = true;
            values_["program_name"].second.push_back(argv[0]);

            for (int i(1); i<argc; ++i) {
                std::map<std::string, std::pair<bool, std::vector<std::string> > >::iterator match(values_.find(argv[i]));
                if (match != values_.end()) {
                    for (unsigned j(0); j < match->second.second.size(); ++j) {
                        if (++i < argc) {
                            if (values_.find(argv[i]) != values_.end()) {
                                std::cout << "ERROR: Number of values given for argument \""<< match->first << "\" is less than expected! Cancelling..."<<std::endl;
                                return;
                            }
                            match->second.second[j] = argv[i];

                        } else if (j < match->second.second.size()){
                            std::cout << "ERROR: Number of values given for argument \""<< match->first << "\" is less than expected! Cancelling..."<<std::endl;
                            return;
                        }
                    }
                    match->second.first = true;
                } else {
                    std::cout << "ERROR: Argument \""<< argv[i] << "\" is not a valid parameter!"<<std::endl;
                }
            }
        }

        void add_argument(std::string const& argument_name, unsigned value_count = 1) {
            if (values_.find(argument_name) == values_.end())
                values_.insert(std::make_pair(argument_name, std::make_pair(false, std::vector<std::string>(value_count))));
            else {
                std::cout << "WARNING: Argument \""<< argument_name << "\" has already been set! Overwriting..."<<std::endl;
            }
        }

        bool is_set(std::string const& argument_name) {
            std::map<std::string, std::pair<bool, std::vector<std::string> > >::const_iterator searched(values_.find(argument_name));
            if (searched != values_.end())
                return searched->second.first;

            std::cout << "ERROR: Argument \""<< argument_name << "\" has not been added to the list of possible arguments! Returnig default value..."<<std::endl;
            return false;
        }

        template <typename T>
        T const get_value_of(std::string const& argument_name) {
            T return_value;
            std::vector<T> return_values(get_values_of<T>(argument_name));
            if (!return_values.empty())
                return_value = return_values.front();
            return return_value;
        }

        template <typename T>
        std::vector<T> const get_values_of(std::string const& argument_name) {
            std::vector<T> return_values;
            std::map<std::string, std::pair<bool, std::vector<std::string> > >::const_iterator searched(values_.find(argument_name));

            if (searched != values_.end()) {
                if (searched->second.first) {
                    for (std::vector<std::string>::const_iterator it(searched->second.second.begin()); it != searched->second.second.end(); ++it) {
                        std::stringstream sstr(*it);
                        T new_value;
                        sstr >> new_value;
                        return_values.push_back(new_value);
                    }
                } else {
                    std::cout << "WARNING: Argument \""<< argument_name << "\" has not been set! Returnig default value..."<<std::endl;
                }
            } else {
                std::cout << "ERROR: Argument \""<< argument_name << "\" has not been added to the list of possible arguments! Returnig default value..."<<std::endl;
            }

            return return_values;
        }

        void print_values() const {
            for (std::map<std::string, std::pair<bool, std::vector<std::string> > >::const_iterator it(values_.begin()); it!=values_.end(); ++it) {
                std::cout<<it->first << ": ";
                for (std::vector<std::string>::const_iterator val(it->second.second.begin()); val!=it->second.second.end(); ++val)
                    std::cout << *val << " ";
                std::cout<<std::endl;
            }
        }

    private:
        std::map<std::string, std::pair<bool, std::vector<std::string> > > values_;
};

}

#endif //SCLAP_HPP
