#pragma once
#include <unordered_map>
#include <string>
#include <vector>

class CliOpts {
public:	
    CliOpts(int argc, char* argv[]);
    const bool hasOpt(const std::string& name) const;
    const std::string getOpt(const std::string& name) const;
    const std::vector<std::string>& getValues() const;

private:
    std::unordered_map<std::string, std::string> _options;
    std::vector<std::string> _values;

};

