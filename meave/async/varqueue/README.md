Variable Length (Growing) Non-Blocking Queue
======

It is a vector of queues. Every queue contain index of the first item to read, index of the first item to write and pointer to the items.

Vector contains M queues, size of the i-th queue is: `SIZE_0 * 4**i`, where `SIZE_0` is a size of the first queue. So queues sizes are: 4kB, 16kb, 64kB, 256kB, 1MB, 4MB, 16MB, 64MB, 256MB, 1GB, 4GB. We need 11 queues to have a queue that can grow from 4kB to 4GB.

Where is index of the queue that is used for reading (`rq_index`) and index of a queue that is used for writing (`wq_index`). It holds, that:

    0 <= rq_index <= wq_index
    
If producer wants to write to a queue and it is full, then it increase `wq_index`, if it doesn't have the maximum value.

If queue is empty and `rq_index < wq_index`, then consumer increases `rq_index`.
    
Pointers to the queues are empty at the beginning. If consumer wants to write to a queue with null pointer, than it tries to allocate it (more consumers can try it and only one wins). If producer wants to read from a queue with a bigger index, that it stores null to the pointer to a smaller queue and frees the memory (more consumer can combat for the pointer).

Queue Class
-----------

    struct SubQueue
    {
        uint64_t head_, tail_;
        T* q_;
    };

    struct Q {
        SubQueue *queus_;
        uns rq_index_, wq_index_;
    };


Algorithm
---------

* Reader:
  * Choose queue with index `rq_index`.

        for (;;)
            while(queue[rq_index] is empty)
                if (rq_queue < wq_queue)
                    try to free the queue
                    expected = rq_index
                    rq_index.compare_exchange(expected, expected + 1)
                else
                    wait;
            if (attempt to store the element as with fixed-length queue succeed)
                return;

* Writer:

        for (;;)
            while(queue[wq_index] is full)
                if (wq_queue < max_index)
                    expected = wq_index
                    wq_index.compare_exchange(expected, expected + 1)
                    if (pointer to queue is null)
                        p = allocate memory
                        q.compare_exchange(null, p)
                        if (q != p)
                            free p
                else
                    wait;
            if (attempt to store the element as with fixed-length queue succeed)
                return;
            
  * It would be probably possible to do the attempts for store/load in a cycle without repeating the whole cycle for every attempt.
  * One can also remember last non-empty/non-full queue from the previous turn to avoid scanning of queues from the beginning.


Drawbacks
---------

* Queue size cannot shrink.
* If consumer don't accept messages grow queue will grow to a big size and will consume a lot of memory.
* Producers can spend some time with memory allocation and consumers with freeing of the memory.
