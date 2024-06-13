import os
import smtplib
import sqlite3
import tkinter as tk
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from tkinter import ttk
# Загрузка шрифта
from tkinter.font import Font

import matplotlib.pyplot as plt
from PIL import Image, ImageTk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from tkcalendar import DateEntry


class CoalStabApp(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title("CoalStab")
        self.state('zoomed')  # Развертывание на весь экран
        self.configure(bg='white')

        self.notification_count = tk.IntVar(value=0)

        self.buttons_frame = None
        self.content = None

        # Настройка шрифта
        self.font_path = "radiance-light italic.otf"
        self.custom_font = Font(family="Radiance Light Italic", size=18)
        self.option_add("*Font", self.custom_font)

        self.create_widgets()
        self.update_notifications_periodically()

        self.init_db()

        # Сразу отображать аналитику собранных данных при запуске
        self.show_dashboard()

    @staticmethod
    def init_db():
        try:
            conn = sqlite3.connect('CoalStab_Sensors.db')
            cursor = conn.cursor()
            cursor.execute(
                '''
                CREATE TABLE IF NOT EXISTS Sensors (
                    id INTEGER PRIMARY KEY,
                    name TEXT NOT NULL,
                    sensor_id TEXT NOT NULL,
                    date_added TEXT NOT NULL
                )
                '''
            )
            conn.commit()
            conn.close()
        except sqlite3.Error as e:
            print(f"Ошибка базы данных: {e}")

    def update_notifications_periodically(self):
        self.update_notifications()
        self.after(6000, self.update_notifications_periodically)

    def update_notifications(self):
        notifications = self.fetch_notifications()
        self.notification_count.set(len(notifications))

    def create_widgets(self):
        # Настройка стилей
        style = ttk.Style()
        style.configure('TButton', font=self.custom_font)
        style.configure('TCheckbutton', font=self.custom_font)
        style.configure('TLabel', font=self.custom_font)
        style.configure('TEntry', font=self.custom_font)

        # Боковая панель
        sidebar = tk.Frame(self, bg="#f0f0f0", width=250)
        sidebar.pack(side="left", fill="y")

        # Логотип и заголовок на боковой панели
        logo_frame = tk.Frame(sidebar, bg="#f0f0f0")
        logo_frame.pack(pady=20)

        try:
            logo = Image.open('logo.png')
            logo = logo.resize((60, 50), Image.Resampling.LANCZOS)
            logo_image = ImageTk.PhotoImage(logo)
            logo_label = ttk.Label(logo_frame, image=logo_image, background="#f0f0f0")
            logo_label.image = logo_image
            logo_label.pack(side="left", padx=5)
        except Exception as e:
            print(f"Не удалось загрузить изображение: {e}")

        title_label = ttk.Label(
            logo_frame, text="CoalStab", background="#f0f0f0"
        )
        title_label.pack(side="left")

        # Кнопки на боковой панели
        self.buttons_frame = tk.Frame(sidebar, bg="#f0f0f0")
        self.buttons_frame.pack(fill="x")

        buttons = [
            ("Список датчиков в системе", self.show_sensor_list),
            ("Аналитика собранных данных", self.show_dashboard),
            ("Обратная связь", self.show_feedback_form),
            ("Логи", self.show_settings)
        ]

        for btn_text, command in buttons:
            btn = tk.Button(
                self.buttons_frame, text=btn_text, anchor="w", bg="#f0f0f0", bd=0,
                padx=20, pady=10, command=command
            )
            btn.pack(fill="x")

        # Информация о пользователе внизу боковой панели
        user_info_frame = tk.Frame(sidebar, bg="#f0f0f0", pady=20)
        user_info_frame.pack(side="bottom", fill="x")

        user_label = tk.Label(
            user_info_frame, text="CoalStab\ncoalstab@mail.ru", bg="#f0f0f0"
        )
        user_label.pack()

        # Основная область
        self.content = tk.Frame(self, bg="white")
        self.content.pack(side="right", expand=True, fill="both")

    def show_sensor_list(self):
        self.clear_content()
        sensor_list_frame = tk.Frame(self.content, bg="white")
        sensor_list_frame.pack(pady=20, padx=20, anchor="w")

        ttk.Label(
            sensor_list_frame, text="Добавить новый датчик", background="white"
        ).pack(anchor="w", pady=10)
        sensor_form_frame = tk.Frame(sensor_list_frame, bg="white")
        sensor_form_frame.pack(anchor="w")

        ttk.Label(
            sensor_form_frame, text="Название датчика:", background="white"
        ).grid(row=0, column=0, pady=5, sticky="w")
        sensor_name = ttk.Entry(sensor_form_frame)
        sensor_name.grid(row=0, column=1, pady=5)

        ttk.Label(
            sensor_form_frame, text="Индикационный номер:", background="white"
        ).grid(row=1, column=0, pady=5, sticky="w")
        sensor_id = ttk.Entry(sensor_form_frame)
        sensor_id.grid(row=1, column=1, pady=5)

        ttk.Label(
            sensor_form_frame, text="Дата подключения:", background="white"
        ).grid(row=2, column=0, pady=5, sticky="w")
        sensor_date = DateEntry(
            sensor_form_frame, width=18, background='darkblue', foreground='white',
            borderwidth=2
        )
        sensor_date.grid(row=2, column=1, pady=5)

        message_label = ttk.Label(sensor_list_frame, text="", background="white")
        message_label.pack(anchor="w", pady=10)

        def hide_message():
            message_label.config(text="")

        def add_sensor():
            name = sensor_name.get()
            id_number = sensor_id.get()
            date = sensor_date.get()
            if not name or not id_number or not date:
                message_label.config(text="Не удалось добавить датчик в систему", foreground="red")
                self.after(3000, hide_message)
            else:
                try:
                    conn = sqlite3.connect('CoalStab_Sensors.db')
                    cursor = conn.cursor()
                    cursor.execute(
                        "INSERT INTO Sensors (name, sensor_id, date_added) VALUES (?, ?, ?)",
                        (name, id_number, date)
                    )
                    conn.commit()
                    conn.close()

                    message_label.config(text="Датчик добавлен", foreground="green")
                    sensor_name.delete(0, tk.END)
                    sensor_id.delete(0, tk.END)
                    sensor_date.set_date("")

                    self.display_sensors(sensor_display_frame)
                    self.after(3000, hide_message)
                except sqlite3.Error as e:
                    message_label.config(text=f"Ошибка базы данных: {e}", foreground="red")
                    self.after(3000, hide_message)

        ttk.Button(
            sensor_form_frame, text="Добавить", command=add_sensor
        ).grid(row=3, columnspan=2, pady=10)

        ttk.Label(
            sensor_list_frame, text="Список датчиков", background="white"
        ).pack(anchor="w", pady=20)

        sensor_display_frame = tk.Frame(sensor_list_frame, bg="white")
        sensor_display_frame.pack(anchor="w")

        self.display_sensors(sensor_display_frame)

    @staticmethod
    def display_sensors(frame):
        for widget in frame.winfo_children():
            widget.destroy()

        try:
            conn = sqlite3.connect('CoalStab_Sensors.db')
            cursor = conn.cursor()
            cursor.execute("SELECT name, sensor_id, date_added FROM Sensors")
            sensors = cursor.fetchall()
            conn.close()

            for idx, (name, sensor_id, date_added) in enumerate(sensors):
                sensor_info = (f"{idx + 1}. Название датчика: {name}, ID: {sensor_id}, "
                               f"Дата подключения: {date_added}")
                ttk.Label(
                    frame, text=sensor_info, background="white"
                ).pack(anchor="w")
        except sqlite3.Error as e:
            ttk.Label(
                frame, text=f"Ошибка базы данных: {e}", foreground="red",
                background="white"
            ).pack(anchor="w")

    def show_dashboard(self):
        self.clear_content()
        ttk.Label(
            self.content, text="Графики", background="white"
        ).pack(anchor="w", pady=10)

        current_data_frame = tk.Frame(self.content, bg="white")
        current_data_frame.pack(anchor="w", padx=20, pady=10)

        # Добавление графиков
        fig1, ax1 = plt.subplots(figsize=(5, 3))
        canvas1 = FigureCanvasTkAgg(fig1, master=self.content)
        canvas1.get_tk_widget().pack(side="top", fill="both", expand=True)
        lines1, = ax1.plot([], [], marker='o', linestyle='-')
        ax1.set_title('Dynamic Stability')
        ax1.set_xlabel('Index')
        ax1.set_ylabel('Value')
        ax1.legend(['Dynamic Stability'], loc='upper right')

        fig2, ax2 = plt.subplots(figsize=(5, 3))
        canvas2 = FigureCanvasTkAgg(fig2, master=self.content)
        canvas2.get_tk_widget().pack(side="top", fill="both", expand=True)
        bars2 = []

        ax2.set_title('Safety Factor')
        ax2.set_xlabel('Index')
        ax2.set_ylabel('Value')

        fig3, ax3 = plt.subplots(figsize=(5, 3))
        canvas3 = FigureCanvasTkAgg(fig3, master=self.content)
        canvas3.get_tk_widget().pack(side="top", fill="both", expand=True)
        ax3.set_title('Slope Angle ')

        def get_data_from_db():
            try:
                conn = sqlite3.connect('CoalStab_Data.db')
                cursor = conn.cursor()
                cursor.execute("SELECT * FROM StabilityData")
                data = cursor.fetchall()
                conn.close()
                return data
            except sqlite3.Error as e:
                print(f"Ошибка базы данных: {e}")
                return []

        def update_data():
            nonlocal bars2
            data = get_data_from_db()
            if data:
                current_index = update_data.current_index

                if current_index >= len(data):
                    update_data.current_index = 0
                    current_index = 0

                # Данные для первого графика
                y1 = [data[i][2] for i in range(max(0, current_index - 9), current_index + 1)]
                x1 = list(range(max(0, current_index - 9), current_index + 1))

                lines1.set_data(x1, y1)
                ax1.set_xlim([min(x1) - 1, max(x1) + 1])
                ax1.set_ylim([min(y1) - 1, max(y1) + 1])

                # Данные для второго графика
                if bars2:
                    for bar in bars2:
                        bar.remove()
                y2 = [data[i][3] for i in range(max(0, current_index - 9), current_index + 1)]
                x2 = list(range(max(0, current_index - 9), current_index + 1))
                bars2 = ax2.bar(x2, y2)
                ax2.set_xlim([min(x2) - 1, max(x2) + 1])
                ax2.set_ylim([0, max(y2) + 1])

                # Данные для третьего графика
                y3 = [data[i][4] for i in range(current_index, min(current_index + 3, len(data)))]
                ax3.clear()
                labels = ['Угол наклона 1', 'Угол наклона 2', 'Угол наклона 3']  # Описания
                ax3.pie(y3, labels=labels, autopct='%1.1f%%')
                ax3.set_title('Slope Angle ')

                update_data.current_index += 1

                canvas1.draw()
                canvas2.draw()
                canvas3.draw()

            self.after(5000, update_data)

        update_data.current_index = 0
        update_data()

    def show_feedback_form(self):
        self.clear_content()
        feedback_frame = tk.Frame(self.content, bg="white")
        feedback_frame.pack(pady=20, padx=20, anchor="w")

        ttk.Label(
            feedback_frame, text="Обратная связь", background="white"
        ).pack(anchor="w", pady=10)
        feedback_form_frame = tk.Frame(feedback_frame, bg="white")
        feedback_form_frame.pack(anchor="w")

        ttk.Label(
            feedback_form_frame, text="Имя:", background="white"
        ).grid(row=0, column=0, pady=5, sticky="e")
        feedback_name = ttk.Entry(feedback_form_frame)
        feedback_name.grid(row=0, column=1, pady=5, sticky="w")

        ttk.Label(
            feedback_form_frame, text="Отдел:", background="white"
        ).grid(row=1, column=0, pady=5, sticky="e")
        feedback_department = ttk.Entry(feedback_form_frame)
        feedback_department.grid(row=1, column=1, pady=5, sticky="w")

        ttk.Label(
            feedback_form_frame, text="Почта:", background="white"
        ).grid(row=2, column=0, pady=5, sticky="e")
        feedback_email = ttk.Entry(feedback_form_frame)
        feedback_email.grid(row=2, column=1, pady=5, sticky="w")

        ttk.Label(
            feedback_form_frame, text="Сообщение:", background="white"
        ).grid(row=3, column=0, pady=5, sticky="ne")
        feedback_message = tk.Text(
            feedback_form_frame, width=40, height=10, bd=2, relief="solid"
        )
        feedback_message.grid(row=3, column=1, pady=5, sticky="w")

        feedback_agreement = tk.BooleanVar()
        tk.Checkbutton(
            feedback_form_frame, text="Я согласен на обработку персональных данных",
            variable=feedback_agreement, bg="white"
        ).grid(row=4, column=0, columnspan=2, pady=5, sticky="w")

        def send_feedback():
            if feedback_agreement.get():
                name = feedback_name.get()
                department = feedback_department.get()
                email = feedback_email.get()
                message = feedback_message.get("1.0", tk.END)

                try:
                    self.send_email(name, department, email, message)
                    success_label = ttk.Label(
                        feedback_frame, text="Сообщение успешно отправлено!", foreground="green",
                        background="white"
                    )
                    success_label.pack(anchor="w", pady=10)
                    feedback_name.delete(0, tk.END)
                    feedback_department.delete(0, tk.END)
                    feedback_email.delete(0, tk.END)
                    feedback_message.delete("1.0", tk.END)
                    feedback_agreement.set(False)
                    self.after(3000, success_label.destroy)
                except Exception as e:
                    error_label = ttk.Label(
                        feedback_frame, text=f"Ошибка отправки: {e}", foreground="red",
                        background="white"
                    )
                    error_label.pack(anchor="w", pady=10)
                    self.after(3000, error_label.destroy)
            else:
                error_label = ttk.Label(
                    feedback_frame, text="Необходимо согласие на обработку данных",
                    foreground="red", background="white"
                )
                error_label.pack(anchor="w", pady=10)
                self.after(3000, error_label.destroy)

        send_button = ttk.Button(
            feedback_frame, text="Отправить", command=send_feedback
        )
        send_button.pack(anchor="w", pady=10)

    @staticmethod
    def send_email(name, department, email, message):
        sender_email = "coalstab@mail.ru"
        receiver_email = "coalstab@mail.ru"
        password = "sW5gGJNBcbv12iHr4d6s"

        subject = f"Обратная связь от {name}"
        body = f"Имя: {name}\nОтдел: {department}\nПочта: {email}\n\nСообщение:\n{message}"

        msg = MIMEMultipart()
        msg['From'] = sender_email
        msg['To'] = receiver_email
        msg['Subject'] = subject
        msg.attach(MIMEText(body, 'plain'))

        server = smtplib.SMTP('smtp.mail.ru', 587)
        server.starttls()
        server.login(sender_email, password)
        text = msg.as_string()
        server.sendmail(sender_email, receiver_email, text)
        server.quit()

    def show_settings(self):
        self.clear_content()
        settings_frame = tk.Frame(self.content, bg="white")
        settings_frame.pack(pady=20, padx=20, anchor="w")

        ttk.Label(
            settings_frame, text="Логи", background="white"
        ).pack(anchor="w", pady=10)

        # Настройка 1: Возможность открыть файл CoalStab.txt
        ttk.Label(
            settings_frame, text="Открыть файл CoalStab.txt", background="white"
        ).pack(anchor="w", pady=5)

        def open_file():
            try:
                with open("CoalStab_Data.txt", "r", encoding="utf-8") as file:
                    content = file.read()
                    file_content_window = tk.Toplevel(self)
                    file_content_window.title("Содержимое файла CoalStab.txt")
                    file_content_text = tk.Text(
                        file_content_window, wrap="word"
                    )
                    file_content_text.insert("1.0", content)
                    file_content_text.config(state="disabled")
                    file_content_text.pack(expand=True, fill="both")
            except Exception as e:
                error_label = ttk.Label(
                    settings_frame, text=f"Ошибка открытия файла: {e}", foreground="red",
                    background="white"
                )
                error_label.pack(anchor="w", pady=10)
                self.after(3000, error_label.destroy)

        ttk.Button(
            settings_frame, text="Открыть файл", command=open_file
        ).pack(anchor="w", pady=5)

    def clear_content(self):
        for widget in self.content.winfo_children():
            widget.destroy()

    @staticmethod
    def fetch_notifications():
        # Заглушка для функции получения уведомлений, чтобы избежать ошибок
        return []


if __name__ == "__main__":
    os.environ['TK_SILENCE_DEPRECATION'] = '1'
    app = CoalStabApp()
    app.mainloop()
