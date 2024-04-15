/* Programa que lee un archivo .csv, lo almacena en un tabla (arreglo de struct)
 * y lo coteja con un arreglo factoria definido internamente.
 * Exporta los resultados en otra tabla .csv
 *
 * Se invoca de la siguiente forma: analizer <ruta/al/archivo.csv>
 * */

#include <cstdint> // std:int32_t
#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <vector> // std::vector
#include <fstream> // std::cerr
#include <iostream>
#include <sstream> // std::stringstream

std::int32_t main(int argc, char* argv[]) {
	if (argc != 2) { // Si el número de argumentos invocados es diferente a 2
        std::cerr << "Uso: " << argv[0] << " <archivo_csv>\n"; // Mensaje de error
        return EXIT_FAILURE; // Salimos del programa.
    }

	//------------------------- ARREGLO FACTORIAL --------------------------------
	/* Definimos un struct que va a almacenar cada posible combinación */
	struct Combinacion {
		std::string etiqueta;	// (1), a, ab, etc...
		bool respuestas[3];		// 3 valores true o false
	};

	/* ARREGLO FACTORIAL: Un arreglo de 8 elementos de combinaciones (tabla) */
	Combinacion arreglo_factorial[8] {
		{"(1)",	{0, 0, 0}},
		{"a",	{1, 0, 0}},
		{"b",	{0, 1, 0}},
		{"ab",	{1, 1, 0}},
		{"c",	{0, 0, 1}},
		{"ac",	{1, 0, 1}},
		{"bc",	{0, 1, 1}},
		{"abc", {1, 1, 1}}
	};

	//---------------------------- BASE DE DATOS --------------------------------
    std::ifstream archivo_csv(argv[1]); // Abrimos el .csv para leerlo
    if (!archivo_csv.is_open()) { // Si no puede abrirse
        std::cerr << "No se puede abrir el archivo .csv\n"; // Mensaje de error
        return EXIT_FAILURE; // Salimos del programa
    }

	// Definimos un struct que va a almacenar cada cuestionario individual.
	struct Cuestionario {
		bool respuestas[3]; // (true si encuentra "Si", false si encuentra "No"
		float valor;
	};

	// BASE DE DATOS: Una tabla (std::vector) con todos los cuestionarios.
	// Los vectores son arreglos dinámicos, pueden crecer o decrecer
	std::vector<Cuestionario> base_de_datos;
	
	std::string linea_leida; // Línea CSV
	Cuestionario cuestionario_leido; // Línea CSV convertida al struct Cuestionario
	std::string respuesta_leida; // Respuesta individual

	while (std::getline(archivo_csv, linea_leida)) { // Leemos la linea completa
		std::stringstream stream(linea_leida); // La convertimos a stream

		std::getline(stream, respuesta_leida, ','); // Extraemos respuesta 1
		if (respuesta_leida.empty() || // Vacía o diferente a Sí / No? SIG LINEA
				(respuesta_leida != "Si" && respuesta_leida != "No")) continue;
		// Asignamos el resultado de comparar la respuesta con "Si" (true o false)
		cuestionario_leido.respuestas[0] = (respuesta_leida == "Si");

		std::getline(stream, respuesta_leida, ','); // Extraemos respuesta 2
		if (respuesta_leida.empty() || // Vacía o diferente a Sí / No? SIG LINEA
				(respuesta_leida != "Si" && respuesta_leida != "No")) continue;
		// Asignamos el resultado de comparar la respuesta con "Si" (true o false)
		cuestionario_leido.respuestas[1] = (respuesta_leida == "Si");

		std::getline(stream, respuesta_leida, ','); // Extraemos respuesta 3
		if (respuesta_leida.empty() || // Vacía o diferente a Sí / No? SIG LINEA
				(respuesta_leida != "Si" && respuesta_leida != "No")) continue;
		// Asignamos el resultado de comparar la respuesta con "Si" (true o false)
		cuestionario_leido.respuestas[2] = respuesta_leida == "Si";

		std::getline(stream, respuesta_leida); // Extraemos valor
		if (respuesta_leida.empty()) continue; // Valor vacío? SIG LINEA
		// Asignamos diréctamente el valor leído
		cuestionario_leido.valor = std::stof(respuesta_leida);

		// Asignamos la línea completa acabamos de leer a la base de datos
		base_de_datos.push_back(cuestionario_leido);
	}
	archivo_csv.close(); // Cerramos el archivo

	//---------------------------- ANÁLISIS --------------------------------------
	// Definimos un struct para las líneas que coincidan.
	struct Coincidencia {
		std::string etiqueta; // Etiqueta del arreglo factorial
		bool respuestas[3]; // Respuesta (true/false)
		float valor; // Valor numérico
	};

	// TABLA FINAL: Un vector del struct anterior para formar la table_final
	std::vector<Coincidencia> tabla_final;

	// LOOP
	bool match; // Cada vuelta se reinicia a -false- esperando salir como -true-
	do {
		match = false;
		// Ciclamos por el arreglo factorial
		for (int i = 0; i < 8; ++i) {
			match = false;
			// Comienza a analizar desde arriba
			auto renglon_analizado = base_de_datos.begin();
			// Mientras no llegue al final
			while (renglon_analizado != base_de_datos.end()) {
				// Si hay match
				if (std::equal(std::begin(arreglo_factorial[i].respuestas),
							std::end(arreglo_factorial[i].respuestas),
							std::begin(renglon_analizado->respuestas))) {
					// Definimos una variable para almacenar todo el renglón
					Coincidencia coincidencia_encontrada;
					// Le copiamos la etiqueta
					coincidencia_encontrada.etiqueta =
						arreglo_factorial[i].etiqueta;
					// Le copiamos las respuestas
					std::copy(std::begin(renglon_analizado->respuestas),
							std::end(renglon_analizado->respuestas),
							std::begin(coincidencia_encontrada.respuestas));
					// Le copiamos el valor numérico
					coincidencia_encontrada.valor = renglon_analizado->valor;

					// Agregamos la coincidencia a la tabla final
					tabla_final.push_back(coincidencia_encontrada);

					// Eliminamos el renglón de la base de datos
					renglon_analizado = base_de_datos.erase(renglon_analizado);

					// Marcamos que hemos encontrado una coincidencia
					match = true;

					// Salimos del bucle interno
					break;
				// Si no encontró coincidencia apuntaremos al siguiente renglón
				} else {
					++renglon_analizado;
				}
			}
			if (!match) { // Si terminó de analizar y no hubo match, salimos
				break;
			}
		}
	} while (match);

	//---------------------------- EXPORTACIÓN -----------------------------------
	// Creamos un objeto de archivo de salida
	std::ofstream archivo_salida("tabla_final.csv");

	if (!archivo_salida.is_open()) { // Abrimos el archivo. Si no se puede...
		std::cerr << "Error al abrir tabla_final.csv\n"; // Mensaje de error
		return EXIT_FAILURE; // Salimos del programa
	}

	for (const auto& cuestionario : tabla_final) { // Cada linea del cuestionario
		archivo_salida
			<< cuestionario.etiqueta << ", "
			<< (cuestionario.respuestas[0] ? "Sí" : "No") << ", "
			<< (cuestionario.respuestas[1] ? "Sí" : "No") << ", "
			<< (cuestionario.respuestas[2] ? "Sí" : "No") << ", "
			<< cuestionario.valor << "\n";
	}

	// Cerramos el archivo
	archivo_salida.close();

	return EXIT_SUCCESS; // Salimos del programa con éxito GAD
}
