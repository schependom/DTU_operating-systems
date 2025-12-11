#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>

#define N 100
#define MSG_SIZE sizeof(message)
#define Q_PROD_NAME "/msgq_producer"
#define Q_CONS_NAME "/msgq_consumer"

typedef struct {
    int item;
} message;

// Global queue descriptors
mqd_t q_prod, q_cons;

void build_message(message *m, int item) {
    m->item = item;
}

int extract_item(message *m) {
    return m->item;
}

void consume_item(int item) {
    printf("Consumed item: %d\n", item);
}

int produce_item() {
    return rand() % 100;
}

// Wrapper for standard POSIX mq_send
void send_msg(mqd_t q_handle, message *m) {
    if (mq_send(q_handle, (char *)m, MSG_SIZE, 0) == -1) {
        perror("mq_send");
        exit(1);
    }
}

// Wrapper for standard POSIX mq_receive
void receive_msg(mqd_t q_handle, message *m) {
    if (mq_receive(q_handle, (char *)m, MSG_SIZE + 10, NULL) == -1) {
        perror("mq_receive");
        exit(1);
    }
}

void producer(void) {
    int item; 
    message m;

    // printf("Producer started...\n");
    while (1) {
        item = produce_item();
        
        // Wait for an "empty slot" token from consumer
        // In the original logic: receive(producer, &m) -> receive on MY queue
        receive_msg(q_prod, &m); 
        
        build_message(&m, item);
        
        // Send data to consumer
        send_msg(q_cons, &m); 
        // printf("Produced: %d\n", item); // Optional debug
    }
}

void consumer(void) {
    int item, i;
    message m;

    // printf("Consumer started... Sending %d tokens.\n", N);
    
    // Send N "empty" messages to producer to start the flow
    // This acts like initializing a semaphore to N
    for (i = 0; i < N; i++) {
        send_msg(q_prod, &m); 
    }

    while (1) {
        // Wait for data message from producer
        receive_msg(q_cons, &m);
        
        item = extract_item(&m);
        
        // Send "empty slot" token back to producer
        send_msg(q_prod, &m); 
        
        consume_item(item);
        
        // Sleep to make output readable
        usleep(100000); 
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // 1. Define Queue Attributes
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;      // Max messages in queue
    attr.mq_msgsize = sizeof(message); // Max size of one message
    attr.mq_curmsgs = 0;

    // 2. Clean up old queues (if program crashed previously)
    mq_unlink(Q_PROD_NAME);
    mq_unlink(Q_CONS_NAME);

    // 3. Create the queues
    // Producer reads from q_prod, Consumer reads from q_cons
    q_prod = mq_open(Q_PROD_NAME, O_CREAT | O_RDWR, 0644, &attr);
    q_cons = mq_open(Q_CONS_NAME, O_CREAT | O_RDWR, 0644, &attr);

    if (q_prod == (mqd_t)-1 || q_cons == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    // 4. Create Processes
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child Process -> Runs Consumer
        consumer();
    } else {
        // Parent Process -> Runs Producer
        producer();
        
        // Parent waits (technically unreachable due to while(1))
        wait(NULL); 
    }

    // Cleanup (Unreachable code in this infinite loop example)
    mq_close(q_prod);
    mq_close(q_cons);
    mq_unlink(Q_PROD_NAME);
    mq_unlink(Q_CONS_NAME);

    return 0;
}