### Thin Bridge Crossing Simulation 

This C program simulates cars crossing a thin bridge between two sides of a river, adhering to the constraint that only one car can cross at a time, and simultaneous crossing by cars from opposite sides is not allowed. The number of cars on each side is taken as input from the user, and a maximum of 5 cars can be on the bridge simultaneously. The program utilizes semaphores for synchronization.

### Code Structure:

1. **`left` Function:**
   - Represents cars on the left side of the river.
   - Each left car thread calls the `passing` function to cross the bridge, indicating its direction.

2. **`right` Function:**
   - Represents cars on the right side of the river.
   - Each right car thread calls the `passing` function to cross the bridge, indicating its direction.

3. **`passing` Function:**
   - Manages the passing of a car, ensuring that only one car is on the bridge at a time.
   - Utilizes semaphores for synchronization.
   - Prints appropriate information to the console, indicating the thread and its originating side.

### Writeup:

#### Code Logic:
1. Each car thread, whether from the left or right side, calls the `passing` function to cross the bridge.
2. The `passing` function uses semaphores to control access to the bridge, allowing a maximum of 5 cars on the bridge simultaneously.
3. If a car from one side is on the bridge, other cars from the same side can join, but cars from the opposite side must wait.
4. Once a car completes crossing the bridge, it releases the semaphore, allowing waiting cars to proceed.

#### Avoiding Concurrency Bugs:
1. Semaphores are used to ensure mutual exclusion, preventing multiple cars from accessing the bridge simultaneously.
2. The program uses appropriate locking and unlocking of semaphores to avoid race conditions.
3. A counting semaphore is employed to limit the number of cars on the bridge to 5, preventing congestion.

### Estimated Concurrency Bugs:
The program aims to eliminate concurrency bugs through the use of semaphores. The design ensures that only one car can be on the bridge at any given time, and the semaphores manage the synchronization, preventing data corruption or unexpected behavior.
