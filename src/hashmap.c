#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

// Structure for each entry in the hashmap
typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

// Structure for the hashmap
typedef struct {
    Entry **table;
    int capacity;
    int size;
} HashMap;

int hash(char *key, int capacity);

HashMap *createHashMap();

void resizeHashMap(HashMap *map);

void insert(HashMap *map, char *key, int value);

int get(HashMap *map, char *key);

void destroyHashMap(HashMap *map);

int main() {
    HashMap *map = createHashMap();

    // Insert some key-value pairs
    insert(map, "apple", 5);
    insert(map, "banana", 10);
    insert(map, "orange", 7);

    // Retrieve values
    printf("Value of 'apple': %d\n", get(map, "apple"));
    printf("Value of 'banana': %d\n", get(map, "banana"));
    printf("Value of 'orange': %d\n", get(map, "orange"));
    printf("Value of 'grape': %d\n", get(map, "grape")); // Key not found, should return -1

    // Deallocate memory
    destroyHashMap(map);

    return 0;
}

// Hash function
int hash(char *key, int capacity) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (31 * hash + key[i]) % capacity;
    }
    return hash;
}

// Initialize a new hashmap
HashMap *createHashMap() {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL) {
        return NULL; // Memory allocation failed
    }

    map->capacity = INITIAL_CAPACITY;
    map->size = 0;
    map->table = (Entry **)calloc(map->capacity, sizeof(Entry *));
    if (map->table == NULL) {
        free(map); // Free hashmap memory if calloc fails
        return NULL;
    }
    
    return map;
}

// Resize the hashmap when the load factor exceeds the threshold
void resizeHashMap(HashMap *map) {
    int new_capacity = map->capacity * 2;
    Entry **new_table = (Entry **)calloc(new_capacity, sizeof(Entry *));
    if (new_table == NULL) {
        return; // Memory allocation failed
    }

    // Rehash existing entries into the new table
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->table[i];
        while (entry != NULL) {
            Entry *next = entry->next;
            int index = hash(entry->key, new_capacity);
            entry->next = new_table[index];
            new_table[index] = entry;
            entry = next;
        }
    }

    // Update the hashmap with the new table and capacity
    free(map->table);
    map->table = new_table;
    map->capacity = new_capacity;
}

// Insert key-value pair into the hashmap
void insert(HashMap *map, char *key, int value) {
    // Check load factor and resize if necessary
    if ((double)map->size / map->capacity >= LOAD_FACTOR_THRESHOLD) {
        resizeHashMap(map);
    }

    int index = hash(key, map->capacity);
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    if (entry == NULL) {
        return; // Memory allocation failed
    }
    entry->key = strdup(key); // Duplicate the key string
    entry->value = value;
    entry->next = NULL;

    // Insert entry into the table
    entry->next = map->table[index];
    map->table[index] = entry;
    map->size++;
}

// Retrieve value associated with a key
int get(HashMap *map, char *key) {
    int index = hash(key, map->capacity);
    Entry *entry = map->table[index];

    // Search the linked list for the key
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    // Key not found
    return -1;
}

// Deallocate memory used by the hashmap
void destroyHashMap(HashMap *map) {
    if (map == NULL) {
        return;
    }
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->table[i];
        while (entry != NULL) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}
