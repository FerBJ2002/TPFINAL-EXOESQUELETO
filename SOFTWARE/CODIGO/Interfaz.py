import customtkinter as ctk
import serial
import time

# Crear una variable global para la conexión serial
esp = None
modo_seleccionado = ""  # Variable para almacenar el modo seleccionado

# Función para verificar la conexión serial
def verificar_conexion():
    """Verifica si la ESP32 está conectada y actualiza el estado en la interfaz."""
    global esp, status_label

    try:
        if esp is None or not esp.is_open:
            esp = serial.Serial("COM6", 9600, timeout=1)  # Intentar abrir la conexión serial
            time.sleep(1)  # Esperar a que la conexión se estabilice

        esp_leido = esp.readline().decode("utf-8").rstrip()
        
        if esp_leido:
            status_label.configure(text="Conectado", text_color="green")  # Actualizar estado a conectado
            mostrar_menu()  # Mostrar el menú con los botones
        else:
            status_label.configure(text="No conectado", text_color="red")  # No se recibió ningún dato
        
    except Exception as e:
        status_label.configure(text="No conectado", text_color="red")  # Fallo en la conexión

# Función para cerrar la comunicación serial
def cerrar_conexion():
    """Cerrar la conexión serial al cerrar la ventana."""
    global esp
    if esp is not None and esp.is_open:
        esp.close()  # Cerrar la conexión serial
        print("Conexión serial cerrada.")
    root.quit()  # Cierra la ventana

# Función para enviar el comando a la ESP32
def enviar_comando(comando):
    """Envía un comando a la ESP32 y muestra el modo seleccionado."""
    global modo_seleccionado
    if esp and esp.is_open:
        esp.write(comando.encode())  # Enviar el comando por serial
        print(f"Comando enviado: {comando}")
        mostrar_menu_modo()  # Mostrar el menú del modo seleccionado
    else:
        print("Error: No se ha conectado con la ESP32")

# Función para mostrar el menú principal con los modos
def mostrar_menu():
    """Reemplaza el contenido de la ventana por los botones del menú."""
    # Limpiar la ventana antes de mostrar el menú
    for widget in root.winfo_children():
        widget.destroy()

    # Título del menú con el mismo fondo que el cuadro de bienvenida
    menu_label = ctk.CTkLabel(
        root,
        text="Equipo conectado, Seleccione un modo: ",
        font=("Helvetica", 20),
        text_color="black",
        corner_radius=10,
        fg_color="#13FDBE"  # Color de fondo igual al del cuadro de bienvenida
    )
    menu_label.grid(row=0, column=0, columnspan=2, pady=(20, 10))

    # Botones del menú en dos filas y dos columnas (eliminamos el "Modo de Espera")
    button2 = ctk.CTkButton(root, text="Intensidad 1", font=("Helvetica", 16), command=lambda: seleccionar_modo("Intensidad1"), fg_color="#13FDBE", text_color="black", width=120, height=50)
    button2.place(x=80, y=120)

    button3 = ctk.CTkButton(root, text="Intensidad 2", font=("Helvetica", 16), command=lambda: seleccionar_modo("Intensidad2"), fg_color="#13FDBE", text_color="black", width=120, height=50)
    button3.place(x=320, y=120)

    button4 = ctk.CTkButton(root, text="Intensidad 3", font=("Helvetica", 16), command=lambda: seleccionar_modo("Intensidad3"), fg_color="#13FDBE", text_color="black", width=120, height=50)
    button4.place(x=80, y=240)

    button5 = ctk.CTkButton(root, text="Modo EMG", font=("Helvetica", 16), command=lambda: seleccionar_modo("EMG"), fg_color="#13FDBE", text_color="black", width=120, height=50)
    button5.place(x=320, y=240)

    # Hacer que las celdas del grid se ajusten al contenido
    root.grid_columnconfigure(0, weight=1)
    root.grid_columnconfigure(1, weight=1)
    root.grid_rowconfigure(1, weight=1)
    root.grid_rowconfigure(2, weight=1)

