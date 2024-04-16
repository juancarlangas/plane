// ------------------------- HEADERS NECESARIOS ---------------------------------
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

std::int32_t main(int argc, char* argv[]) {
	/* Revisamos que se hayan ingresado 2 argumentos (programa y archivo.csv)
	 * En caso contrario, se manda mensaje de error y se termina el programa */
	if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_csv>\n";
        return EXIT_FAILURE;
    }

	//------------------------- ARREGLO FACTORIAL --------------------------------
	/* Definimos un struct que almacena cada combinación,
	 * Un valor bool (boleano) puede ser true (no cero) o false (cero) */
	struct Combinacion {
		std::string etiqueta;
		bool respuestas[3]; // True o false
	};

	/* Definimos el arreglo factorial y lo inicializamos. Podemos usar 1 y 0
	 * o las palabras -true- y -false- */
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
	/* Intentamos abrir el archivo.csv. En caso de no poderse abrir,
	 * mandamos mensaje de error y salimos del programa */
    std::ifstream archivo_csv(argv[1]);
    if (!archivo_csv.is_open()) {
        std::cerr << "No se puede abrir el archivo .csv\n";
        return EXIT_FAILURE;
    }

	// Definimos un struct que almacena las respuestas de cada entrevistado
	struct Entrevistado {
		bool respuestas[3];
		float valor;
	};

	// Definimos un vector (tabla dinámica) con todos los entrevistados
	std::vector<Entrevistado> base_de_datos;
	
	/* Definimos variables temporales con las que vamos a leer la información.*/
	std::string linea_csv;
	Entrevistado entrevistado;
	std::string respuesta;

	/* Leemos línea por línea hasta llegar al fin del documento .csv
	 * Si tienen valores erróneos o vacíos saltamos a la siguiente línea.
	 * De lo contrario, añadiremos el renglón a la base de datos */
	while (std::getline(archivo_csv, linea_csv)) {
		std::stringstream stream(linea_csv);

		// Pregunta 1
		std::getline(stream, respuesta, ',');
		if (respuesta.empty() || (respuesta != "Si" && respuesta != "No"))
			continue;
		entrevistado.respuestas[0] = (respuesta == "Si");

		// Pregunta 2
		std::getline(stream, respuesta, ',');
		if (respuesta.empty() || (respuesta != "Si" && respuesta != "No"))
			continue;
		entrevistado.respuestas[1] = (respuesta == "Si");

		// Pregunta 3
		std::getline(stream, respuesta, ',');
		if (respuesta.empty() || (respuesta != "Si" && respuesta != "No"))
			continue;
		entrevistado.respuestas[2] = (respuesta == "Si");

		// Valor numérico
		std::getline(stream, respuesta);
		if (respuesta.empty()) continue; // Valor vacío? SIG LINEA
		entrevistado.valor = std::stof(respuesta);

		base_de_datos.push_back(entrevistado);
	}
	archivo_csv.close();

	//---------------------------- ANÁLISIS --------------------------------------
	// Definimos un struct para cada renglón de la tabla final
	struct Coincidencia {
		std::string etiqueta;
		bool respuestas[3];
		float valor;
	};

	// Definimos la tabla_final que es un vector de todas las coincidencias
	std::vector<Coincidencia> tabla_final;

	bool se_encontro;
	do {
		se_encontro = false;

		// Ciclamos por cada elemento del arreglo factorial
		for (int i = 0; i < 8; ++i) {
			se_encontro = false;

			// Comenzamos desde arriba de la base de datos
			auto entrevistado = base_de_datos.begin();
			while (entrevistado != base_de_datos.end()) {

				// Si las respuestas son las mismas
				if (std::equal(std::begin(arreglo_factorial[i].respuestas),
							std::end(arreglo_factorial[i].respuestas),
							std::begin(entrevistado->respuestas))) {

					se_encontro = true;

					// Copiamos todo el renglón a una variable temporal
					Coincidencia coincidencia {
						arreglo_factorial[i].etiqueta,
						{
							entrevistado->respuestas[0],
							entrevistado->respuestas[1],
							entrevistado->respuestas[2]
						},
						entrevistado->valor
					};

					// Lo agregamos a la tabla final
					tabla_final.push_back(coincidencia);

					// Eliminamos el renglón de la base de datos
					entrevistado = base_de_datos.erase(entrevistado);

					break;
				}

				// Si no coincidió, apuntaremos al sig rengón
				else {
					++entrevistado;
				}
			}

			// Si recorrió toda la tabla y aún no encontró nada, salimos
			if (not(se_encontro)) {
				break;
			}
		}

	} while (se_encontro); // Mientras haya encontrado


	//---------------------------- EXPORTACIÓN -----------------------------------
	// Creamos los archivos de salida
	std::ofstream archivo_analisis_completo("analisis-completo.csv");
	std::ofstream archivo_analisis_resultados("analisis-resultados.csv");

	// Intentamos abrirlos. De lo contrario, mandamos mensaje de error y salimos
	if (!archivo_analisis_completo.is_open()) {
		std::cerr << "Error al abrir tabla_final.csv\n";
		return EXIT_FAILURE;
	}

	if (!archivo_analisis_resultados.is_open()) {
		std::cerr << "Error al abrir tabla_final.csv\n";
		return EXIT_FAILURE;
	}

	// Escribimos la información
	for (const auto& coincidencia : tabla_final) { // Cada linea del cuestionario
		archivo_analisis_completo
			<< coincidencia.etiqueta << ", "
			<< (coincidencia.respuestas[0] ? "Sí" : "No") << ", "
			<< (coincidencia.respuestas[1] ? "Sí" : "No") << ", "
			<< (coincidencia.respuestas[2] ? "Sí" : "No") << ", "
			<< coincidencia.valor << "\n";

		archivo_analisis_resultados
			<< coincidencia.valor << '\n';
	}

	// Cerramos low archivo
	archivo_analisis_completo.close();
	archivo_analisis_resultados.close();

	return EXIT_SUCCESS; // Salimos del programa con éxito GAD
}
