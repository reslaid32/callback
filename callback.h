
#ifndef __CALLBACK_H
#define __CALLBACK_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef CB_MAX_REASONS
#define CB_MAX_REASONS 256
#endif

#ifndef CB_MAX_CALLBACKS
#define CB_MAX_CALLBACKS 64
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  typedef ssize_t index_t;

  struct callback_type
  {
    char *name;
    size_t size;
  };

  struct callback_function_argument
  {
    struct callback_type type;
    char *name;
  };

  struct callback_function_signature
  {
    struct callback_type returntype;
    char *name;
    struct callback_function_argument *args;
    size_t args_count;
  };

  struct callback_function
  {
    void *ptr;
    char *name;
    char *signature;
    struct callback_function_signature *fn_signature;
  };

  struct callback_reason
  {
    index_t id;        /* reason id */
    char *shortname;   /* reason name: error_callback, etc */
    char *message;     /* reason message */
    char *description; /* reason description */
  };

  static struct callback_reason g_reasons[CB_MAX_REASONS];
  static size_t g_reasons_count = 0;

/* forward type-allias */
#define callback_ctx callback_context
  struct callback_ctx
  {
    struct callback_reason reason;
    struct callback_function caller;
  };

  typedef void (*callback_fn) (struct callback_ctx);
  struct callback
  {
    index_t id; /* callback id */
    char *name;
    callback_fn fn;
  };

  static struct callback g_callbacks[CB_MAX_CALLBACKS];
  static size_t g_callbacks_count = 0;

/* callback type api */
#define callback_make_type_def(Type) callback_make_type (#Type, sizeof (Type))

  struct callback_type callback_make_type (char *_type_name,
                                           size_t _type_size);

  /* callback reason api */
  struct callback_reason callback_make_reason (char *shortname, char *message,
                                               char *description);
  bool callback_reasonid_registered (index_t id);
  bool callback_reason_registered (struct callback_reason reason);
  index_t callback_register_reason (struct callback_reason reason);
  void callback_unregister_reason (index_t idx);
  struct callback_reason *callback_get_reason (index_t id);

  /* callback api */
  struct callback_function_argument
  callback_make_function_argument (struct callback_type _arg_type,
                                   char *_arg_name);

  struct callback_function_signature callback_make_function_signature (
      struct callback_type _func_return_type, char *_func_name,
      struct callback_function_argument *_func_args, size_t _func_args_count);

  struct callback_function
  callback_make_function (void *_func_ptr, char *_func_name,
                          char *_func_str_signature,
                          struct callback_function_signature *_func_signature);

  struct callback_ctx callback_make_ctx (struct callback_reason reason,
                                         struct callback_function caller);

  struct callback callback_make (char *name, callback_fn fn);

  bool callback_id_registered (index_t id);
  bool callback_registered (struct callback callback);
  index_t callback_register (struct callback callback);
  void callback_reason (index_t idx);
  struct callback *callback_force_get (index_t id);
  struct callback *callback_get (index_t id);
  struct callback *callback_get_name (char *name);
  struct callback *callback_get_ptr (callback_fn fn);
  bool callback_invoke (struct callback *cb, struct callback_ctx ctx);
  bool callback_invoke_id (index_t id, struct callback_ctx ctx);
  bool callback_invoke_name (char *name, struct callback_ctx ctx);

#ifdef __cplusplus
}
#endif

#endif /* __CALLBACK_H */