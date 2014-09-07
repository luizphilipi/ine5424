typedef void (*fptr)(void);

fptr __CTOR_END__[1] __attribute__((__unused__, section(".ctors"))) = { (fptr)0 };
fptr __DTOR_END__[1] __attribute__((__unused__, section(".dtors"))) = { (fptr)0 };

static void __do_global_ctors_aux()
{
    fptr * p;
    for(p = __CTOR_END__ - 1; *p != (fptr) -1; p--)
        (*p)();
}

void _init()
{
    __do_global_ctors_aux();
}

