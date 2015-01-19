#include <stdio.h>

void dfs(int depth) {
  if (depth == 0) {
    printf("Reach the end\n");
    return;
  }
  dfs(depth- 1);
}

int main() {
   int depth = 1000000;
   dfs(depth);
}

// ulimit -s 8196 (8k, default)
// ulimit -s 4194304 (4M)
