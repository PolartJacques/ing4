#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LOGICAL_SIZE 65536 //2^16
#define PHYSICAL_SIZE 16384 //2^14
#define PAGE_SIZE 256 //2^8

typedef int Address;
typedef struct {
  bool manager[LOGICAL_SIZE];
  int mapping_table[LOGICAL_SIZE / PAGE_SIZE];
} Logical_memory;

// RAM //

int RAM[PHYSICAL_SIZE];
bool ram_manager[PHYSICAL_SIZE / PAGE_SIZE];

// SECONDARY FUNCTIONS //

void init_ram_manager() {
  for (int i = 0; i < PHYSICAL_SIZE / PAGE_SIZE; i ++) ram_manager[i] = true;
}

void init_logical_memory(Logical_memory *memory) {
  for (int i = 0; i < LOGICAL_SIZE; i++) memory->manager[i] = true;
}

/**
 * @brief map pages of logical memory to pages of physical memory
 * 
 * @param memory logical memory used
 * @param address address returned by my_alloc
 * @param size size allocated
 */
void map_pages(Logical_memory *memory, Address address, int size) {
  int first_page = address / PAGE_SIZE;
  int last_page = (address + size - 1) / PAGE_SIZE;
  int page_to_map = first_page;
  for (int i = 0; i < PHYSICAL_SIZE / PAGE_SIZE; i++) {
    if (ram_manager[i] = true) {
      ram_manager[i] = false;
      memory->mapping_table[page_to_map] = i;
      if (page_to_map == last_page) return;
    }
  }
}

/**
 * @brief find contiguous free address using first fit algorithm
 * 
 * @param memory logical memort used
 * @param size size to allocate
 * @return Address first address of contiguous free address 
 */
Address first_fit(Logical_memory *memory, int size) {
  int counter = 0;
  int i;
  for (i = 0; i < LOGICAL_SIZE && counter < size; i++) {
    if (memory->manager[i]) counter++;
    else counter = 0;
  }
  if (counter < size) return -1;
  return i - size;
}

/**
 * @brief find contiguous free address using worst fit algorithm
 * 
 * @param memory logical memort used
 * @param size size to allocate
 * @return Address first address of contiguous free address 
 */
Address best_fit(Logical_memory *memory, int size) {
  Address best_hole_address = -1;
  int best_hole_size = LOGICAL_SIZE;
  Address hole_address = 0;
  int counter = 0;
  for (int i = 0; i < LOGICAL_SIZE; i++) {
    if (memory->manager[i]) {
      if (counter == 0) hole_address = i;
      counter++;
    }
    else {
      if (counter < best_hole_size && counter >= size) {
        best_hole_address = hole_address;
        best_hole_size = counter;
      }
      counter = 0;
    }
  }
  if (counter < best_hole_size && counter >= size) {
    best_hole_address = hole_address;
    best_hole_size = counter;
  }
  return best_hole_address;
}

/**
 * @brief find contiguous free address using worst fit algorithm
 * 
 * @param memory logical memort used
 * @param size size to allocate
 * @return Address first address of contiguous free address 
 */
Address worst_fit(Logical_memory *memory, int size) {
  Address worst_hole_address = -1;
  int worst_hole_size = 0;
  Address hole_address = 0;
  int counter = 0;
  for (int i = 0; i < LOGICAL_SIZE; i++) {
    if (memory->manager[i]) {
      if (counter == 0) hole_address = i;
      counter++;
    }
    else {
      if (counter > worst_hole_size) {
        worst_hole_address = hole_address;
        worst_hole_size = counter;
      }
      counter = 0;
    }
  }
  if (counter > worst_hole_size) {
    worst_hole_address = hole_address;
    worst_hole_size = counter;
  }
  return worst_hole_address;
}

/**
 * @brief convert logical address to physical address
 * 
 * @param memory memory used
 * @param address address returned by my_alloc
 * @return Address physical address
 */
Address convert_address(Logical_memory *memory, Address address) {
  int logical_page = address / PAGE_SIZE;
  int offset = address - logical_page;
  return memory->mapping_table[logical_page] + offset;
}

// MAIN FUNCTIONS //

Address my_alloc(Logical_memory *memory, int size) {
  // find first address
  Address first_address = first_fit(memory, size);
  // mark range as allocated
  for (int i = first_address; i < first_address + size; i++) memory->manager[i] = false;
  // map logical pages with physical pages
  map_pages(memory, first_address, size);
  return first_address;
}

void my_write(Logical_memory *memory, Address address, int value) {
  int physical_address = convert_address(memory, address);
  RAM[physical_address] = value;
}

int my_read(Logical_memory *memory, Address address) {
  int physical_address = convert_address(memory, address);
  return RAM[physical_address];
}

void my_free(Logical_memory *memory, Address address, int size) {
  // free logical memory manager
  for (int i = 0; i < address + size; i++) memory->manager[i] = true;
  // free ram manager
  int first_page = address / PAGE_SIZE;
  int last_page = (address + size - 1) / PAGE_SIZE;
  for (int i = first_page; i <= last_page; i++) {
    // check if page in logical memory is completely free to free it in the page in RAM
    bool page_is_free = true;
    for (int j = i * PAGE_SIZE; j < (i + 1) * PAGE_SIZE || page_is_free; j++) page_is_free = memory->manager[j];
    if (page_is_free) ram_manager[memory->mapping_table[i]] = true;
  }
}

// MAIN //

void main() {
  // init
  init_ram_manager();
  Logical_memory memory;
  init_logical_memory(&memory);

  // to test first fit, best fit and worst fit
  memory.manager[0] = false;
  memory.manager[5] = false;

  Address address = my_alloc(&memory, 3);
  printf("%i\n", address);
  
  my_write(&memory, address, 42);
  my_write(&memory, address + 1, 43);
  my_write(&memory, address + 2, 44);
  printf("%i\n", my_read(&memory, address));
  printf("%i\n", my_read(&memory, address + 1));
  printf("%i\n", my_read(&memory, address + 2));
  
  my_free(&memory, address, 3);
  
  Address address2 = my_alloc(&memory, 3);
  printf("%i\n", address);
}