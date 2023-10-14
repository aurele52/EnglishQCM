#include "picojson.h"
#include <fstream>

#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

#include <curl/curl.h>

using namespace std;

enum setting_type {
    CLEAR,
    KEY,
    DEFAULT_LANG
};

typedef struct {
    string key;
    string default_lang;
} settings_struct;

bool setup_curl(CURL **curl) {
    *curl = curl_easy_init();

    if (curl == nullptr) {
        cerr << "Error: failed to set up curl" << endl;
        return false;
    }

    return true;
}

// curlを解放
void cleanup_curl(CURL **curl) {
    cout << "\r";

    curl_easy_cleanup(*curl);
}

// curlで受信したときの動作
size_t curl_on_receive(char *ptr, size_t size, size_t nmemb, void *stream) {
    vector<char> *recv_buffer = (vector<char>*)stream;
    const size_t sizes = size * nmemb;
    recv_buffer->insert(recv_buffer->end(), (char*)ptr, (char*)ptr + sizes);
    return sizes;
}

// curlの接続設定
bool connect_curl(CURL **curl, string url, string post_data, string &res_string) {
    vector<char> res_data;
    const char *post_data_c = post_data.c_str();
    const char *url_c = url.c_str();

    curl_easy_setopt(*curl, CURLOPT_URL, url_c);
    curl_easy_setopt(*curl, CURLOPT_POST, 1);
    curl_easy_setopt(*curl, CURLOPT_POSTFIELDS, post_data_c);
    curl_easy_setopt(*curl, CURLOPT_POSTFIELDSIZE, strlen(post_data_c));
    curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER, 1);
    curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, curl_on_receive);
    curl_easy_setopt(*curl, CURLOPT_WRITEDATA, &res_data);

    // 通信開始
    CURLcode res = curl_easy_perform(*curl);
    if (res != CURLE_OK) {
        cerr << "Error: curl_easy_perform failed: " << curl_easy_strerror(res) << endl;
        cleanup_curl(curl);
        return false;
    }

    cleanup_curl(curl);

    res_string = string(res_data.data());

    return true;
}

int translate(string api_key, string str, string &translated_text, string source_lang_code, string target_lang_code)
{
	source_lang_code = "EN";
	target_lang_code = "FR";
	std::cout << api_key << " " << str << " " << translated_text << " " << source_lang_code << " " << target_lang_code << std::endl;
    CURL *curl;

    // curlの初期設定
    if (!setup_curl(&curl)) {
        cleanup_curl(&curl);
        return 1;
    }

    string get_data;
    string post_data;
    
    post_data = "auth_key=" + api_key + "&text=" + str + "&target_lang=" + target_lang_code;
    
    if (source_lang_code != "") {
        post_data += "&source_lang=" + source_lang_code;
    }
    
    cout << "翻訳中.." << flush;
    if (!connect_curl(&curl, "https://api-free.deepl.com/v2/translate", post_data, get_data)) {
        cout << "\r" << string(8, ' ');
        cout << "\r";
        cerr << "Error: 翻訳結果の取得に失敗しました" << endl;
        cleanup_curl(&curl);
        return false;
    }
    cout << "\r" << string(8, ' ');
    cout << "\r";

    picojson::value v;
    string err = picojson::parse(v, get_data);
    if (!err.empty()) {
        cerr << "Error: picojson error - " << err << endl;
        return false;
    }

    translated_text = "";
    picojson::object& v_obj = v.get<picojson::object>();
    picojson::array& v_array = v_obj["translations"].get<picojson::array>();
    for (auto v_element : v_array) {
        picojson::object& v_element_obj = v_element.get<picojson::object>();
        translated_text += v_element_obj["text"].get<string>();
    }
    
    return true;
}

using namespace std;

map< string, vector<string> > source_langs;
map< string, vector<string> > target_langs;

int main(int argc, char *argv[]) {
    string translated_text;
    if (translate("asd", "think", translated_text, "", "")) {
        std::cout << translated_text << std::endl;
    }
    return 0;
}
