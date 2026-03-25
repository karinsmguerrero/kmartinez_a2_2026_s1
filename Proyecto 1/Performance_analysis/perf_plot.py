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
    files = glob.glob("output_smt_*.txt")

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

    # -------- Gráfica 1: CPU cycles --------
    plt.figure()
    plt.plot(threads, cycles, marker='o')
    plt.xlabel("Número de hilos")
    plt.ylabel("CPU Cycles")
    plt.title("CPU Cycles vs Número de Hilos")
    plt.grid()

    # -------- Gráfica 2: Cache misses --------
    plt.figure()
    plt.plot(threads, cache_misses, marker='o')
    plt.xlabel("Número de hilos")
    plt.ylabel("Cache Misses")
    plt.title("Cache Misses vs Número de Hilos")
    plt.grid()

    # -------- Gráfica 3: Tiempo --------
    plt.figure()
    plt.plot(threads, time_elapsed, marker='o')
    plt.xlabel("Número de hilos")
    plt.ylabel("Tiempo (segundos)")
    plt.title("Tiempo vs Número de Hilos")
    plt.grid()

    plt.show()


if __name__ == "__main__":
    main()