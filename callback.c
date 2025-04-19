#include "callback.h"

struct callback_reason g_reasons[CB_MAX_REASONS];
size_t g_reasons_count = 0;

struct callback g_callbacks[CB_MAX_CALLBACKS];
size_t g_callbacks_count = 0;

/* callback type api */
struct callback_type
callback_make_type (char *_type_name, size_t _type_size)
{
  struct callback_type cbtype = { 0 };
#if 0
  strcpy(cbtype.name, _type_name);
#else
  cbtype.name = _type_name;
#endif
  cbtype.size = _type_size;
  return cbtype;
}

struct callback_function_argument
callback_make_function_argument (struct callback_type _arg_type,
                                 char *_arg_name)
{
  struct callback_function_argument cbfunarg = { 0 };
  cbfunarg.type = _arg_type;
  cbfunarg.name = _arg_name;
  return cbfunarg;
}

struct callback_function_signature
callback_make_function_signature (
    struct callback_type _func_return_type, char *_func_name,
    struct callback_function_argument *_func_args, size_t _func_args_count)
{
  struct callback_function_signature cbfunsign;
  cbfunsign.returntype = _func_return_type;
  cbfunsign.name = _func_name;
  cbfunsign.args = _func_args;
  cbfunsign.args_count = _func_args_count;
  return cbfunsign;
}

struct callback_function
callback_make_function (void *_func_ptr, char *_func_name,
                        char *_func_str_signature,
                        struct callback_function_signature *_func_signature)
{
  struct callback_function cbfun = { 0 };
  cbfun.ptr = _func_ptr;
  cbfun.name = _func_name;
  cbfun.signature = _func_str_signature;
  cbfun.fn_signature = _func_signature;
  return cbfun;
}

struct callback_ctx
callback_make_ctx (struct callback_reason reason,
                   struct callback_function caller)
{
  struct callback_ctx cbctx = { 0 };
  cbctx.reason = reason;
  cbctx.caller = caller;
  return cbctx;
}

struct callback
callback_make (char *name, callback_fn fn)
{
  struct callback cb = { 0 };
  cb.name = name;
  cb.fn = fn;
  return cb;
}

/* callback reason api */
struct callback_reason
callback_make_reason (char *shortname, char *message, char *description)
{
  struct callback_reason cbreason = { 0 };
  cbreason.shortname = shortname;
  cbreason.message = message;
  cbreason.description = description;
  return cbreason;
}

bool
callback_reasonid_registered (index_t id)
{
  for (size_t i = 0; i < g_reasons_count; ++i)
    if (g_reasons[i].id == id)
      return true;
  return false;
}

bool
callback_reason_registered (struct callback_reason reason)
{
  return callback_reasonid_registered (reason.id);
}

index_t
callback_register_reason (struct callback_reason reason)
{
  if (callback_reason_registered (reason))
    {
      for (size_t i = 0; i < g_reasons_count; ++i)
        if (g_reasons[i].id == reason.id)
          return (index_t)i;
    }

  if (g_reasons_count >= CB_MAX_REASONS)
    return (index_t)-1;

  g_reasons[g_reasons_count] = reason;
  g_reasons[g_reasons_count].id = g_reasons_count;
  return (index_t)(g_reasons_count++);
}

void
callback_unregister_reason (index_t idx)
{
  if (idx >= g_reasons_count)
    return;

  for (size_t i = idx; i + 1 < g_reasons_count; ++i)
    g_reasons[i] = g_reasons[i + 1];

  --g_reasons_count;
}

struct callback_reason *
callback_get_reason (index_t id)
{
  if (!callback_reasonid_registered (id))
    return NULL;
  return &(g_reasons[id]);
}

/* callback api */
bool
callback_id_registered (index_t id)
{
  for (size_t i = 0; i < g_callbacks_count; ++i)
    if (g_callbacks[i].id == id)
      return true;
  return false;
}

bool
callback_registered (struct callback callback)
{
  return callback_id_registered (callback.id);
}

index_t
callback_register (struct callback callback)
{
  if (callback_registered (callback))
    {
      for (size_t i = 0; i < g_callbacks_count; ++i)
        if (g_callbacks[i].id == callback.id)
          return (index_t)i;
    }

  if (g_callbacks_count >= CB_MAX_REASONS)
    return (index_t)-1;

  g_callbacks[g_callbacks_count] = callback;
  g_callbacks[g_callbacks_count].id = g_callbacks_count;
  return (index_t)(g_callbacks_count++);
}

void
callback_reason (index_t idx)
{
  if (idx >= g_callbacks_count)
    return;

  for (size_t i = idx; i + 1 < g_callbacks_count; ++i)
    g_callbacks[i] = g_callbacks[i + 1];

  --g_callbacks_count;
}

struct callback *
callback_get_unsafe (index_t id)
{
  return &(g_callbacks[id]);
}

struct callback *
callback_get (index_t id)
{
  if (!callback_id_registered (id))
    return NULL;
  return callback_get_unsafe (id);
}

struct callback *
callback_get_name (char *name)
{
  for (size_t i = 0; i < g_callbacks_count; ++i)
    if (strcmp (g_callbacks[i].name, name) == 0)
      return callback_get (i);
  return NULL;
}

struct callback *
callback_get_ptr (callback_fn fn)
{
  for (size_t i = 0; i < g_callbacks_count; ++i)
    if (g_callbacks[i].fn == fn)
      return callback_get (i);
  return NULL;
}

bool
callback_invoke_ptr (callback_fn fn, struct callback_ctx ctx)
{
  if (!fn)
    return false;
  fn (ctx);
  return true;
}

bool
callback_invoke (struct callback *cb, struct callback_ctx ctx)
{
  if (!cb || !cb->fn)
    return false;
  cb->fn (ctx);
  return true;
}

bool
callback_invoke_id (index_t id, struct callback_ctx ctx)
{
  if (!callback_id_registered (id))
    return false;

  struct callback *cb = callback_get_unsafe (id);
  return callback_invoke (cb, ctx);
}

bool
callback_invoke_name (char *name, struct callback_ctx ctx)
{
#if 1
  for (size_t i = 0; i < g_callbacks_count; ++i)
    if (g_callbacks[i].name == name)
      return callback_invoke_id (i, ctx);
  return false;
#else
  if (!name)
    return false;
  struct callback *cb = callback_get_name (name);
  if (!cb)
    return false;
  callback_invoke (cb, ctx);
  return true;
#endif
}

bool
callback_invoke_ptr_reg (callback_fn fn, struct callback_ctx ctx)
{
  for (size_t i = 0; i < g_callbacks_count; ++i)
    if (g_callbacks[i].fn == fn)
      return callback_invoke_id (i, ctx);
  return false;
}
