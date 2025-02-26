Search git repos on filesystem

```
❯ clang -O3 -flto -march=native -DNDEBUG  main.c  && time ./a.out

________________________________________________________
Executed in  272.12 millis    fish           external
   usr time    6.66 millis   36.00 micros    6.62 millis
   sys time   74.51 millis  318.00 micros   74.19 millis
```

### TODO:
- MAX_NESTING_LEVEL configurable?
- Ignore hidden files?
