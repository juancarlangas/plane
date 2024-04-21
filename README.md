# ANALIZER
Programa que analiza una base de datos y la coteja con un arreglo factorial para encontrar coincidencias y mostrarlas en un archivo de texto.

## Instalación
1. Instala (si aún no lo tienes) la aplicación `Powershell` desde Windows Store.
2. Descarga `analizer-amd64.exe` desde la carpeta [Releases](https://github.com/juancarlangas/plane/releases/) y guárdalo en la misma carpeta donde se encuentra la base de datos a analizar.
3. Abre y prepara tu base de datos en Excel o LibreOffice Calc.
    a. Elimina la primer fila (títulos de columnas) si es que contiene.
    b. Asegúrate que todas las palabras `Si` estén **sin acento**. Puedes usar la herramienta **Sustituir o reemplazar** para cambiar todas las instancias de `Sí` por `Si`.
    c. Guarda tu base de datos en formato CSV **dentro de la misma carpeta** donde se encuentra el archivo `analizer-amd64.exe`.
4. Abre el Explorador de Archivos y navega hasta la carpeta donde se encuentra tu base de datos y el archivo `analizer-amd64.exe`. Da click derecho y selecciona `Abrir con Powershell` o `Abrir con Terminal`.
5. En la terminal, escribe el siguiente comando y presiona `Enter`:
```Powershell
analizer-amd64.exe <nombre_de_tu_base_de_datos.csv>
```
6. Espera a que el programa termine de analizar tu base de datos. Al finalizar, encontrarás dos archivos en la misma carpeta donde se encuentra tu base de datos:
    - `analisis-completo.txt`: Incluye la etiqueta del arreglo factorial y las columnas de respuestas.
    - `analisis-resultados.txt`: Incluye solo el valor numérico.
