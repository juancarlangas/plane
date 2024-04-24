// ------------------------- HEADERS NECESARIOS ---------------------------------
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

std::int32_t main(int argc, char* argv[]) {

	/* Revisamos que se hayan ingresado solo 2 argumentos:
	 * 		<nombre_del_programa> <archivo_csv>
	 * En caso de no ser así, mandamos mensaje de error y salimos del programa */

	if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_csv>\n";
        return EXIT_FAILURE;
    }

	//------------------------- ARREGLO FACTORIAL --------------------------------
	/* Vamos a crear la tabla del ARREGLO FACTORIAL. Primero definimos un tipo
	 * de struct que almacena la etiqueta y las respuestas de cada combinación.
	 * Luego lo usamos para construir nuestra tabla */

	struct Combinacion {
		std::string etiqueta; // (1), a, b, ab, c, ac, bc, abc, etc
		bool respuestas[3]; // True o false
	};

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
	/* Creamos un objeto de tipo ifstream para leer el archivo .csv */
    std::ifstream archivo_csv(argv[1]);

	/* Si no se pudo abrir el archivo, mandamos mensaje de error y salimos del programa */
    if (!archivo_csv.is_open()) {
        std::cerr << "No se puede abrir el archivo .csv\n";
        return EXIT_FAILURE;
    }

	/* Vamos a crear la tabla para nuestra base de datos. Definimos un struct que
	 * almacena las respuestas y el valor numérico de cada entrevistado. Luego lo
	 * usamos para construir nuestra tabla. */

	struct Entrevistado {
		bool respuestas[3];
		float valor;
	};

	std::vector<Entrevistado> base_de_datos;
	
	/* Definimos variables temporales con las que vamos a leer la información.
	 * Al final de cada vuelta, si fue un renglón válido, usamos estas variables
	 * para agregar la línea completa a la base de datos. */

	std::string linea_csv;
	Entrevistado entrevistado;
	std::string respuesta;

	/* Leemos línea por línea hasta llegar al fin del documento .csv */
	while (std::getline(archivo_csv, linea_csv)) {
		std::stringstream stream(linea_csv);

		/* Pregunta 1: Leemos hasta la primer coma. En caso de que la respuesta
		 * sea vacía o no sea "Si" o "No", saltamos el resto del ciclo y continuamos */
		std::getline(stream, respuesta, ',');
		if (respuesta.empty() || (respuesta != "Si" && respuesta != "No"))
			continue;
		entrevistado.respuestas[0] = (respuesta == "Si");

		/* Pregunta 2: Leemos hasta la segunda coma. En caso de que la respuesta
		 * sea vacía o no sea "Si" o "No", saltamos el resto del ciclo y continuamos */
		std::getline(stream, respuesta, ',');
		if (respuesta.empty() || (respuesta != "Si" && respuesta != "No"))
			continue;
		entrevistado.respuestas[1] = (respuesta == "Si");

		/* Pregunta 3: Leemos hasta la tercer coma. En caso de que la respuesta
		 * sea vacía o no sea "Si" o "No", saltamos el resto del ciclo y continuamos */
		std::getline(stream, respuesta, ',');
		if (respuesta.empty() || (respuesta != "Si" && respuesta != "No"))
			continue;
		entrevistado.respuestas[2] = (respuesta == "Si");

		// Valor numérico : Leemos hasta el final de la línea csv. En caso de que
		// la respuesta sea vacía, saltamos el resto del ciclo y continuamos
		std::getline(stream, respuesta);
		if (respuesta.empty()) continue; // Valor vacío? SIG LINEA
		entrevistado.valor = std::stof(respuesta);

		/* Si llegamos hasta aquí, es porque todas las respuestas son válidas.
		 * Por lo tanto, agregamos el entrevistado a la base de datos */
		base_de_datos.push_back(entrevistado);
	}
	archivo_csv.close(); // Cerramos el archivo de lectura una vez terminado


	//---------------------------- ANÁLISIS --------------------------------------
	/* Ahora que ya tenemos llena nuestra base de datos, vamos a hacer las comparaciones
	 * utilizando una tabla nueva que será una mezcla del arreglo factorial y la base de datos.
	 * Ahí vamos a meter las coincidencias que encontremos. */

	// Definimos un struct para cada renglón de dicha tabla
	struct RenglonAnalisis {
		std::string etiqueta; // (1), a, b, ab, c, ac, bc, abc, etc
		bool respuestas[3]; // True o false
		float valor; // Valor numérico
	};

	std::vector<RenglonAnalisis> analisis;

	// Creamos una variable tipo <true/false> que comienza cada búsqueda en false
	// y espera convertirse en true al final de cada ciclo. Si no lo logra, salimos
	// del ciclo y terminamos el programa.

	bool se_encontro;
	do {
		se_encontro = false;

		// Ciclamos por cada elemento del arreglo factorial
		for (int i = 0; i < 8; ++i) {
			se_encontro = false;

			// Comenzamos desde arriba de la base de datos
			auto renglon = base_de_datos.begin();
			while (renglon != base_de_datos.end()) {

				// Si encontró coincidencia
				if (std::equal(std::begin(arreglo_factorial[i].respuestas),
							std::end(arreglo_factorial[i].respuestas),
							std::begin(renglon->respuestas))) {

					se_encontro = true; // Ya chingamos

					// Creamos un renglon final temporal y lo llenamos
					// con la informacion del arreglo factorial y la base de datos
					RenglonAnalisis renglon_analisis = {
						arreglo_factorial[i].etiqueta,
						{
							renglon->respuestas[0],
							renglon->respuestas[1],
							renglon->respuestas[2]
						},
						renglon->valor
					};

					// Y lo anañadimos a la tabla del análisis final
					analisis.push_back(renglon_analisis);

					// Eliminamos el renglón de la base de datos original
					renglon = base_de_datos.erase(renglon);

					break;
				}

				// Si no coincidió, apuntaremos al sig rengón de la base de datos
				else {
					++renglon;
				}
			}

			// Si recorrió toda la base de datos y aún no encontró nada, salimos
			if (not(se_encontro)) {
				break;
			}
		}

	} while (se_encontro); // Mientras haya encontrado coincidencia, continua


	//---------------------------- EXPORTACIÓN -----------------------------------
	// Creamos dos archivos de salida
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

	// Escribimos la información:
	// - Para analisis-completo.csv: etiqueta, respuestas, valor
	// - Para analisis-resultados.csv: valor

	for (const auto& renglon_analisis : analisis) { // Cada linea del cuestionario
		archivo_analisis_completo
			<< renglon_analisis.etiqueta << ", "
			<< (renglon_analisis.respuestas[0] ? "Sí" : "No") << ", "
			<< (renglon_analisis.respuestas[1] ? "Sí" : "No") << ", "
			<< (renglon_analisis.respuestas[2] ? "Sí" : "No") << ", "
			<< renglon_analisis.valor << "\n";

		archivo_analisis_resultados
			<< renglon_analisis.valor << '\n';
	}

	// Cerramos ambos archivos
	archivo_analisis_completo.close();
	archivo_analisis_resultados.close();

	return EXIT_SUCCESS; // Salimos del programa con éxito GAD
}
