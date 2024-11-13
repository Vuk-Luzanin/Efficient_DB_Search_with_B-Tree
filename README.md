# Efficient Data Search in B-Tree Indexed Database Tables 🔍🌳

This repository demonstrates an efficient way to search data from multiple database tables using B-trees for indexing. The B-tree indexing structure helps optimize search, insertion, and deletion operations in large datasets, ensuring fast data retrieval.

## 🚀 Features

- **B-tree indexing** for efficient searching.
- **Multiple table support** for querying and combining data from different tables.
- **Optimized performance** for large datasets, minimizing the time complexity of queries.

## ⚙️ How It Works

The system uses a B-tree structure to index data from multiple database tables. This allows for efficient searching by reducing the number of comparisons required compared to a linear search. The B-tree ensures that the database queries are processed quickly, even with large amounts of data.

### B-Tree Structure
- **Balanced tree** with sorted nodes.
- Each node contains keys and pointers to child nodes, ensuring efficient searching and retrieval.

### Supported Operations
- **Insert**: Adding new data to the table with B-tree indexing.
- **Search**: Fast lookup of data based on indexed values.
- **Delete**: Efficient removal of data while maintaining tree balance.

### Time Complexity of B-Tree Operations

- **Search**: 
  - Time complexity: **O(log n)**
  - Searching in a B-tree involves traversing from the root to the leaf node. Since the tree is balanced, the number of levels (height) is logarithmic in relation to the number of elements, making search operations efficient even in large datasets.

- **Insert**: 
  - Time complexity: **O(log n)**
  - Insertion in a B-tree requires finding the correct position for the new element (which takes logarithmic time) and possibly splitting nodes to maintain balance. Since the tree is self-balancing, insertion remains efficient.

- **Delete**: 
  - Time complexity: **O(log n)**
  - Deletion requires finding the element, removing it, and rebalancing the tree if necessary. Like insertion, this operation involves logarithmic time complexity because the tree remains balanced after each delete operation.

### Overall Efficiency
- **Logarithmic growth**: The height of a B-tree increases logarithmically with the number of elements. This ensures that as the dataset grows, the time complexity for search, insert, and delete operations remains efficient and manageable, even for large-scale datasets.
- **Space Complexity**: 
  - The space complexity is **O(n)**, where **n** is the number of elements. Each node in the tree holds several keys and pointers, making B-trees efficient in terms of space usage compared to other tree structures.

B-trees are well-suited for applications with large datasets where quick access, insertion, and deletion of data are required, such as in databases and file systems.

