import re
import glob
import matplotlib.pyplot as plt


def parse_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # -------- Threads --------
    thread_match = re.search(r"test_smt (\d+)", content)
    if thread_match:
        threads = int(thread_match.group(1))
    else:
        threads = int(re.search(r'(\d+)', filepath).group(1))

    # -------- Separar bloques --------
    blocks = content.split("------------------------")

    cycles_list = []
    cache_list = []
    time_list = []

    for block in blocks:
        # CPU cycles
        cycles_match = re.search(r"([\d\.]+)\s+cpu-cycles", block)
        if cycles_match:
            cycles = float(cycles_match.group(1).replace('.', ''))
            cycles_list.append(cycles)

        # Cache misses
        cache_match = re.search(r"([\d\.]+)\s+cache-misses", block)
        if cache_match:
            cache = float(cache_match.group(1).replace('.', ''))
            cache_list.append(cache)

        # Tiempo
        time_match = re.search(r"([\d\,\.]+)\s+seconds time elapsed", block)
        if time_match:
            time_val = float(time_match.group(1).replace(',', '.'))
            time_list.append(time_val)

    if not cycles_list or not cache_list or not time_list:
        raise ValueError("No se pudieron extraer métricas")

    # -------- Promedios --------
    avg_cycles = sum(cycles_list) / len(cycles_list)
    avg_cache = sum(cache_list) / len(cache_list)
    avg_time = sum(time_list) / len(time_list)

    print(f"{filepath} -> runs: {len(cycles_list)}")

    return threads, avg_cycles, avg_cache, avg_time


def main():
    files = glob.glob("/home/karina/Documentos/TEC/Arqui 2/Proyectos/Proyecto 1/Performance_analysis/output_smt_*.txt")

    if not files:
        print("No se encontraron archivos")
        return

    data = []

    for file in files:
        try:
            parsed = parse_file(file)
            print(f"  -> {parsed}")
            data.append(parsed)
        except Exception as e:
            print(f"Error en {file}: {e}")

    if not data:
        print("No se pudo extraer ningún dato")
        return

    # Ordenar por threads
    data.sort(key=lambda x: x[0])

    threads = [d[0] for d in data]
    cycles = [d[1] for d in data]
    cache_misses = [d[2] for d in data]
    time_elapsed = [d[3] for d in data]
    
    # -------- Speedup --------
    if 1 not in threads:
        raise ValueError("Necesitas datos con 1 hilo para calcular speedup")

    t1 = time_elapsed[threads.index(1)]
    speedup = [t1 / t for t in time_elapsed]
    
    # -------- Efficiency --------
    efficiency = [s / t for s, t in zip(speedup, threads)]

    # -------- Crear grid 2x3 --------
    fig, axs = plt.subplots(2, 3, figsize=(14, 8))


    # -------- CPU cycles --------
    axs[0][0].plot(threads, cycles, marker='o')
    axs[0][0].set_title("CPU Cycles")
    axs[0][0].set_xlabel("Threads")
    axs[0][0].set_ylabel("Cycles")
    axs[0][0].grid()


    # -------- Cache misses --------
    axs[0][1].plot(threads, cache_misses, marker='o')
    axs[0][1].set_title("Cache Misses")
    axs[0][1].set_xlabel("Threads")
    axs[0][1].set_ylabel("Misses")
    axs[0][1].grid()


    # -------- Tiempo --------
    axs[0][2].plot(threads, time_elapsed, marker='o')
    axs[0][2].set_title("Tiempo")
    axs[0][2].set_xlabel("Threads")
    axs[0][2].set_ylabel("Segundos")
    axs[0][2].grid()


    # -------- Speedup --------
    axs[1][0].plot(threads, speedup, marker='o')
    #axs[1][0].plot(threads, threads, linestyle='--', label="Ideal")
    axs[1][0].set_title("Speedup")
    axs[1][0].set_xlabel("Threads")
    axs[1][0].set_ylabel("Speedup")
    #axs[1][0].legend()
    axs[1][0].grid()


    # -------- Efficiency --------
    axs[1][1].plot(threads, efficiency, marker='o')
    #axs[1][1].axhline(y=1, linestyle='--')
    axs[1][1].set_title("Efficiency")
    axs[1][1].set_xlabel("Threads")
    axs[1][1].set_ylabel("Efficiency")
    axs[1][1].grid()


    # -------- Espacio vacío --------
    axs[1][2].axis('off')


    # Ajuste automático
    plt.tight_layout()

    plt.show()



if __name__ == "__main__":
    main()