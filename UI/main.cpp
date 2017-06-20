#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <future>
#include <iostream>
#include <rapidjson/document.h>
#include <sstream>

std::future<std::string> invokeAPI(std::string const& url)
{
  return std::async(std::launch::async,
                    [](std::string const& url) mutable {
                      std::list<std::string> header;
                      header.push_back("Content-Type: application/json");

                      curlpp::Cleanup clean;
                      curlpp::Easy r;
                      r.setOpt<curlpp::options::Url>(url);
                      r.setOpt<curlpp::options::HttpHeader>(header);

                      std::ostringstream response;
                      r.setOpt<curlpp::options::WriteStream>(&response);

                      r.perform();

                      return std::string(response.str());
                    },
                    url);
}

std::string buildURL(int i, int j)
{
  return std::string("http://localhost:3000/api/add/") + std::to_string(i) +
         "/" + std::to_string(j);
}

int main(int, char**)
{
  int i, j = 0;
  std::cout << "Sample ADD API C++ call\n";
  while (true)
  {
    std::cout << "Enter first int:  ";
    std::cin >> i;
    std::cout << "Enter second int: ";
    std::cin >> j;
    auto url = buildURL(i, j);
    auto res = invokeAPI(url);
    res.wait();
    rapidjson::Document doc;
    doc.Parse(res.get().c_str());
    assert(doc.IsObject());
    assert(doc.HasMember("result"));
    assert(doc["result"].IsInt());
    std::cout << i << " + " << j << " = " << doc["result"].GetInt() << "\n";
  }
  return 0;
}
