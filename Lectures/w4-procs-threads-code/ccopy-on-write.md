Copy-on-write (COW) is a memory-management optimization used by operating systems when creating new processes. Let's unpack it in your context:

⸻

What happens when a process forks?

-   When a parent process forks a child, the child initially gets the same memory contents as the parent.
-   Naïve approach: The OS could immediately copy all of the parent's memory into the child's address space.
-   This is very costly if the parent has a lot of memory allocated.
-   Worse, in many cases the child immediately replaces its memory using exec(), so the copied memory would be wasted.

⸻

What copy-on-write does

-   Instead of duplicating memory right away, the OS maps the child's virtual memory pages to point to the same physical pages as the parent.
-   Both processes can read from these shared pages without any issues.
-   These shared pages are marked read-only by the OS.
-   If either the parent or the child tries to write to one of these pages:
-   A page fault occurs.
-   The OS allocates a new physical page.
-   It copies the contents of the original page to the new page.
-   The process making the write now gets its own private copy to modify.
-   This way, only the pages that are actually written to are duplicated.

⸻

Why this is efficient

-   No wasted copies: If the child process calls exec() right after fork(), the parent's memory never needs to be duplicated.
-   Lazy copying: Memory pages are only copied if they are actually modified by either process.
-   Less overhead: Big processes can fork without incurring the cost of duplicating large amounts of memory immediately.

⸻

Addressing your context

You mentioned "parent and child processes have no shared writable memory."

-   That's exactly what COW ensures. Even though they start by sharing physical pages, those pages are marked read-only.
-   As soon as one process tries to write, the OS copies the page, so after that, each process has its own version.
-   Thus, from the process's perspective, they never share writable memory - but the OS saves work by deferring copying until necessary.

⸻

✅ In short:
Copy-on-write is a technique where the OS delays copying memory pages during fork() until one of the processes tries to modify them. This is efficient because it avoids unnecessary copying, especially when the child will quickly call exec() or when most pages are never written to.

Would you like me to also explain how COW is implemented at the page table / MMU level (with reference bits and protection flags), or do you want to keep it at the high-level OS concept?
