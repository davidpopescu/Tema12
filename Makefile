CC = g++
CXXFLAGS = -std=c++11 -fopenmp
MPICC = mpic++
MPIFLAGS = -std=c++11
PTHREADSFLAGS = -pthread
SRC_OPENMP = merge_openmp.cpp
SRC_THREADS = merge_pthreads.cpp
SRC_MPI = merge_mpi.cpp
SRC_PTHREADS = merge_pthreads.cpp
EXEC_OPENMP = merge_openmp.exe
EXEC_PTHREADS = merge_pthreads.exe
EXEC_MPI = merge_mpi.exe
NUM_PROC = 2

all: $(EXEC_OPENMP) $(EXEC_THREADS) $(EXEC_MPI) $(EXEC_PTHREADS)

$(EXEC_OPENMP): $(SRC_OPENMP)
	$(CC) $(CXXFLAGS) -o $@ $^

$(EXEC_THREADS): $(SRC_THREADS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(PTHREADSFLAGS)

$(EXEC_MPI): $(SRC_MPI)
	$(MPICC) $(MPIFLAGS) -o $@ $^

$(EXEC_PTHREADS): $(SRC_PTHREADS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(PTHREADSFLAGS)

run_openmp: 
	./$(EXEC_OPENMP)

run_threads: 
	./$(EXEC_THREADS)

run_mpi: 
	mpirun -np $(NUM_PROC) ./$(EXEC_MPI)

run_pthreads: 
	./$(EXEC_PTHREADS)

clean:
	rm -f *.o $(EXEC_OPENMP) $(EXEC_THREADS) $(EXEC_MPI) $(EXEC_PTHREADS)
