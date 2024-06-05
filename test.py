import tkinter as tk
from tkinter import ttk, font
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import sqlite3
from PIL import Image, ImageTk

def get_data_from_db():
    conn = sqlite3.connect('CoalStab_Data.db')
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM StabilityData")
    data = cursor.fetchall()
    conn.close()
    return data

root = tk.Tk()
root.title("CoalStab Data Dashboard")

style = ttk.Style()
style.configure("White.TFrame", background='white')

logo = Image.open('logo.png')
logo = logo.resize((60, 50), Image.LANCZOS)
logo_image = ImageTk.PhotoImage(logo)
logo_label = ttk.Label(root, image=logo_image)
logo_label.image = logo_image
logo_label.grid(row=0, column=0, sticky=tk.W)

title_label = ttk.Label(root, text="CoalStab", font=('Helvetica', 18, 'bold'))
title_label.grid(row=0, column=1, sticky=tk.W)

text_data_frame = ttk.Frame(root, padding="3 3 12 12", style='White.TFrame')
text_data_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S))

labels = ["Dynamic Stability Factor", "Safety Factor", "Slope Angle", "Stability", "Tension", "Stability Factor", "Slope Safety"]
label_widgets = []
for i, label in enumerate(labels):
    lbl = ttk.Label(text_data_frame, text=f"{label}: ", background='white')
    lbl.grid(row=i, column=0, sticky=tk.W)
    label_widgets.append(lbl)

fig, ax = plt.subplots(figsize=(4.5, 1.7))
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.draw()
canvas.get_tk_widget().grid(row=2, column=1, padx=10, pady=10)

lines, = ax.plot([], [], marker='o', linestyle='-')

def update_data():
    data = get_data_from_db()
    for i, lbl in enumerate(label_widgets):
        lbl.config(text=f"{labels[i]}: {data[0][i+2]}")
    y = [row[2] for row in data]
    x = np.arange(len(y))
    lines.set_data(x, y)
    ax.set_xlim([min(x) - 1, max(x) + 1])
    ax.set_ylim([min(y) - 1, max(y) + 1])
    canvas.draw()
    root.after(2000, update_data)

update_data()

root.mainloop()
