#pragma once
#include "CommandParser.h"

class InputCommandParser : public CommandParser
{
public:

    InputCommandParser()
    {
        std::cin >> bulk_size;
    }

    void parse(Bulk& bulk) override
    {
        while (!std::cin.eof())
        {
            while (!bulk.is_ready())
            {
                std::string str;
                input(str);

                if (std::cin.eof())
                {
                    break;
                }
                else
                {
                    parse_cmd(str, bulk);
                }
            }
            ready(bulk);
        }
    }
private:

    void input(std::string& str) const override
    {
        std::cin >> str;
    }
};