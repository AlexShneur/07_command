#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <chrono>

using chronoTime = std::chrono::time_point<std::chrono::system_clock>;

// Observer or Subscriber
class BulkPrinter
{
public:
    virtual ~BulkPrinter() = default;

    virtual void update() = 0;
};

// Observable or Publisher
class Observable
{
public:
    virtual ~Observable() = default;

    // 1. Show ownership explicitly.
    virtual void subscribe(const std::shared_ptr<BulkPrinter>& obs) = 0;
};

class Bulk : public Observable
{
public:

    Bulk(int _size)
        : size{ _size }
    {
        index = 0;
    }

    void subscribe(const std::shared_ptr<BulkPrinter>& obs) override
    {
        m_subs.emplace_back(obs);
    }

    void add_data(const std::string& elem, const chronoTime& time, bool newElem)
    {
        if (m_bulk.empty())
        {
            m_time = time;
        }
        m_bulk.emplace_back(elem);
        if (newElem)
            ++index;
    }

    void ready(bool isReady)
    {
        if (isReady)
        {
            notify();
            m_bulk.clear();
            index = 0;
        }
    }

    const bool is_ready() const
    {
        return index == size;
    }

    const std::vector<std::string>& get_bulk() const
    {
        return m_bulk;
    }
    const chronoTime& get_time() const
    {
        return m_time;
    }

private:

    void notify()
    {
        auto iter = m_subs.begin();
        while (iter != m_subs.end())
        {
            auto ptr = iter->lock();
            if (ptr)
            {  // Если наблюдатель ещё существует, то оповещаем его
                ptr->update();
                ++iter;
            }
            else
            {  // Иначе - удаляем из списка наблюдателей.
                m_subs.erase(iter++);
            }
        }
    }

    // 1. Save weak_ptr
    // 2. Use std::list
    std::list<std::weak_ptr<BulkPrinter>> m_subs;

    std::vector<std::string> m_bulk;
    chronoTime m_time;
    const int size;
    int index;
};