#include <iostream>
#include <curl/curl.h>

int main() {
  CURL* curl;
  CURLcode result;
  std::string url = "https://finance.yahoo.com";

  curl = curl_easy_init();

  if(!curl) {
    std::cerr << "HTTP request failed, please try again\n";
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);

  result = curl_easy_perform(curl);

  if(result != CURLE_OK) { 
    std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
    return -1;
  }
  
  curl_easy_cleanup(curl);
}