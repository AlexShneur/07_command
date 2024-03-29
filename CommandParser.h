#pragma once
#include "Command.h"
#include "BracketStorage.h"

class CommandParser
{
public:

    int get_bulk_size() const
    {
        return bulk_size;
    }

    virtual void parse(Bulk& bulk) = 0;

protected:
    virtual void input(std::string& str) const = 0;

    void parse_cmd(const std::string& str, Bulk& bulk)
    {
        Command cmd(str);
        if (cmd.get_type() == CommandType::bracket)
        {
            if (cmd.get_bracket() == BracketType::open)
            {
                bulk.ready(bracketStorage.empty());
                bracketStorage.push(BracketType::open);
            }
            else if (cmd.get_bracket() == BracketType::close)
            {
                bracketStorage.pop();
                bulk.ready(bracketStorage.empty());
            }
        }
        else
        {
            auto time = std::chrono::system_clock::now();
            bulk.add_data(cmd.get_data(), time, bracketStorage.empty());
        }
    }

    void ready(Bulk& bulk)
    {
        bulk.ready(bracketStorage.empty());
    }

    BracketStorage bracketStorage;
    int bulk_size = -1;
};