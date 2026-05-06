/* All tests are AI generated, their purpose is to check the functionality of the code and treshold values */

#include <iostream>

int main() {
    init();

    std::cout << "=== BASIC TEST ===\n";
    {
        Q *q0 = create_queue();
        enqueue_byte(q0, 10);
        enqueue_byte(q0, 20);
        enqueue_byte(q0, 30);

        printf("%d ", dequeue_byte(q0)); // 10
        printf("%d ", dequeue_byte(q0)); // 20
        printf("%d\n", dequeue_byte(q0)); // 30

        destroy_queue(q0);
    }

    std::cout << "=== MULTI QUEUE TEST ===\n";
    {
        Q *q0 = create_queue();
        Q *q1 = create_queue();

        enqueue_byte(q0, 1);
        enqueue_byte(q1, 2);
        enqueue_byte(q0, 3);
        enqueue_byte(q1, 4);

        printf("%d %d\n", dequeue_byte(q0), dequeue_byte(q1)); // 1 2
        printf("%d %d\n", dequeue_byte(q0), dequeue_byte(q1)); // 3 4

        destroy_queue(q0);
        destroy_queue(q1);
    }

    std::cout << "=== BOUNDARY (BLOCK SPLIT) TEST ===\n";
    {
        Q *q = create_queue();

        // filling over the border (BLOCK_SIZE ~ 8 in your code)
        for(int i = 0; i < 20; i++) {
            enqueue_byte(q, i);
        }

        for(int i = 0; i < 20; i++) {
            printf("%d ", dequeue_byte(q));
        }
        printf("\n");

        destroy_queue(q);
    }

    std::cout << "=== INTERLEAVE TEST ===\n";
    {
        Q *q0 = create_queue();
        Q *q1 = create_queue();

        for(int i = 0; i < 10; i++) {
            enqueue_byte(q0, i);
            enqueue_byte(q1, i + 100);
        }

        for(int i = 0; i < 10; i++) {
            printf("%d %d | ", dequeue_byte(q0), dequeue_byte(q1));
        }
        printf("\n");

        destroy_queue(q0);
        destroy_queue(q1);
    }

    std::cout << "=== FREE LIST REUSE TEST ===\n";
    {
        Q *q0 = create_queue();

        for(int i = 0; i < 50; i++) {
            enqueue_byte(q0, i);
        }

        for(int i = 0; i < 25; i++) {
            dequeue_byte(q0);
        }

        for(int i = 100; i < 130; i++) {
            enqueue_byte(q0, i);
        }

        int remaining = 50 - 25 + 30; // 55
        for(int i = 0; i < remaining; i++) {
            printf("%d ", dequeue_byte(q0));
        }
        printf("\n");

        destroy_queue(q0);
    }

    std::cout << "=== EMPTY EDGE TEST ===\n";
    {
        Q *q = create_queue();

        enqueue_byte(q, 42);
        printf("%d\n", dequeue_byte(q));

        // empty queue
        // next dequeue should call illegal op
        // printf("%d\n", dequeue_byte(q)); // commented on purpose

        destroy_queue(q);
    }

    std::cout << "\n=== REUSE SAME QUEUE ===\n";
    {
        Q *q = create_queue();

        for(int i = 0; i < 10; i++){
            enqueue_byte(q, i);
        }

        for(int i = 0; i < 10; i++){
            dequeue_byte(q);
        }

        // queue empty -> reuse
        for(int i = 100; i < 110; i++){
            enqueue_byte(q, i);
        }

        for(int i = 0; i < 10; i++){
            printf("%d ", dequeue_byte(q));
        }
        printf("\n");

        destroy_queue(q);
    }

    std::cout << "\n=== PING-PONG ===\n";
    {
        Q *q = create_queue();

        for(int i = 0; i < 50; i++){
            enqueue_byte(q, i);
            printf("%d ", dequeue_byte(q));
        }
        printf("\n");

        destroy_queue(q);
    }

    std::cout << "\n=== RANDOM OPS ===\n";
    {
        Q *q = create_queue();

        for(int i = 0; i < 200; i++){
            if(rand() % 2){
                enqueue_byte(q, i);
            } else if(q[4]){
                printf("%d ", dequeue_byte(q));
            }
        }
        printf("\n");

        destroy_queue(q);
    }

    std::cout << "=== DONE ===\n";

    std::cout << "\n=== FULL MEMORY ===\n";
    {
        Q *q = create_queue();

        // wants to fill the whole memory
        int i = 0;
        while(true){
            enqueue_byte(q, i++);
        }

        // should call out_of_memory
    }
    return 0;
}