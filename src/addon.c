#include <node_api.h>
#include "rsqrt.h"

#define execute(call) if ((call) != napi_ok) return NULL; 


napi_value Q_rsqrt_wrapper(napi_env env, napi_callback_info args) {
  size_t length = 1;
  napi_value argv[length];
  double input;

  execute(napi_get_cb_info(env, args, &length, argv, NULL, NULL));
  execute(napi_get_value_double(env, argv[0], &input));
  execute(napi_create_double(env, Q_rsqrt((float)input), &argv[0])); 

  return argv[0];
}

napi_value init(napi_env env, napi_value exports) {
  napi_value fn;

  const char* name = "Q_rsqrt";

  execute(napi_create_function(env, name, NAPI_AUTO_LENGTH, Q_rsqrt_wrapper, NULL, &fn));
  execute(napi_set_named_property(env, exports, name, fn));

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
