#include <iostream>
#include <memory>
#include <stdio.h>
#include <malloc.h>

using namespace std;

class C
{
public: 
	C(){cout<<"C"<<endl;}
	~C(){cout<<"~C"<<endl;}
};

static void my_init_hook(void);
static void *my_malloc_hook(size_t, const void *);
static void my_free_hook(void*, const void *);

static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void*, const void *);

void (*__MALLOC_HOOK_VOLATILE __malloc_initialize_hook) (void) = my_init_hook; 

static void my_init_hook(void)
{
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
}

static void *my_malloc_hook(size_t size, const void *caller)
{
	void *result;
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
	result = malloc(size);
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	printf("malloc (%u) returns %p\n", (unsigned int)size, result);
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
	return result;
}

static void my_free_hook(void *ptr, const void *caller)
{
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
	free(ptr);
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	printf("freed pointer %p\n", ptr);
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
}

int main()
{
	{
		cout<<"------- create shared_ptr p"<<endl;

		shared_ptr<C> p(new C);
		//auto p = make_shared<C>();

		cout<<"------- create weak_ptr wp with p"<<endl;

		weak_ptr<C> wp = p;
		
		cout<<"------- wp exp:"<<wp.expired()<<", reset p"<<endl;

		p = nullptr;

		cout<<"------- reset weak_ptr wp, exp:"<<wp.expired()<<endl;

		wp.reset();

		cout<<"------- end"<<endl;
	}

	return 0;
}
