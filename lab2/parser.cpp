#define CURL_STATICLIB
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <time.h>
#include <thread>
#include <nlohmann/json.hpp>
#include "TablePrinter-master-2/library/TablePrinter.hpp"
#include <algorithm>

using json = nlohmann::json;

class Statistics
{
private:
    std::map<std::string, std::vector<double>> m_statistics;

public:
    Statistics(std::map<std::string, std::vector<double>> stat) : m_statistics(stat) {}
    Statistics() {}
    void add_valute(std::string ind_val, double course)
    {
        if (!this->m_statistics.try_emplace(ind_val, std::vector<double> {course}).second) 
            this->m_statistics[ind_val].push_back(course);
    }
    void average_value_of_all()
    {
        trl::TablePrinter table_of_valutes;
        table_of_valutes.AddColumn("Valute", 6);
        table_of_valutes.AddColumn("Average Value", 13);
        table_of_valutes.PrintHeader();
        for (auto courses : this->m_statistics)
        {
            table_of_valutes << courses.first;
            double average = std::accumulate(courses.second.begin(), courses.second.end(), 0.0) / courses.second.size();
            table_of_valutes << average;
        }
        table_of_valutes.PrintFooter();
    }
    void median_value_of_all()
    {
        trl::TablePrinter table_of_valutes;
        table_of_valutes.AddColumn("Valute", 6);
        table_of_valutes.AddColumn("Median Value", 13);
        table_of_valutes.PrintHeader();
        for (auto courses : this->m_statistics)
        {
            table_of_valutes << courses.first;
            size_t n = courses.second.size() / 2;
            std::nth_element(courses.second.begin(), courses.second.begin() + n, courses.second.end());
            table_of_valutes << courses.second[n];
        }
        table_of_valutes.PrintFooter();
    }
};

void pad_to(std::string &str_, const size_t len, const int size_, const char paddingChar = ' ')
{
    if (len > size_)
        str_.insert(str_.begin(), len - size_, paddingChar);
}

int count_spaces(std::string &str_)
{
    int i = 0;
    int count = 0;
    while (str_[i])
    {
        if (str_[i] == ' ')
            count++;
        i += 1;
    }
    return count;
}

void parser(std::string data, Statistics& stat)
{
    try
    {
        json j = json::parse(data,
                             /* callback */ nullptr,
                             /* allow exceptions */ true,
                             /* ignore_comments */ true);

        trl::TablePrinter table_of_valutes;
        table_of_valutes.AddColumn("Valute", 6);
        table_of_valutes.AddColumn("Name", 40);
        table_of_valutes.AddColumn("Value", 9);
        table_of_valutes.PrintHeader();
        for (auto &[key, value] : j.at("Valute").items())
        {
            std::string name, value_val;
            int str_len;
            name = (std::string)value.at("Name");
            value_val = to_string(value.at("Value"));
            int cnt = count_spaces(name);
            if (cnt % 2 != 0)
                str_len = name.length() / 2 + cnt;
            else if (cnt == 0)
                str_len = name.length() / 2;
            else
                str_len = name.length() / 2 + cnt - 1;
            pad_to(name, 40, str_len);
            pad_to(value_val, 9, value_val.size());
            table_of_valutes << key << name << value_val;

            stat.add_valute(key, value.at("Value"));
        }
        table_of_valutes.PrintFooter();
    }
    catch (json::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void connect_and_parse(Statistics& stat)
{
    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    std::string readBuffer;
    CURLcode response;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/daily_json.js");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        response = curl_easy_perform(curl);

        if (response != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() returned: %s\n", curl_easy_strerror(response));
        }

        parser(readBuffer, stat);

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void connect_thr(int needed_time, Statistics& stat)
{
    while (true)
    {
        connect_and_parse(stat);
        sleep(needed_time);
        std::cout << "\n===========================================================\n\n";
    }
}

void disconnect_thr(Statistics& stat)
{
    int get = getchar();
    get = getchar();
    std::cout << "\n=========================Average===========================\n";
    stat.average_value_of_all();
    std::cout << "\n=========================Median============================\n";
    stat.median_value_of_all();
    std::cout << "\n===========================================================\n\n";
    exit(0);
}

int main()
{
    int needed_time;
    std::cout << "Enter the time how often you need to send the request: ";
    std::cin >> needed_time;
    Statistics stat;
    std::thread connect_thread(connect_thr, needed_time, std::ref(stat));
    std::thread disconnect_thread(disconnect_thr, std::ref(stat));
    disconnect_thread.join();
    connect_thread.join();
    return 0;
}