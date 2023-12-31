#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_BUCKETS 8


struct BucketNode {
    //Private Values
    struct BucketNode *__prev;
    struct BucketNode *__next;
    char *__key;
    int __value;
};

struct HashMap {
    //Private Values
    int __buckets;
    int __length;
    struct BucketNode *__heads[MAX_BUCKETS];
    struct BucketNode *__tails[MAX_BUCKETS];
    
    //Public Methods
    void (*put)(struct HashMap *this_hashmap, char *key, int value);
    void (*dump)(struct HashMap *this_hashmap);
    int (*get)(struct HashMap *this_hashmap, char *key, int fallback);
    int (*hash)(char *key);
    void (*del)(struct HashMap *this_hashmap);
    void (*buckets)(struct HashMap *this_hashmap);
};

int __HashMap_hash(char *key) {
    int hash = 0;
    for (char c = *key; c != '\0'; c = *++key) {
        //printf("%c\n", c);
        hash += c*3;
    }
    return hash % MAX_BUCKETS;
}

void __HashMap_del(struct HashMap *this) {
    for (int i = 0; i < MAX_BUCKETS; ++i) {
        struct BucketNode *current, *next;
        current = this->__heads[i];
        while(current) {
            free(current->__key);
            //printf("Key deleted.\n");
            next = current->__next;
            free(current);
            current = next;
            //printf("Node deleted.\n");
        }
    }
    free(this);
    this = NULL;
    //printf("Hashmap deleted\n");
}

int __HashMap_get(struct HashMap *this, char *key, int fallback) {
    for (int i = 0; i < MAX_BUCKETS; ++i) {
        for (struct BucketNode *current = this->__heads[i]; current != NULL; current = current->__next) {
            if (strcmp(current->__key, key) == 0) {
                //printf("current->__key: %s, key: %s\n", current->__key, key);
                return current->__value;
            }
        }
    }
    return fallback;
}

void __HashMap_put(struct HashMap *this, char *key, int value) {
    int key_exists = 0;
    for (int i = 0; i < MAX_BUCKETS; ++i) {
        for (struct BucketNode *current = this->__heads[i]; current != NULL; current = current->__next) {
            if (strcmp(current->__key, key) == 0) {
                //printf("current->__key: %s, key: %s\n", current->__key, key);
                key_exists = 1;
                current->__value = value;
            }
        }
    }

    if (key_exists == 0) {
        int bucket = this->hash(key);
        char *this_key = malloc(sizeof(key));
        strcpy(this_key, key);
        struct BucketNode *new_node = (struct BucketNode *) malloc(sizeof(*new_node));

        new_node->__key = this_key;
        new_node->__value = value;

        if (this->__heads[bucket] == NULL) {
            new_node->__prev = NULL;
            new_node->__next = NULL;
            this->__heads[bucket] = new_node;
            this->__tails[bucket] = new_node;
        } else {
            new_node->__prev = this->__tails[bucket];
            new_node->__next = NULL;
            this->__tails[bucket]->__next = new_node;
            this->__tails[bucket] = new_node;
        }
        this->__length += 1;
    }
}

void __HashMap_printbuckets(struct HashMap *this) {
    printf("\nTotal Length: %d\n", this->__length);
    int iter = 0;
    for (int i = 0; i < MAX_BUCKETS; ++i) {
        printf("Bucket: %d = ", i);
        for (struct BucketNode *current = this->__heads[i]; current != NULL; current = current->__next) {
            iter+=1;
            if (iter == this->__length) {
                printf("[\"%s\" : %d]\n", current->__key, current->__value);
            } else {
                printf("[\"%s\" : %d],  ", current->__key, current->__value);
            }
        }
        printf("\n");
    }
}

void __HashMap_dump(struct HashMap *this) {
    printf("(length=%d, [", this->__length);
    int iter = 0;
    for (int i = 0; i < MAX_BUCKETS; ++i) {
        for (struct BucketNode *current = this->__heads[i]; current != NULL; current = current->__next) {
            iter+=1;
            if (iter == this->__length) {
                printf("\"%s\" : %d", current->__key, current->__value);
            } else {
                printf("\"%s\" : %d, ", current->__key, current->__value);
            }
        }
    }
    printf("])\n");
}

struct HashMap *HashMap_new() {
    // Initialize new hashmap memory and heads and tails bucketnodes for each bucket
    struct HashMap *new = (struct HashMap *) malloc(sizeof(*new));
    new->__buckets = MAX_BUCKETS;
    new->__length = 0;
    for (int i = 0; i < MAX_BUCKETS; ++i) {
        new->__heads[i] = NULL;
        new->__tails[i] = NULL;
        printf("bucket: %d, &head: %p, &tail: %p\n", i, new->__heads[i], new->__tails[i]);
    }

    // Assign Internal Methods
    new->dump = &__HashMap_dump;
    new->put = &__HashMap_put;
    new->get = &__HashMap_get;
    new->hash = &__HashMap_hash;
    new->del = &__HashMap_del;
    new->buckets = &__HashMap_printbuckets;
    
    return new;
}

int main() {
    struct HashMap *my_hashmap = HashMap_new();

    my_hashmap->dump(my_hashmap);
    my_hashmap->put(my_hashmap, "key1", 1);
    my_hashmap->put(my_hashmap, "key2", 2);
    my_hashmap->put(my_hashmap, "key3", 3);
    my_hashmap->put(my_hashmap, "key3", 4);
    my_hashmap->put(my_hashmap, "key4", 4);
    my_hashmap->put(my_hashmap, "key5", 5);
    my_hashmap->put(my_hashmap, "key6", 6);
    my_hashmap->put(my_hashmap, "key7", 7);
    my_hashmap->put(my_hashmap, "key8", 8);
    my_hashmap->put(my_hashmap, "key9", 9);
    my_hashmap->put(my_hashmap, "key19", 19);
    my_hashmap->put(my_hashmap, "key29", 29);
    my_hashmap->put(my_hashmap, "key39", 39);
    my_hashmap->dump(my_hashmap);
    printf("get key3: %d\n", my_hashmap->get(my_hashmap, "key3", -1));
    printf("get key6: %d\n", my_hashmap->get(my_hashmap, "key6", -1));
    my_hashmap->buckets(my_hashmap);
    my_hashmap->del(my_hashmap);
    my_hashmap = NULL;
    printf("hashmap pointer: %p\n", my_hashmap);

    return 0;
}
