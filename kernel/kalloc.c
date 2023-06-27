// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

// PHYSTOP >> 12 is equal to memory size divided by
// 2 ^ 12. this will say that how many pages of size
// 2 ^ 12 (aka 4096 bites) do we have.
struct{
    struct spinlock lock;
    int count[PHYSTOP >> 12];
} kref;

#define PA2PN(pa) (((uint64)pa) >> 12)

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  init_lock();
  init_kref();
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP) {
      panic("kfree");
  }

    // checking if the references to a page were 0 or lesser,
    // then free it otherwise do nothing.
  acquire(&kref.lock);
  decrement_kref(pa);
  if (kref.count[PA2PN(pa)] > 0) {
      release(&kref.lock);
      return;
  }
  set_kref(pa, 0);
  release(&kref.lock);

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r) {
      memset((char *) r, 5, PGSIZE); // fill with junk

      acquire(&kref.lock);
      set_kref((void*)r, 1);
      release(&kref.lock);
  }
  return (void*)r;
}


// user defined functions
uint64
free_memory(){
    struct run *r;
    uint64 free_memory=0;
    r = kmem.freelist;
    acquire(&kmem.lock);
    while(r){
        free_memory += PGSIZE;
        r = r->next;
    }
    release(&kmem.lock);
    return free_memory;
}

void init_lock(){
    initlock(&kref.lock, "kref");
}

void init_kref(){
    acquire(&kref.lock);
    for (int i = 0;i < (PHYSTOP >> 12);i++)
        kref.count[i] = 0;
    release(&kref.lock);
}

void increment_kref(void* pa){
    acquire(&kref.lock);
    kref.count[PA2PN(pa)]++;
    release(&kref.lock);
}

void decrement_kref(void* pa){
    kref.count[PA2PN(pa)]--;
}

void set_kref(void *pa, int number){
    kref.count[PA2PN(pa)] = number;
}
