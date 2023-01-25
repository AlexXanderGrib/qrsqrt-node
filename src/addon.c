#include <node_api.h>
#include "rsqrt.h"

#define xstr(a) str(a)
#define str(a) #a

#ifdef DEBUG 
#define NAPI_CALL(env, call)                                      \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      const char* err_message = error_info->error_message;        \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      if (!is_pending) {                                          \
        const char* message = (err_message == NULL)               \
            ? "empty error message"                               \
            : err_message;                                        \
        napi_throw_error((env), "" xstr(call) "", message);                   \
        return NULL;                                              \
      }                                                           \
    }                                                             \
  } while(0)
#else
  #define NAPI_CALL(env, call) if ((call) != napi_ok) return NULL;
#endif



napi_value Q_rsqrt_wrapper(napi_env env, napi_callback_info args) {
  size_t length = 1;
  napi_value argv[length];
  NAPI_CALL(env, napi_get_cb_info(env, args, &length, argv, NULL, NULL));

  double input;
  NAPI_CALL(env, napi_get_value_double(env, argv[0], &input));
  NAPI_CALL(env, napi_create_double(env, Q_rsqrt((float)input), &argv[0])); 

  return argv[0];
}

napi_value Q_rsqrt_wrapper_vec(napi_env env, napi_callback_info args) {
  size_t length = 1;
  napi_value argv[length];
  NAPI_CALL(env, napi_get_cb_info(env, args, &length, argv, NULL, NULL));

  float* array;
  size_t size;

  napi_value array_buffer;
  napi_typedarray_type typed_array_type;
  size_t offset;

  NAPI_CALL(env, napi_get_typedarray_info(env, argv[0], &typed_array_type, &size, &array, &array_buffer, &offset));

  Q_rsqrt_vec(size, array);

  return NULL;
}



napi_value init(napi_env env, napi_value exports) {
  napi_value single;
  const char* single_name = "Q_rsqrt";
  NAPI_CALL(env, napi_create_function(env, single_name, NAPI_AUTO_LENGTH, Q_rsqrt_wrapper, NULL, &single));
  NAPI_CALL(env, napi_set_named_property(env, exports, single_name, single));

  napi_value vector;
  const char* vector_name = "Q_rsqrt_vec";
  NAPI_CALL(env, napi_create_function(env, vector_name, NAPI_AUTO_LENGTH, Q_rsqrt_wrapper_vec, NULL, &vector));
  NAPI_CALL(env, napi_set_named_property(env, exports, vector_name, vector));


  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
