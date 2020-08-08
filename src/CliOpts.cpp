#include "CliOpts.h"

using std::string;

CliOpts::CliOpts(int argc, char* argv[]) {
    string optName;
    for (int i = 1; i < argc; ++i) {
        const string opt = argv[i];
        //Options name
        if (opt[0] == '-') {
            //Long options (--)
            if (opt[1] == '-') {
                _options[opt.substr(2)] = "";
                optName = opt.substr(2);
            }
            //Short options (-)
            else {
                for (int j = 1; j < opt.length(); ++j) {
                    _options[{ opt[j] }] = "";
                }
                optName = "";
            }
        }
        //Options value
        else {
            //No current option name for this value.
            if (optName.empty()) {
                _values.push_back(opt);
            }
            //Add value to option name.
            else {
                _options[optName] = opt;
                optName = "";
            }
        }
    }
}

const bool CliOpts::hasOpt(const std::string& name) const {
    return _options.find(name) != _options.end();
}

const string CliOpts::getOpt(const std::string& name) const {
    return _options.at(name);
}

const std::vector<std::string>& CliOpts::getValues() const {
    return _values;
}