# Función para seleccionar un modo y actualizar la interfaz
def seleccionar_modo(modo):
    """Selecciona el modo y muestra el menú con las opciones de 'Iniciar', 'Detener'."""
    global modo_seleccionado
    modo_seleccionado = modo  # Guardar el modo seleccionado
    enviar_comando(modo)  # Enviar el comando para activar el modo
    mostrar_menu_modo()  # Mostrar el menú con los botones de 'Iniciar' y 'Detener'

# Función para mostrar el menú del modo seleccionado (con los botones Anterior, Iniciar, Detener)
def mostrar_menu_modo():
    """Muestra el menú con el modo seleccionado y tres botones de acción."""
    # Limpiar la ventana antes de mostrar el menú del modo
    for widget in root.winfo_children():
        widget.destroy()

    # Título con el modo seleccionado
    modo_label = ctk.CTkLabel(
        root,
        text=f"Modo: {modo_seleccionado}",
        font=("Helvetica", 20),
        text_color="black",
        corner_radius=10,
        fg_color="#13FDBE"
    )
    modo_label.grid(row=0, column=0, columnspan=2, pady=(20, 10))

    # Botones para las acciones del modo
    button_anterior = ctk.CTkButton(root, text="Anterior", font=("Helvetica", 16), command=mostrar_menu, fg_color="#13FDBE", text_color="black", width=80, height=40)
    button_anterior.place(x=110, y=300)  # Ubicado en la parte inferior izquierda

    button_iniciar = ctk.CTkButton(root, text="Iniciar", font=("Helvetica", 16), command=lambda: enviar_comando(f"Iniciar {modo_seleccionado}"), fg_color="#13FDBE", text_color="black", width=80, height=40)
    button_iniciar.place(x=210, y=300)  # Ubicado en la parte inferior central

    button_detener = ctk.CTkButton(root, text="Detener", font=("Helvetica", 16), command=lambda: enviar_comando("Espera"), fg_color="#13FDBE", text_color="black", width=80, height=40)
    button_detener.place(x=310, y=300)  # Ubicado en la parte inferior derecha

    # Ajustar la disposición de la ventana
    root.grid_columnconfigure(0, weight=1)
    root.grid_columnconfigure(1, weight=1)
    root.grid_rowconfigure(1, weight=1)
    root.grid_rowconfigure(2, weight=1)

# Crear la ventana principal
root = ctk.CTk()
root.geometry("500x400")
root.configure(fg_color="white")  # Fondo blanco

# Configurar la acción de cerrar la ventana para cerrar la conexión serial
root.protocol("WM_DELETE_WINDOW", cerrar_conexion)

# Texto superior (cuadro de bienvenida)
welcome_label = ctk.CTkLabel(
    root,
    text="Bienvenido al asistente de uso",
    fg_color="#13FDBE",  # Fondo del cuadro
    text_color="black",  # Color del texto
    font=("Helvetica", 26, "bold"),  # Tamaño de la fuente
    corner_radius=10,  # Bordes redondeados del cuadro
    height=80  # Cambia el tamaño en Y aquí
)
welcome_label.pack(padx=20, pady=(20, 10), fill="x")  # Cambia el tamaño en X con padx

# Texto de estado de la conexión
status_label = ctk.CTkLabel(
    root,
    text="No conectado",  # Estado inicial
    text_color="red",  # Color inicial
    font=("Helvetica", 20),
)
status_label.pack(pady=(40, 10))  # Ajusta el espacio arriba y abajo del texto

# Texto inferior
label = ctk.CTkLabel(
    root,
    text="Conecte el equipo a la computadora.",
    font=("Helvetica", 20),
    text_color="black"
)
label.pack(pady=(10, 10))  # Ajusta el espacio alrededor del texto

# Botón redondeado para refrescar la conexión
refresh_button = ctk.CTkButton(
    root,
    text="Refrescar",
    corner_radius=15,  # Control del redondeo de los bordes
    command=verificar_conexion,
    width=120,
    height=50,
    fg_color="#13FDBE",
    text_color="black",
    font=("Helvetica", 16)
)
refresh_button.place(x=300, y=300)  # Colocando el botón en la parte inferior derecha

# Llamada a la función de verificación de la conexión al iniciar
verificar_conexion()

# Ejecutar la ventana principal
root.mainloop()