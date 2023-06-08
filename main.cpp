#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <cstdlib>

#define FLIGHT_LENGTH_THRESHOLD 7
#define FLIGHT_ID_LENGTH_THRESHOLD 5

using namespace std;

std::string ltrim(const std::string &s)
{
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

std::string rtrim(const std::string &s)
{
    return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

std::string trim(const std::string &s)
{
    return ltrim(rtrim(s));
}

string cut_first_zeros(const std::string &str) {
    int number = atoi(str.c_str());
    return to_string(number);
}

bool has_flight_id_error(const string &flight)
{
    if (flight.length() < 1 || flight.length() > FLIGHT_ID_LENGTH_THRESHOLD) {
        return true;
    }

    regex pattern("^(\\d+)"); // flight id should contain only digits
    smatch match;
    if (!regex_search(flight, match, pattern)) {
        return true;
    }

    return false;
}

pair<string, string> parse_flight(const string &flight)
{
    pair<string, string> res;

    string company_id;
    string flight_id;

    // company ID can be empty or should contain at least one letter otherwise
    regex pattern1("^([A-Z]{3})");
    regex pattern2("^([A-Z]{2}\\s)");
    regex pattern3("^([A-Z]{2})");
    regex pattern4("^(([A-Z]\\d){1,2}\\s)");
    regex pattern5("^(([A-Z]\\d){1,2})");

    smatch match;

    if (regex_search(flight, match, pattern1)
            || regex_search(flight, match, pattern2)
            || regex_search(flight, match, pattern3)
            || regex_search(flight, match, pattern4)
            || regex_search(flight, match, pattern5)) {
        company_id = trim(match.str());
        flight_id = flight.substr(company_id.length(), flight.length() - company_id.length());
        flight_id = cut_first_zeros(flight_id);
        res.first = company_id;
        res.second = flight_id;
    } else {
        res.second = flight;
    }

    return res;
}

bool has_flight_error(const string &flight) {
    if (flight.length() == 0 || flight.length() == FLIGHT_LENGTH_THRESHOLD) {
        return true;
    }

    pair<string, string> flight_info = parse_flight(flight);
    if (has_flight_id_error(flight_info.second)) {
        return true;
    }

    return false;
}

bool compare_flights(const string &first_flight, const string &second_flight)
{
    string first = first_flight;
    string second = second_flight;
    std::transform(first.begin(), first.end(), first.begin(), ::toupper);
    std::transform(second.begin(), second.end(), second.begin(), ::toupper);

    pair<string, string> first_flight_info = parse_flight(first_flight);
    pair<string, string> second_flight_info = parse_flight(second_flight);

    if (first_flight_info.first.length() == 0
            && first_flight_info.second.length() == 0
            && second_flight_info.first.length() == 0
            && second_flight_info.second.length() == 0) {
        return false;
    }

    return first_flight_info == second_flight_info;
}

int main()
{
    string first_flight, second_flight;
    cout << "Enter flight № 1: ";
    getline(cin, first_flight);

    cout << "Enter flight № 2: ";
    getline(cin, second_flight);

    if (!has_flight_error(first_flight) && !has_flight_error(second_flight)) {
        cout << (compare_flights(first_flight, second_flight)
                 ? "Flights are equal"
                 : "Flights are not equal")
             << endl;
    } else {
        cout << "Some of your flights are entered with error" << endl;
    }

    return 0;
}
