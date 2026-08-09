# archive

Here is a list of functions which are redundant/archived,
which where previously used, so if you wanted to know something here it is:)

## dfa.c

### why it has been archived

Due to the state_t struct and the state_next function,
which does pretty much the same, the is_friendly function has been heavily
simplified :)

```C
bool is_friendly(const char *digits) {
  bool z = false;
  size_t counter = 0;
  size_t i = 0;
  size_t next_promise = 0;
  while (digits[i] != '\0') {
    if ((counter % 10) == 0 && counter != 0) {
      i = ++next_promise;
      counter = 0;
    }
    counter += digits[i] - '0';
    i++;
  }
  if ((counter % 10) == 0 && counter != 0) {
    z = true;
  }
  return z;
}

```

## why this has been archived

This was just for practice

```C
int bfs_state(state_t *states_out) {
    int count = 0;
    int head = 0, tail = 0;
    state_t queue[1024];
    bool visited[1024] = {false};

    queue[tail++] = state_new();
    while(head < tail){
        state_t cur = queue[head++];
        states_out[count++] = cur;
        for(int i = 0; i< 9; i++){
            state_t nxt = state_next(cur, i);
            if(!visited[hash[nxt]]){
                visited[hash[nxt]] = true;
            }
            // does this no matter what
            queue[tail++] = nxt;
        }
    }


}

```
