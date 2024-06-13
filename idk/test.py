import os
os.environ['TK_SILENCE_DEPRECATION'] = '1'
import tkinter as tk
from tkinter import ttk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import sqlite3
from PIL import Image, ImageTk
import requests

def get_data_from_db():
    try:
        conn = sqlite3.connect('CoalStab_Data.db')
        cursor = conn.cursor()
        cursor.execute("SELECT * FROM StabilityData")
        data = cursor.fetchall()
        conn.close()
        return data
    except sqlite3.Error as e:
        print(f"Database error: {e}")
        return []

def get_weather_data():
    api_key = "3843c62f930c1dec6121067562c726aa"  # Replace with your OpenWeatherMap API key
    city = "Kemerovo"
    url = f"http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=metric"

    try:
        response = requests.get(url)
        weather_data = response.json()
        temp = weather_data['main']['temp']
        pressure = weather_data['main']['pressure']
        humidity = weather_data['main']['humidity']
        return temp, pressure, humidity
    except KeyError as e:
        return None, None, None
    except Exception as e:
        return None, None, None

def main():
    root = tk.Tk()
    root.title("CoalStab Data Dashboard")

    # Configure grid layout
    root.columnconfigure(0, weight=1)
    root.columnconfigure(1, weight=1)
    root.rowconfigure(1, weight=1)
    root.rowconfigure(2, weight=1)

    # Load logo image
    try:
        logo = Image.open('logo.png')
        logo = logo.resize((60, 50), Image.LANCZOS)
        logo_image = ImageTk.PhotoImage(logo)
    except Exception as e:
        print(f"Failed to load image: {e}")
        return

    # Create logo and title labels
    logo_label = ttk.Label(root, image=logo_image)
    logo_label.image = logo_image  # keep a reference!
    logo_label.grid(row=0, column=0, sticky=tk.W, padx=5, pady=5)

    title_label = ttk.Label(root, text="CoalStab", font=('Helvetica', 18, 'bold'))
    title_label.grid(row=0, column=1, sticky=tk.W, padx=5, pady=5)

    # Create frame for data labels
    text_data_frame = ttk.Frame(root, padding="3 3 12 12")
    text_data_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S))
    text_data_frame.columnconfigure(0, weight=1)

    labels = ["Dynamic Stability Factor", "Safety Factor", "Slope Angle", "Stability", "Tension", "Stability Factor", "Slope Safety", "Temperature", "Pressure", "Humidity"]
    label_widgets = []
    for i, label in enumerate(labels):
        lbl = ttk.Label(text_data_frame, text=f"{label}: ")
        lbl.grid(row=i, column=0, sticky=tk.W, pady=2)
        label_widgets.append(lbl)

    # Create matplotlib figure
    fig, ax = plt.subplots(figsize=(6, 4))
    canvas = FigureCanvasTkAgg(fig, master=root)
    canvas.draw()
    canvas.get_tk_widget().grid(row=2, column=0, columnspan=2, padx=10, pady=10, sticky=(tk.W, tk.E, tk.N, tk.S))

    lines, = ax.plot([], [], marker='o', linestyle='-')

    # Fetch and display data
    data = get_data_from_db()
    temp, pressure, humidity = get_weather_data()
    if data:
        for i, lbl in enumerate(label_widgets[:-3]):
            lbl.config(text=f"{labels[i]}: {data[0][i+2]}")
        if temp is not None and pressure is not None and humidity is not None:
            label_widgets[-3].config(text=f"{labels[-3]}: {temp} °C")
            label_widgets[-2].config(text=f"{labels[-2]}: {pressure} hPa")
            label_widgets[-1].config(text=f"{labels[-1]}: {humidity} %")

        y = [row[2] for row in data]
        x = np.arange(len(y))
        lines.set_data(x, y)
        ax.set_xlim([min(x) - 1, max(x) + 1])
        ax.set_ylim([min(y) - 1, max(y) + 1])
        canvas.draw()

    root.mainloop()

if __name__ == "__main__":
    main()
