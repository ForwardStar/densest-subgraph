## Densest Subgraph Discovery
A fast algorithm with high approximation to find densest subgraphs.

How to use it:
- Generate undirected graph data. Refer to ``datasets`` folder for example. The first line is a single number `n` representing the number of vertices. Following are `n` lines. The `i`-th line starts with `i-1`, corresponded with several numbers `j` as edges between `i-1` and `j`.
- Build from source:
  ```sh
  make
  ```
- Run the binary file:
  ```sh
  ./main $(DATA_DIR)
  ```