#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef __MMU__H__

#define __MMU__H__
#define SIZE 65536

typedef short byte_t;
typedef int address_t;

typedef struct hole {
  address_t adr;
  int sz;
  struct hole *next;
  struct hole *prev;
} hole_t;

typedef struct {
  byte_t mem[SIZE];
  hole_t root; // first hole
} mem_t;

// dynamically allocates a mem_t structure and initializes its content
mem_t *initMem() {
  mem_t *mem = malloc(sizeof(mem_t));
  mem->root.adr = 0;
  mem->root.next = &mem->root;
  mem->root.prev = &mem->root;
  mem->root.sz = SIZE;
  return mem;
}

address_t first_fit(mem_t *mp, int sz) {
  hole_t *this_hole = &mp->root;
  do {
    if (this_hole->sz >= sz) {
      address_t address = this_hole->adr;
      this_hole->sz = this_hole->sz - sz;
      this_hole->adr = this_hole->adr + sz;
      if (this_hole->sz == 0) {
        this_hole->prev->next = this_hole->next;
        if (this_hole != &mp->root) free(this_hole);
      } 
      return address;
    }
  } while (this_hole->next != &mp->root);
}

// allocates space in bytes (byte_t) using First-Fit, Best-Fit or Worst-Fit
address_t myAlloc(mem_t *mp, int sz) {
  return first_fit(mp, sz);
}

// release memory that has already been allocated previously
void myFree(mem_t *mp, address_t p, int sz) {
  hole_t *this_hole = &mp->root;
  bool has_been_freed = false;
  do {
    if (this_hole->adr + this_hole->sz == p) {
      // if space to free is contiguous with current hole, juste extend current hole
      this_hole->sz += sz;
      has_been_freed = true;
    } else if (this_hole->adr == this_hole->prev->adr + this_hole->prev->sz) {
      // if previus hole is contiguous with current hole, merge them
      this_hole->prev->sz += this_hole->sz;
      this_hole->prev->next = this_hole->next;
      free(this_hole);
    }
    this_hole = this_hole->next;
  } while (this_hole->adr + this_hole->sz < p);
  if (!has_been_freed) {
    // if space has'nt been freed yet, create a new hole
    hole_t *new_hole = malloc(sizeof(hole_t));
    new_hole->adr = p;
    new_hole->sz = sz;
    new_hole->next = this_hole;
    new_hole->prev = this_hole->prev;
    this_hole->prev->next = new_hole;
    this_hole->prev = new_hole;
  }
}

// assign a value to a byte
void myWrite(mem_t *mp, address_t p, byte_t val) {
  mp->mem[p] = val;
}

// read memory from a byte
byte_t myRead(mem_t *mp, address_t p) {
  return mp->mem[p];
}

#endif

int main() {
  mem_t *mem = initMem();
  address_t adr1 = myAlloc(mem, 5);
  address_t adr2 = myAlloc(mem, 10);
  address_t adr3 = myAlloc(mem, 100);
  myFree(mem, adr2, 10);
  myFree(mem, adr1, 5);
  myWrite(mem, adr3, 543); // write on the 1 st byte
  myWrite(mem, adr3+9, 34); // write on the 10th byte
  byte_t val1 = myRead(mem, adr3);
  byte_t val2 = myRead(mem, adr3+9);
  printf("%i\n", val1);
  printf("%i\n", val2);
}