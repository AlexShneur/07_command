#pragma once
#include "CommandParser.h"
#include <vector>
#include <thread>

class TestCommandParser : public CommandParser
{
public:
    TestCommandParser(int _bulk_size, const std::vector<std::string>& _data)
        : data(_data)
    {
        bulk_size = _bulk_size;
    }

    void parse(Bulk& bulk) override
    {
        for (auto it = data.begin(); it != data.end();)
        {
            while (!bulk.is_ready())
            {
                auto str = *it;
                input(str);
                ++it;

                if (str == "EOF")
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
    std::vector<std::string> data;

    void input(std::string& str) const override
    {
        std::cout << str << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};