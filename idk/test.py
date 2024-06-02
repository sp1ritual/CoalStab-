import sqlite3
import matplotlib.pyplot as plt

def fetch_stability_factors():
    # Подключение к базе данных
    conn = sqlite3.connect('CoalStab_Data.db')
    cursor = conn.cursor()

    # Запрос к базе данных для извлечения данных столбца Stability Factor
    query = "SELECT StabilityFactor FROM StabilityData"
    cursor.execute(query)
    stability_factors = [row[0] for row in cursor.fetchall()]

    # Закрытие подключения к базе данных
    cursor.close()
    conn.close()
    
    return stability_factors

def plot_stability_factors(stability_factors):
    # Построение графика
    plt.figure(figsize=(10, 5))
    plt.plot(stability_factors, marker='o', linestyle='-', color='b')
    plt.title('Stability Factor Over Time')
    plt.xlabel('Entry Number')
    plt.ylabel('Stability Factor')
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    stability_factors = fetch_stability_factors()
    plot_stability_factors(stability_factors)
