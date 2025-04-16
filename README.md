# Operating Systems Project: Multi-Level Reader-Writer Synchronization and Memory Management  
**Course:** Spring 2024  
**Professor:** Mohamed KHALGUI  
**Teaching Assistant:** Ahmed Sameh (`ahmed.al-shabrawi@guc.edu.eg`)  

---

## 📌 Project Overview  
This project implements a **multi-level reader-writer synchronization system** and a **memory management simulator** in **C**, addressing three milestones:  
1. **Priority-based Reader-Writer Synchronization** (Semaphores/Mutexes).  
2. **Memory Management Simulator** (Segmentation vs. Paging).  
3. **Generalized 3-Level Reader-Writer System** (Multi-threaded data flow).  

---

## 🛠️ Milestones & Deliverables  

### **Milestone 1: Inter-Process Communication (IPC) & Synchronization**  
**Goal:** Implement two synchronization schemes for shared memory access in **C**:  
- **Writer-Priority:** Writers get exclusive access; readers wait.  
- **Reader-Priority:** Multiple readers access concurrently; writers block readers.  

**Key Components:**  
- POSIX semaphores (`sem_init`, `sem_wait`, `sem_post`).  
- Mutex locks (`pthread_mutex_t`).  
- Shared memory (`shmget`, `shmat`).  

---

### **Milestone 2: Memory Management Simulator**  
**Goal:** Simulate segmentation and paging in **C**, highlighting fragmentation issues.  

**Features:**  
1. **Segmentation:**  
   - Logical divisions (code/data/stack) using `struct segment`.  
   - External fragmentation analysis (e.g., memory gaps).  
2. **Paging:**  
   - Fixed-size pages with a page table (`struct page_table_entry`).  
   - Internal fragmentation metrics (unused page space).  

**Algorithms:**  
- First-Fit/Best-Fit for segmentation.  
- Page replacement (optional: FIFO/LRU).  

---

### **Milestone 3: Multi-Level Reader-Writer System**  
**Goal:** Three-level data pipeline in **C**:  
1. **Level 1:** 3 writers → Shared Memory A.  
2. **Level 2:** 1 reader (aggregates data) → Shared Memory B.  
3. **Level 3:** 3 readers → Process data from Memory B.  

**Synchronization Challenges:**  
- Thread-safe shared memory access (`pthreads`).  
- Deadlock prevention (e.g., lock ordering).  

---

## 🔧 Compilation & Execution

To compile and run the **Milestone 1** reader-writer synchronization program, follow these steps:

### 🔨 Compilation

From inside the `main_files` directory, run:

```bash
gcc -pthread milestone_1_reader_writer_sync.c -o program.exe
