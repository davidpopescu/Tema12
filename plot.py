import matplotlib.pyplot as plt

def read_times(file_path):
    with open(file_path, 'r') as file:
        return [float(line.strip()) for line in file]

def calculate_speedup(base_time, parallel_times):
    return [base_time / parallel_time for parallel_time in parallel_times]

def calculate_efficiency(speedup, num_threads):
    efficiency = [s / t for s, t in zip(speedup, num_threads)]
    max_efficiency = max(efficiency)
    return [e / max_efficiency for e in efficiency]

files = ["time_hybrid.txt", "time_mpi.txt", "time_openmp.txt", "time_pthreads.txt"]

times_data = [read_times(file) for file in files]

num_threads = [1, 2, 4, 8]

labels = ["Hybrid", "MPI", "OpenMP", "Pthreads"]


sequential_time = times_data[0][0] 
speedup_data = [calculate_speedup(sequential_time, times) for times in times_data]

efficiency_data = [calculate_efficiency(speedup, num_threads) for speedup in speedup_data]

plt.figure(figsize=(12, 6))

for i, times in enumerate(times_data):
    plt.plot(num_threads, times, label=f'{labels[i]} - Durată')

plt.xlabel('Număr de fire de execuție')
plt.ylabel('Timp (secunde)')
plt.title('Evoluția timpului, speed-up-ului și eficienței în funcție de numărul de fire de execuție')
plt.legend()
plt.show()

plt.figure(figsize=(12, 6))

for i, speedup in enumerate(speedup_data):
    plt.plot(num_threads, speedup, label=f'{labels[i]} - Speed-up')

plt.xlabel('Număr de fire de execuție')
plt.ylabel('Speed-up')
plt.title('Evoluția speed-up-ului și eficienței în funcție de numărul de fire de execuție')
plt.legend()
plt.show()

plt.figure(figsize=(12, 6))

for i, efficiency in enumerate(efficiency_data):
    plt.plot(num_threads, efficiency, label=f'{labels[i]} - Eficiență')

plt.xlabel('Număr de fire de execuție')
plt.ylabel('Eficiență')
plt.title('Evoluția eficienței în funcție de numărul de fire de execuție')
plt.legend()
plt.show()
