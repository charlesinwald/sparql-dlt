#include <iostream>
#include <curl/curl.h>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "pool.h"

using namespace std;
typedef map<std::string, std::string> Params;
CURL *curl; // CURL easy handle for use in most libcurl calls

void shutdown_handler(int s);
void make_request(const char *query);

void init();

//Append URL parameters to URL, using a map of string key-value pairs
static std::string params_string(Params const &params) {
    if (params.empty()) return "";
    Params::const_iterator pb = params.cbegin(), pe = params.cend();
    string data = pb->first + "=" + pb->second;
    ++pb;
    if (pb == pe) return data;
    for (; pb != pe; ++pb)
        data += "&" + pb->first + "=" + pb->second;
    return data;
}


int main() {
    int request_count = 0;
    int num_threads =  thread::hardware_concurrency();


    init();
    if (curl) {
        auto* pool = new tasks;
        pool->start(num_threads);
        pool->queue( [&] (){ return make_request("select+distinct+%3FConcept+where+{[]+a+%3FConcept}+LIMIT+100"); } );
//        for (;;) {
//            make_request(res, "select+distinct+%3FConcept+where+{[]+a+%3FConcept}+LIMIT+100");
//            request_count++;
//            cout << request_count << endl;
//        }
        
        return 0;
    }
    curl_global_cleanup();
    return 0;
}

/*
 * Set up CURL, initialize Ctrl+C handler
 */
void init() {
    curl_global_init(CURL_GLOBAL_DEFAULT); //sets up program environment that libcurl needs
    curl = curl_easy_init(); //creates easy handle for CURL

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = shutdown_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    pause();
}

//Handle shutdown by cleaning up CURL
void shutdown_handler(int s){
   cout << "Shutting Down..." <<endl;
   curl_easy_cleanup(curl);
   exit(1);

}

/*
 * Make a libCURL request to the sparql endpoint
 */
void make_request(const char *query) {
    CURLcode res;
    const auto str = "localhost:8890/sparql?" + params_string({{"default-graph-uri",    ""},
                                                               {"query",                query},
                                                               {"format",               "application/rdf+json"},
                                                               {"CXML_redir_for_subjs", "121"},
                                                               {"CXML_redir_for_hrefs", ""},
                                                               {"timeout",              "30000"},
                                                               {"debug",                "on"},
                                                               {"run",                  "+Run+Query+"}});
    cout << str << endl;
    curl_easy_setopt(curl, CURLOPT_URL, str.c_str()); //tells CURL how to behave
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
}
