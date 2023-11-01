#include <traceroute.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <jansson.h>

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    fwrite(contents, 1, total_size, (FILE *)userp);
    return total_size;
}

void find_geolocation(char* ip_address) {
    CURL *curl;
    CURLcode res;
   
    curl = curl_easy_init();

    if (curl) {
        // Construct the URL 
        char url[100];
        snprintf(url, sizeof(url), "https://ipinfo.io/%s", ip_address);

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Specify a callback function to write the response to a file
        FILE* response_file = fopen("geolocation.json", "w");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_file);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } 
        
        fclose(response_file);
        curl_easy_cleanup(curl);

        FILE *jsonFile = fopen("geolocation.json", "r");
        if (!jsonFile) {
            fprintf(stderr, "Failed to open JSON file.\n");
        }

        json_t *root = json_loadf(jsonFile, 0, NULL);
        fclose(jsonFile);
        
        if (!root) {
            fprintf(stderr, "Failed to parse JSON.\n");
            exit(1);
        }

        json_t *city = json_object_get(root, "city");
        json_t *region = json_object_get(root, "region");
        json_t *country = json_object_get(root, "country");
        json_t *org = json_object_get(root, "org");
 
        if (json_is_string(city) && json_is_string(region) && json_is_string(country) && json_is_string(org)) {
            printf("Location: \n");
            const char *data1 = json_string_value(city);
            printf("  city: %s\n", data1);
            const char *data2 = json_string_value(region);
            printf("  region: %s\n", data2);
            const char *data3 = json_string_value(country);
            printf("  country: %s\n", data3);
            const char *data4 = json_string_value(org);
            printf("  organization: %s\n", data4);
        }
    }

}
