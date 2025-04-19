#include "callback.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* A simple callback function for testing purposes */
void
test_callback_fn (struct callback_ctx ctx)
{
  (void)ctx; // We won't use ctx in this test
}

/* Test creating and registering a callback */
void
test_callback_register ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  // Register the callback and check if it was successfully registered
  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  struct callback *retrieved_cb = callback_get (cb_id);
  assert (retrieved_cb != NULL); // Ensure the callback was retrieved
  assert (strcmp (retrieved_cb->name, "test_callback")
          == 0); // Ensure the name matches
}

/* Test invoking a callback by its ID */
void
test_callback_invoke_id ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");
  struct callback_function caller
      = callback_make_function ((void *)1, "caller_fn", "int -> void", NULL);
  struct callback_ctx ctx = callback_make_ctx (reason, caller);

  // Invoke the callback and check the result
  bool result = callback_invoke_id (cb_id, ctx);
  assert (result); // Ensure the callback was successfully invoked
}

/* Test invoking a callback by name */
void
test_callback_invoke_name ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");
  struct callback_function caller
      = callback_make_function ((void *)1, "caller_fn", "int -> void", NULL);
  struct callback_ctx ctx = callback_make_ctx (reason, caller);

  // Invoke the callback by name and check the result
  bool result = callback_invoke_name ("test_callback", ctx);
  assert (result); // Ensure the callback was successfully invoked
}

/* Test invoking a callback by function pointer */
void
test_callback_invoke_ptr ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");
  struct callback_function caller
      = callback_make_function ((void *)1, "caller_fn", "int -> void", NULL);
  struct callback_ctx ctx = callback_make_ctx (reason, caller);

  // Invoke the callback by function pointer and check the result
  bool result = callback_invoke_ptr (test_fn, ctx);
  assert (result); // Ensure the callback was successfully invoked
}

/* Test invoking a callback by registered function pointer */
void
test_callback_invoke_ptr_reg ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");
  struct callback_function caller
      = callback_make_function ((void *)1, "caller_fn", "int -> void", NULL);
  struct callback_ctx ctx = callback_make_ctx (reason, caller);

  // Invoke the callback by registered function pointer and check the result
  bool result = callback_invoke_ptr_reg (test_fn, ctx);
  assert (result); // Ensure the callback was successfully invoked
}

/* Test unregistering a callback by ID */
void
test_callback_unregister ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  // Unregister the callback by ID
  callback_reason (cb_id); // This is the function that un-registers by ID
  struct callback *retrieved_cb = callback_get (cb_id);
  assert (retrieved_cb
          == NULL); // Ensure the callback was successfully unregistered
}

/* Test callback reason registration */
void
test_callback_reason_register ()
{
  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");

  // Register the reason
  index_t reason_id = callback_register_reason (reason);
  assert (reason_id
          != (index_t)-1); // Ensure the reason was successfully registered

  struct callback_reason *retrieved_reason = callback_get_reason (reason_id);
  assert (retrieved_reason != NULL); // Ensure the reason was retrieved
  assert (strcmp (retrieved_reason->shortname, "error")
          == 0); // Ensure the reason name matches
}

/* Test callback reason unregister */
void
test_callback_reason_unregister ()
{
  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");

  // Register and then unregister the reason
  index_t reason_id = callback_register_reason (reason);
  assert (reason_id
          != (index_t)-1); // Ensure the reason was successfully registered

  callback_unregister_reason (reason_id); // Unregister the reason

  struct callback_reason *retrieved_reason = callback_get_reason (reason_id);
  assert (retrieved_reason
          == NULL); // Ensure the reason was successfully unregistered
}

/* Test callback reason ID registration check */
void
test_callback_reasonid_registered ()
{
  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");

  // Register the reason
  index_t reason_id = callback_register_reason (reason);
  assert (reason_id
          != (index_t)-1); // Ensure the reason was successfully registered

  // Check if the reason ID is registered
  bool is_registered = callback_reasonid_registered (reason_id);
  assert (is_registered); // Ensure the reason ID is registered
}

/* Test callback reason registration check */
void
test_callback_reason_registered ()
{
  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");

  // Register the reason
  index_t reason_id = callback_register_reason (reason);
  assert (reason_id
          != (index_t)-1); // Ensure the reason was successfully registered

  // Check if the reason is registered
  bool is_registered = callback_reason_registered (reason);
  assert (is_registered); // Ensure the reason is registered
}

/* Test callback registration check by ID */
void
test_callback_id_registered ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  // Check if the callback ID is registered
  bool is_registered = callback_id_registered (cb_id);
  assert (is_registered); // Ensure the callback ID is registered
}

/* Test callback registration check */
void
test_callback_registered ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  // Check if the callback is registered
  bool is_registered = callback_registered (test_callback);
  assert (is_registered); // Ensure the callback is registered
}

/* Test getting a callback by pointer */
void
test_callback_get_ptr ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  // Get callback by function pointer
  struct callback *retrieved_cb = callback_get_ptr (test_fn);
  assert (retrieved_cb
          != NULL); // Ensure the callback was retrieved by pointer
}

/* Test getting a callback by name */
void
test_callback_get_name ()
{
  callback_fn test_fn = test_callback_fn;
  struct callback test_callback = callback_make ("test_callback", test_fn);

  index_t cb_id = callback_register (test_callback);
  assert (cb_id != (index_t)-1); // Ensure callback was successfully registered

  // Get callback by name
  struct callback *retrieved_cb = callback_get_name ("test_callback");
  assert (retrieved_cb != NULL); // Ensure the callback was retrieved by name
}

/* Test creating a callback context */
void
test_callback_make_ctx ()
{
  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");
  struct callback_function caller
      = callback_make_function ((void *)1, "caller_fn", "int -> void", NULL);

  // Create the callback context
  struct callback_ctx ctx = callback_make_ctx (reason, caller);
  assert (ctx.reason.id != (index_t)-1); // Ensure the reason ID is valid
  assert (strcmp (ctx.caller.name, "caller_fn")
          == 0); // Ensure the caller name is correct
}

/* Test getting a reason by ID */
void
test_callback_get_reason ()
{
  struct callback_reason reason = callback_make_reason (
      "error", "An error occurred", "Detailed error description");

  // Register the reason
  index_t reason_id = callback_register_reason (reason);
  assert (reason_id
          != (index_t)-1); // Ensure the reason was successfully registered

  // Retrieve the reason by ID
  struct callback_reason *retrieved_reason = callback_get_reason (reason_id);
  assert (retrieved_reason != NULL); // Ensure the reason was retrieved
  assert (strcmp (retrieved_reason->shortname, "error")
          == 0); // Ensure the reason matches
}

int
main ()
{
  /* Run all tests */
  test_callback_register ();
  test_callback_invoke_id ();
  test_callback_invoke_name ();
  test_callback_invoke_ptr ();
  test_callback_invoke_ptr_reg ();
  test_callback_unregister ();
  test_callback_reason_register ();
  test_callback_reason_unregister ();
  test_callback_reasonid_registered ();
  test_callback_reason_registered ();
  test_callback_id_registered ();
  test_callback_registered ();
  test_callback_get_ptr ();
  test_callback_get_name ();
  test_callback_make_ctx ();
  test_callback_get_reason ();

  printf ("All tests passed!\n");
  return 0;
}
