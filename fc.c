// -ldl
#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>

long long mallocCallNum = 0;
long long callocCallNum = 0;
long long freeCallNum = 0;

void* (*malloc_real)(size_t) = NULL;
void* (*calloc_real)(size_t,size_t) = NULL;
void (*free_real)(void*) = NULL;

void __attribute__((constructor)) init_hooking()
{
	fprintf(stderr, "init_hooking() call start!!!\n");
	malloc_real = dlsym(RTLD_NEXT, "malloc");
	fprintf(stderr, "malloc_real at %p \n", malloc_real);
	calloc_real = dlsym(RTLD_NEXT, "calloc");
	fprintf(stderr, "calloc_real at %p \n", calloc_real);
	free_real = dlsym(RTLD_NEXT, "free");
	fprintf(stderr, "free_real at %p \n", free_real);
	fprintf(stderr, "init_hooking() call finish!!!\n");
}

void __attribute__((destructor)) finish_hooking()
{
	fprintf(stderr, "-----free check result----- \n");
	fprintf(stderr, "malloc was called #%lld \n", mallocCallNum);
	fprintf(stderr, "calloc was called #%lld \n", callocCallNum);
	fprintf(stderr, "free was called #%lld \n", freeCallNum);
	fprintf(stderr, "malloc + calloc #%lld \n", mallocCallNum + callocCallNum);
	fprintf(stderr, "not free memory #%lld \n", mallocCallNum + callocCallNum - freeCallNum);
	fprintf(stderr, "--------------------------- \n");
}

void* malloc(size_t size)
{
	void* ret = malloc_real(size);
	fprintf(stderr, "[malloc Call #%lld (%u)] %p \n", ++mallocCallNum, (unsigned int) size, ret);
	return ret;
}

void* calloc(size_t num, size_t size)
{
	void* ret = calloc_real(num,size);
	fprintf(stderr, "[calloc Call #%lld (%u, %u)] %p \n", ++callocCallNum, (unsigned int) num, (unsigned int) size, ret);
	return ret;
}

void free(void *ptr)
{
	if(ptr == NULL)return;
	free_real(ptr);
	fprintf(stderr, "[free Call #%lld] %p \n", ++freeCallNum, ptr);
}

