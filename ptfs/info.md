# MAKE A (simple) PAGE TABLE FROM SCRATCH

## 1. Step gather information

### my plan

- read:
  - wikipedia article
  - art of unix programming 1
  - OSTEP 13-16 on virt-memory
  - maybe APUE chapter 7 and 14;
- create plan:
  - first implement a very very simple page table with following assumptions:
    //this is nearly the same as the memory pool i created
    - address spaces must be places continusly
    - sizeof addressspace < sizeof physicalmemory
    - each addressspace is exactly the same size
      physical address = virtual adress + base
