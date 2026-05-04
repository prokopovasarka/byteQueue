#include <iostream>

#define MEM_MAX 2048
#define BLOCK_SIZE 32

unsigned int ctr = 0;

unsigned char data[MEM_MAX];

typedef unsigned char Q;

void on_out_of_memory(){
    std::cout << "!Out of memory!" << std::endl;
    abort();
}

void on_illegal_operation(){
    std::cout << "!Illegal operation!" << std::endl;
    abort();
}

// Creates a FIFO byte queue, returning a handle to it.
Q *create_queue(){
    if(ctr == 64){
        on_out_of_memory();
    }

    Q *queue = data + ctr*BLOCK_SIZE;
    ctr++;

    queue[0] = 0; //head
    queue[1] = 0; //tail

    return queue;
}

// Destroy an earlier created byte queue.
void destroy_queue(Q *q){
    if(!q){
        on_illegal_operation();
    }

    Q *block = q;

    for (unsigned i = 0; i < BLOCK_SIZE; i++) {
        block[i] = 0;
    }
}

// Adds a new byte to a queue.
void enqueue_byte(Q *q, unsigned char b){
    if(!q){
        on_illegal_operation();
    }

    auto tail = q[1];

    q[2 + tail] = b;
    q[1]++;
}

// Pops the next byte off the FIFO queue.
unsigned char dequeue_byte(Q *q) {
    if(!q){
        on_illegal_operation();
    }

    auto head = q[0];
    auto popped = q[ 2 + head];
    q[ 2 + head] = 0;
    q[0]++;

    return popped;
}

int main() {
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
