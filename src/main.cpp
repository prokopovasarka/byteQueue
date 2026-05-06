#include <iostream>

#define MEM_MAX 2048
#define BLOCKS_NUMBER 255
#define BLOCK_SIZE 8
#define END_OF_QUEUE 255

unsigned char data[MEM_MAX];
#define FREE_LIST data[MEM_MAX - 1] // start of the free list in last unused block

typedef unsigned char Q;

void on_out_of_memory(){
    std::cout << "!Out of memory!" << std::endl;
    abort();
}

void on_illegal_operation(){
    std::cout << "!Illegal operation!" << std::endl;
    abort();
}

// Allocates block from free list
unsigned char get_block(){
    if(FREE_LIST == END_OF_QUEUE) return END_OF_QUEUE;
    unsigned char idx = FREE_LIST;
    auto ptr =  data + idx * BLOCK_SIZE;
    FREE_LIST   = ptr[0];
    ptr[0] = END_OF_QUEUE;
    return idx;
}

// Adds block to free list
void free_block(unsigned char *block){
    block[0] = FREE_LIST;
    FREE_LIST = (block - data) / BLOCK_SIZE;
}

void init(){
    for(unsigned i = 0; i < BLOCKS_NUMBER-1; i++){
        data[i * BLOCK_SIZE] = i+1;
    }
    data[(BLOCKS_NUMBER-1)*BLOCK_SIZE] = END_OF_QUEUE;
    FREE_LIST = 0;
}

// Creates a FIFO byte queue, returning a handle to it.
Q *create_queue(){
    if(FREE_LIST == END_OF_QUEUE){
        on_out_of_memory();
    }

    auto idx_head =  get_block();
    auto idx_first =  get_block();
    auto ptr =  data + idx_head * BLOCK_SIZE;

    ptr[0] = idx_first;
    ptr[1] = 1; // head offset
    ptr[2] = 1; // tail offset
    ptr[3] = idx_first; // tail index for O(1) search
    ptr[4] = 0; // empty flag

    return ptr;
}

// Destroy an earlier created byte queue.
void destroy_queue(Q *q){
    if(!q){
        on_illegal_operation();
    }

    auto block = data + q[0] * BLOCK_SIZE;
    while(block != nullptr){
        auto next = block[0];
        free_block(block);
        if(next == END_OF_QUEUE){
            block = nullptr;
        }else{
            block = data + next * BLOCK_SIZE;
        }
    }
    free_block(q);
}

// Adds a new byte to a queue.
void enqueue_byte(Q *q, unsigned char b){
    if(!q){
        on_illegal_operation();
    }

    auto tail_offset = q[2];
    auto tail_ptr = data + q[3] * BLOCK_SIZE;

    tail_ptr[tail_offset] = b;
    q[2]++;
    q[4] = 1;

    if(q[2] == BLOCK_SIZE){
        auto new_block_idx = get_block();
        if(new_block_idx == END_OF_QUEUE){
            on_out_of_memory();
        }
        tail_ptr[0] = new_block_idx;
        q[3] = new_block_idx;
        q[2] = 1;
    }
}

// Pops the next byte off the FIFO queue.
unsigned char dequeue_byte(Q *q) {
    if(!q || q[4] == 0){
        on_illegal_operation();
    }

    auto next_ptr = data + q[0] * BLOCK_SIZE;
    auto head_offset = q[1];

    auto b = next_ptr[head_offset];
    q[1]++;

    if(q[1] == BLOCK_SIZE){ // end of block
        if(q[0] != q[3]){
            auto next = next_ptr[0];
            free_block(next_ptr);
            q[0] = next;
        }
        q[1] = 1;
    }

    if(q[1] == q[2] && q[0] == q[3]){ // empty queue
        q[4] = 0;
    }

    return b;
}

int main() {
    init();

    std::cout << "Test:" << std::endl;
    Q *q0 = create_queue();
    enqueue_byte(q0, 0);
    enqueue_byte(q0, 1);
    Q *q1 = create_queue();
    enqueue_byte(q1, 3);
    enqueue_byte(q0, 2);
    enqueue_byte(q1, 4);
    printf("%d", dequeue_byte(q0));
    printf("%d\n", dequeue_byte(q0));
    enqueue_byte(q0, 5);
    enqueue_byte(q1, 6);
    printf("%d", dequeue_byte(q0));
    printf("%d\n", dequeue_byte(q0));
    destroy_queue(q0);
    printf("%d", dequeue_byte(q1));
    printf("%d", dequeue_byte(q1));
    printf("%d\n", dequeue_byte(q1));
    destroy_queue(q1);

    return 0;
}
