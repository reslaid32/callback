#include "callback.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
on_error_callback (struct callback_ctx ctx)
{
  struct callback *cb = callback_get_ptr (on_error_callback);
  printf ("Callback: %s at %p\n", cb->name, (void *)cb->fn);
  printf ("Reason: %s - %s\n", ctx.reason.shortname, ctx.reason.message);
  printf ("Description: %s\n", ctx.reason.description);
  printf ("Caller: %s (%s)\n", ctx.caller.name, ctx.caller.signature);
}

int
main ()
{
  /* Creating a callback reason */
  struct callback_reason reason
      = callback_make_reason ("error_callback", "An error has occurred",
                              "Triggered when an error is detected");

  /* Registering the callback reason */
  index_t reason_id = callback_register_reason (reason);
  if (reason_id < 0)
    {
      fprintf (stderr, "Failed to register reason\n");
      return 1;
    }

  /* Creating a callback */
  struct callback cb = callback_make ("ErrorCallback", on_error_callback);

  /* Registering the callback */
  index_t callback_id = callback_register (cb);
  if (callback_id < 0)
    {
      fprintf (stderr, "Failed to register callback\n");
      return 1;
    }

  /* Simulating a function that caused an error */
  struct callback_function caller
      = callback_make_function (NULL, "main", "int main(void)", NULL);

  /* Creating the callback context */
  struct callback_ctx ctx
      = callback_make_ctx (*callback_get_reason (reason_id), caller);

  /* Invoking the callback by ID */
  callback_invoke_id (callback_id, ctx);

  return 0;
}
