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

    # -------- CPU cycles --------
    cycles_match = re.search(r"([\d\.]+)\s+cpu-cycles", content)
    if cycles_match:
        cycles = float(cycles_match.group(1).replace('.', ''))
    else:
        raise ValueError("cpu-cycles no encontrado")

    # -------- Cache misses --------
    cache_match = re.search(r"([\d\.]+)\s+cache-misses", content)
    if cache_match:
        cache_misses = float(cache_match.group(1).replace('.', ''))
    else:
        raise ValueError("cache-misses no encontrado")

    # -------- Tiempo --------
    time_match = re.search(r"([\d\,\.]+)\s+seconds time elapsed", content)
    if time_match:
        time_str = time_match.group(1).replace(',', '.')
        time_elapsed = float(time_str)
    else:
        raise ValueError("time elapsed no encontrado")

    return threads, cycles, cache_misses, time_elapsed


def main():
    files = glob.glob("/home/karina/Documentos/TEC/Arqui 2/Proyectos/Proyecto 1/Performance_analysis/output_smt_*.txt")

    if not files:
        print("No se encontraron archivos")
        return

    data = []

    for file in files:
        print(f"Procesando {file}")
        try:
            parsed = parse_file(file)
            print(f"  -> {parsed}")
            data.append(parsed)
        except Exception as e:
            print(f"Error en {file}: {e}")

    if not data:
        print("No se pudo extraer ningún dato")
        return

    # Ordenar por número de hilos
    data.sort(key=lambda x: x[0])

    threads = [d[0] for d in data]
    cycles = [d[1] for d in data]
    cache_misses = [d[2] for d in data]
    time_elapsed = [d[3] for d in data]

    # Crear figura con 3 subplots (3 filas, 1 columna)
    fig, axs = plt.subplots(3, 1, figsize=(8, 12))

    # -------- CPU cycles --------
    axs[0].plot(threads, cycles, marker='o')
    axs[0].set_title("CPU Cycles vs Número de Hilos")
    axs[0].set_xlabel("Número de hilos")
    axs[0].set_ylabel("CPU Cycles")
    axs[0].grid()

    # -------- Cache misses --------
    axs[1].plot(threads, cache_misses, marker='o')
    axs[1].set_title("Cache Misses vs Número de Hilos")
    axs[1].set_xlabel("Número de hilos")
    axs[1].set_ylabel("Cache Misses")
    axs[1].grid()

    # -------- Tiempo --------
    axs[2].plot(threads, time_elapsed, marker='o')
    axs[2].set_title("Tiempo vs Número de Hilos")
    axs[2].set_xlabel("Número de hilos")
    axs[2].set_ylabel("Tiempo (segundos)")
    axs[2].grid()

    # Ajustar espacios automáticamente
    plt.tight_layout()

    plt.show()


if __name__ == "__main__":
    main()