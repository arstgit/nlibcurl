#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>

#include "curl.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static int process_data (struct MemoryStruct *userp, char **response_ptr) {
  char * raw = userp->memory;
  size_t length = strlen(raw);
  char *ptr = realloc(*response_ptr, length + 1);
  if(ptr == NULL) {
    fprintf(stderr, "not enough memory (realloc returned NULL)\n");
    return 0;
  }
  *response_ptr = ptr;

  strcpy(*response_ptr, raw);

  return 0;
}
  
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    fprintf(stderr, "not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
 
int curl_get(char *url, char **response_ptr) {
  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);
  chunk.size = 0;
 
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
 
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  res = curl_easy_perform(curl_handle);
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  } else {
    process_data(&chunk, response_ptr);
  }
 
  free(chunk.memory);
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();

  return 0;
}

