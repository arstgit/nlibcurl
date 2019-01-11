#include <stdlib.h>
#include <node_api.h>

#include "curl.h"

static napi_value process(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 1;
    napi_value argv[argc];
    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to parse arguments");
    }
    
    size_t result;
    status = napi_get_value_string_utf8(env, argv[0], NULL, 0, &result);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid string was passed as argument");
    }
    char * url = malloc(result + 1);
    status = napi_get_value_string_utf8(env, argv[0], url, result + 1, &result);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid string was passed as argument");
    }
    
    char *response = malloc(1);
    curl_get(url, &response);
    free(url);
    
    napi_value res;
    status = napi_create_string_utf8(env, response, NAPI_AUTO_LENGTH, &res);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to create return value");
    }
    free(response);

    return res;    
}

static napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value fn;

    status = napi_create_function(env, NULL, 0, process, NULL, &fn);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to wrap native function");
    }

    status = napi_set_named_property(env, exports, "curl", fn);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Unable to populate exports");
    }
    
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
