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
            values_(),
            parameter_info_(),
            print_info_trigger_() {}

        void parse_parameters(int argc, const char** argv) {
            values_["program_name"].first = true;
            values_["program_name"].second.push_back(argv[0]);

            for (int i(1); i<argc; ++i) {
                if (argv[i] == print_info_trigger_) {
                    print_parameter_information();
                    break;
                } else {
                    std::map<std::string, std::pair<bool, std::vector<std::string> > >::iterator match(values_.find(argv[i]));
                    if (match != values_.end()) {
                        for (unsigned j(0); j < match->second.second.size(); ++j) {
                            if (++i < argc) {
                                if (values_.find(argv[i]) != values_.end()) {
                                    std::cout << "ERROR: Number of values given for parameter \""<< match->first << "\" is less than expected! Cancelling..."<<std::endl;
                                    return;
                                }
                                match->second.second[j] = argv[i];

                            } else if (j < match->second.second.size()){
                                std::cout << "ERROR: Number of values given for parameter \""<< match->first << "\" is less than expected! Cancelling..."<<std::endl;
                                return;
                            }
                        }
                        match->second.first = true;
                    } else {
                        std::cout << "ERROR: parameter \""<< argv[i] << "\" is not a valid parameter!"<<std::endl;
                    }
                }
            }
        }

        void add_parameter(std::string const& parameter_name, unsigned value_count = 1) {
            if (values_.find(parameter_name) == values_.end())
                values_.insert(std::make_pair(parameter_name, std::make_pair(false, std::vector<std::string>(value_count))));
            else {
                std::cout << "WARNING: parameter \""<< parameter_name << "\" has already been set! Overwriting..."<<std::endl;
            }
        }

        void set_parameter_info(std::string const& parameter_name, std::string const& info) {
            if (values_.find(parameter_name) != values_.end())
                parameter_info_.insert(std::make_pair(parameter_name, info));
            else std::cout << "ERROR: parameter \""<< parameter_name << "\" has not been added to the list of possible parameters! Doing nothing..."<<std::endl;
        }

        void set_print_info_trigger(std::string const& info_trigger) {
            print_info_trigger_ =  info_trigger;
        }

        bool is_set(std::string const& parameter_name) {
            std::map<std::string, std::pair<bool, std::vector<std::string> > >::const_iterator searched(values_.find(parameter_name));
            if (searched != values_.end())
                return searched->second.first;

            std::cout << "ERROR: parameter \""<< parameter_name << "\" has not been added to the list of possible parameters! Returnig default value..."<<std::endl;
            return false;
        }

        template <typename T>
        T const get_value_of(std::string const& parameter_name) {
            T return_value = T();
            std::vector<T> return_values(get_values_of<T>(parameter_name));
            if (!return_values.empty())
                return_value = return_values.front();
            return return_value;
        }

        template <typename T>
        std::vector<T> const get_values_of(std::string const& parameter_name) {
            std::vector<T> return_values;
            std::map<std::string, std::pair<bool, std::vector<std::string> > >::const_iterator searched(values_.find(parameter_name));

            if (searched != values_.end()) {
                if (searched->second.first) {
                    for (std::vector<std::string>::const_iterator it(searched->second.second.begin()); it != searched->second.second.end(); ++it) {
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

        void print_parameter_information() const {
            for (std::map<std::string, std::string>::const_iterator it(parameter_info_.begin()); it!=parameter_info_.end(); ++it)
                std::cout<<it->first << ": " << it->second << std::cout<<std::endl;
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
        std::map<std::string, std::string> parameter_info_;
        std::string print_info_trigger_;
};

}

#endif //SCLAP_HPP
