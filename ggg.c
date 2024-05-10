#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Memory block structure
struct MemoryBlock {
    int size;
    bool allocated; // true if allocated, false if free
};

// Process structure
struct Process {
    char name[20]; // Process name
    int size; // Process size
    bool allocated; // true if allocated, false if not
};

// Function to initialize memory
void initializeMemory(struct MemoryBlock *memoryMap, int numUnits) {
    for (int i = 0; i < numUnits; i++) {
        memoryMap[i].size = 1;
        memoryMap[i].allocated = false; // All blocks are initially free
    }
}

// Function to display memory status
void displayMemoryStatus(const struct MemoryBlock *memoryMap, int numUnits) {
    printf("Memory Status:\n");
    printf("Unit\tSize\tAllocated\n");
    for (int i = 0; i < numUnits; i++) {
        printf("%d\t%d\t%s\n", i + 1, memoryMap[i].size,
               (memoryMap[i].allocated ? "Yes" : "No"));
    }
}

// Function to find a suitable free memory block using First Fit strategy
int findFreeBlockFirstFit(const struct MemoryBlock *memoryMap, int numUnits, int processSize) {
    int blockSize = 0;
    for (int i = 0; i < numUnits; i++) {
        if (!memoryMap[i].allocated) {
            blockSize += memoryMap[i].size;
            if (blockSize >= processSize) {
                return i - (blockSize != processSize ? 0 : 1);
            }
        } else {
            blockSize = 0;
        }
    }
    return -1;
}

// Function to allocate memory to a process using First Fit strategy
void allocateMemory(struct MemoryBlock *memoryMap, int numUnits, struct Process *process) {
    int blockIndex = findFreeBlockFirstFit(memoryMap, numUnits, process->size);
    if (blockIndex != -1) {
        for (int i = blockIndex; i < blockIndex + process->size; i++) {
            memoryMap[i].allocated = true; // Mark the block as allocated
        }
        process->allocated = true;
        printf("Allocated memory for process %s from unit %d to unit %d\n", process->name, blockIndex + 1,
               blockIndex + process->size);
    } else {
        printf("Failed to allocate memory for process %s due to insufficient memory\n", process->name);
    }
}

// Function to deallocate memory
void deallocateMemory(struct MemoryBlock *memoryMap, int numUnits, struct Process *process) {
    for (int i = 0; i < numUnits; i++) {
        if (memoryMap[i].allocated) {
            memoryMap[i].allocated = false; // Mark the block as free
        }
    }
    process->allocated = false;
    printf("Deallocated memory for process %s\n", process->name);
}

// Function to display process state
void displayProcessState(const struct Process *processes, int numProcesses) {
    printf("Process State:\n");
    printf("Name\tSize\tAllocated\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("%s\t%d\t%s\n", processes[i].name, processes[i].size,
               (processes[i].allocated ? "Yes" : "No"));
    }
}

int main() {
    int memorySize, unitSize, numUnits;
    printf("Enter total memory size: ");
    scanf("%d", &memorySize);
    printf("Enter size of one unit: ");
    scanf("%d", &unitSize);

    numUnits = memorySize / unitSize;
    struct MemoryBlock *memoryMap = (struct MemoryBlock *)malloc(numUnits * sizeof(struct MemoryBlock));
    if (memoryMap == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    initializeMemory(memoryMap, numUnits); // Initialize memory

    int numProcesses;
    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);

    struct Process *processes = (struct Process *)malloc(numProcesses * sizeof(struct Process));
    if (processes == NULL) {
        printf("Memory allocation failed\n");
        free(memoryMap);
        return 1;
    }

    // Input process information
    for (int i = 0; i < numProcesses; i++) {
        printf("Enter name of process %d: ", i + 1);
        scanf("%s", processes[i].name);
        printf("Enter size of process %d (in Units): ", i + 1);
        scanf("%d", &processes[i].size);
        processes[i].allocated = false;
    }

    int choice, processIndex;
    do {
        printf("\nMemory Allocation Menu:\n");
        printf("1. Allocate Memory for Process\n");
        printf("2. Deallocate Memory for Process\n");
        printf("3. Display Memory Status\n");
        printf("4. Display Process State\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter process index (1-%d): ", numProcesses);
                scanf("%d", &processIndex);
                if (processIndex < 1 || processIndex > numProcesses) {
                    printf("Invalid process index\n");
                    break;
                }
                allocateMemory(memoryMap, numUnits, &processes[processIndex - 1]);
                break;
            case 2:
                printf("Enter process index (1-%d): ", numProcesses);
                scanf("%d", &processIndex);
                if (processIndex < 1 || processIndex > numProcesses) {
                    printf("Invalid process index\n");
                    break;
                }
                deallocateMemory(memoryMap, numUnits, &processes[processIndex - 1]);
                break;
            case 3:
                displayMemoryStatus(memoryMap, numUnits);
                break;
            case 4:
                displayProcessState(processes, numProcesses);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    } while (choice != 5);

    free(memoryMap);
    free(processes);
    return 0;
}
