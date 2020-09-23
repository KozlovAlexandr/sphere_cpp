#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <functional>
#include <algorithm>
#include <iterator>

using Data = std::vector<std::vector<std::string>>; 
using Splitted = std::vector<std::string>;

void update(Data &data, int field_num, const std::string &action)
{
    std::function<int(int)> func;
    if (action[0] == 'U')
    {
        func = toupper;
    } else if (action[0] == 'u')
    {
        func = tolower;
    } else if (action.size() == 3 && action[0] == 'R')
    {
        func = [from=action[1], to=action[2]](char c) { return c == from ? to : c; };
    } else
    {
        assert(false && "unknown case");
    }
    
    auto update_single_str_func = [&func](std::string& s) { std::transform(s.begin(), s.end(), s.begin(), func); };
    std::for_each(data.begin(), data.end(), [&update_single_str_func, field_num] (Splitted& splitted) {
        if (field_num < splitted.size())
            update_single_str_func(splitted[field_num]);
    });
}

int main()
{
    std::string filename;
    std::cin >> filename; 
    std::ifstream fin(filename, std::ios::in);

    Data data;

    while (fin)
    {
        std::string line;
        std::getline(fin, line);
        if (line.empty())
            break;
        
        std::istringstream line_stream(line);
        std::string field;
        Splitted splitted_line;

        while (std::getline(line_stream, field, '\t')) 
        {
            splitted_line.push_back(std::move(field));
        }

        data.push_back(std::move(splitted_line));
    }

    while (1)
    {
        int n;
        char sep;
        std::string action;

        std::cin >> n >> sep >> action;
        if (!std::cin)
            break;

        assert(sep == ':' && "bad separator");

        update(data, n, action);
    }

    for (Splitted &splitted_line : data)
    {
        std::move(splitted_line.begin(), splitted_line.end(), 
                std::ostream_iterator<std::string>(std::cout, "\t"));
        
        std::cout << std::endl;
    }

    return 0;
}