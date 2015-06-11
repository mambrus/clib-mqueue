#define __init __attribute__((constructor))
#define __fini __attribute__((destructor))

/* Module/library initializers/deinitializers. */

void __init __libmqueue_init(void)
{
}

void __fini __libmqueue_fini(void)
{
}
