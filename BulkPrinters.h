#pragma once
#include "Bulk.h"
#include <cassert>
#include <iostream>
#include <fstream>

class ConsoleBulkPrinter : public BulkPrinter, public std::enable_shared_from_this<BulkPrinter>
{
public:
    // Объект должен быть создан в динамической памяти.
    static std::shared_ptr<ConsoleBulkPrinter> create(Bulk* bulk)
    {
        auto ptr = std::shared_ptr<ConsoleBulkPrinter>{ new ConsoleBulkPrinter{bulk} };
        ptr->set_bulk(bulk);
        return ptr;
    }

    void update() override
    {
        assert(m_bulk);
        print_bulk();
    }

private:
    // Чтобы пользователи не создавали объект в стеке, сделаем конструктор закрытым.
    ConsoleBulkPrinter(Bulk* bulk)
        : m_bulk{ bulk } {
    }

    void set_bulk(Bulk* bulk)
    {
        m_bulk = bulk;
        m_bulk->subscribe(shared_from_this());
    }

    void print_bulk() const
    {
        if (!m_bulk->get_bulk().empty())
        {
            std::cout << "bulk: ";
            for (auto it = begin(m_bulk->get_bulk()); it != end(m_bulk->get_bulk()); ++it)
            {
                if (it == (end(m_bulk->get_bulk()) - 1))
                    std::cout << *it << std::endl;
                else
                    std::cout << *it << ", ";
            }
        }
    }

    Bulk* m_bulk;
};

class FileBulkPrinter : public BulkPrinter, public std::enable_shared_from_this<BulkPrinter>
{
public:
    // Объект должен быть создан в динамической памяти.
    static std::shared_ptr<FileBulkPrinter> create(Bulk* bulk)
    {
        auto ptr = std::shared_ptr<FileBulkPrinter>{ new FileBulkPrinter{bulk} };
        ptr->set_bulk(bulk);
        return ptr;
    }

    void update() override
    {
        assert(m_bulk);
        print_bulk();
    }

private:
    // Чтобы пользователи не создавали объект в стеке, сделаем конструктор закрытым.
    FileBulkPrinter(Bulk* bulk)
        : m_bulk{ bulk }
    {

    }

    void set_bulk(Bulk* bulk)
    {
        m_bulk = bulk;
        m_bulk->subscribe(shared_from_this());
    }

    void print_bulk() const
    {
        if (!m_bulk->get_bulk().empty())
        {
            auto time_t = std::chrono::system_clock::to_time_t(m_bulk->get_time());
            std::ofstream file("bulk" + std::to_string(time_t) + ".log");
            file << "bulk: ";
            for (auto it = begin(m_bulk->get_bulk()); it != end(m_bulk->get_bulk()); ++it)
            {
                if (it == (end(m_bulk->get_bulk()) - 1))
                    file << *it << std::endl;
                else
                    file << *it << ", ";
            }
            file.close();
        }
    }

    Bulk* m_bulk;
};