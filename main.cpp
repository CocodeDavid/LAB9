#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <limits>
#include "Graph.h" 
using namespace std;

struct Transaccion {    //simplmente guarda los datos por defecto
    int id;
    string producto;    
    int cantidad;
    double precio;
};

struct Parejas {  //lleva la cuenta de la combinacion de productos
    string p1;
    string p2;
    int peso;
};

void coOcurrencia(vector<Parejas>& lista_pares, string prod1, string prod2) { //busca una pareja, si la encunetra le suma al peso y sino crea la pareja con peso 1
    if (prod1 > prod2) {
        swap(prod1, prod2); //
    }

    for (int i = 0; i < lista_pares.size(); i++) {  //busca y suma al peso
        if (lista_pares[i].p1 == prod1 && lista_pares[i].p2 == prod2) {
            lista_pares[i].peso++; 
            return;                
        }
    }

    Parejas nuevo_par;   //crea pareja y lo mete al vector
    nuevo_par.p1 = prod1;
    nuevo_par.p2 = prod2;
    nuevo_par.peso = 1; 
    lista_pares.push_back(nuevo_par);
}

// Obtener productos únicos
vector<string> obtenerProductosUnicos(Graph<string>& grafo) {
    vector<string> productos;
    for (int i = 0; i < grafo.vertexList.size(); i++) {
        productos.push_back(grafo.vertexList.get(i)->data);
    }
    sort(productos.begin(), productos.end());
    return productos;
}

int main() {
   
    vector<Transaccion> datos;
    
    ifstream archivo("transacciones.txt");
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo transacciones.txt" << endl;
        return 1;
    }
    
    string linea;
    int linea_numero = 0;
    while (getline(archivo, linea)) {
        linea_numero++;
        if (linea.empty()) continue;
        
        int id;
        string producto;
        int cantidad;
        double precio;
        
        // Parsear la línea: id,producto,cantidad,precio
        size_t primera_coma = linea.find(',');
        size_t segunda_coma = (primera_coma == string::npos) ? string::npos : linea.find(',', primera_coma + 1);
        size_t tercera_coma = (segunda_coma == string::npos) ? string::npos : linea.find(',', segunda_coma + 1);

        if (primera_coma == string::npos || segunda_coma == string::npos || tercera_coma == string::npos) {
            cerr << "Linea inválida (formato incorrecto) en la linea " << linea_numero << endl;
            continue;
        }

        string id_str = linea.substr(0, primera_coma);
        producto = linea.substr(primera_coma + 1, segunda_coma - primera_coma - 1);
        string cantidad_str = linea.substr(segunda_coma + 1, tercera_coma - segunda_coma - 1);
        string precio_str = linea.substr(tercera_coma + 1);

        if (id_str.empty() || producto.empty() || cantidad_str.empty() || precio_str.empty()) {
            cerr << "Linea inválida (campos vacíos) en la linea " << linea_numero << endl;
            continue;
        }

        try {
            id = stoi(id_str);
            cantidad = stoi(cantidad_str);
            precio = stod(precio_str);
        } catch (const exception& e) {
            cerr << "Linea inválida en la linea " << linea_numero << ": " << e.what() << endl;
            continue;
        }

        datos.push_back({id, producto, cantidad, precio});
    }
    archivo.close();

    if (datos.empty()) {
        cerr << "No se encontraron transacciones válidas en el archivo." << endl;
        return 1;
    }

    vector<vector<string>> compras_por_transaccion; 
    vector<string> transaccion_actual;  //es como la bolsa
    
    int id_actual = datos[0].id;

    for (int i = 0; i < datos.size(); i++) {  //agrupa los productos segun el id
        if (datos[i].id == id_actual) {
            transaccion_actual.push_back(datos[i].producto);
        } else {
            compras_por_transaccion.push_back(transaccion_actual);  //guarda la bolsa en el vector bidimensional
            transaccion_actual.clear();
            id_actual = datos[i].id;
            transaccion_actual.push_back(datos[i].producto); //agrega el producto que nos hizo saltar al else
        }
    }
    if (!transaccion_actual.empty()) {			//mete la ultima bolsa si quedo algo en el "carrito"
        compras_por_transaccion.push_back(transaccion_actual);
    }

    vector<Parejas> lista_coOcurrente;  //el vector que lleva la cuenta de las combinaciones

    for (int i = 0; i < compras_por_transaccion.size(); i++) {
        vector<string> productos = compras_por_transaccion[i];
        for (size_t j = 0; j < productos.size(); j++) {
            for (size_t k = j + 1; k < productos.size(); k++) { //usamos j+1 pq sino un producto se hace amigo de si mismo
                coOcurrencia(lista_coOcurrente, productos[j], productos[k]);
            }
        }
    }


    Graph<string> grafo_compras;
    
    for (int i = 0; i < lista_coOcurrente.size(); i++) {
        string p1 = lista_coOcurrente[i].p1;
        string p2 = lista_coOcurrente[i].p2;
        int peso = lista_coOcurrente[i].peso;

        grafo_compras.addEdge(p1, p2, peso);  //se hace el grafo no dirigido
        grafo_compras.addEdge(p2, p1, peso);
    }

    // Menú principal interactivo
    int opcion = 0;
    while (opcion != 5) {
        cout << "\n========================================" << endl;
        cout << "   ANÁLISIS DE RELACIONES DE PRODUCTOS" << endl;
        cout << "========================================" << endl;
        cout << "1. Mostrar productos disponibles" << endl;
        cout << "2. Mostrar grafo de relaciones" << endl;
        cout << "3. Mostrar productos con mayor frecuencia" << endl;
        cout << "4. Mostrar productos relacionados a un producto" << endl;
        cout << "5. Salir" << endl;
        cout << "========================================" << endl;
        cout << "Seleccione una opción: ";
        if (!(cin >> opcion)) {
            cout << "\nEntrada inválida.\n" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1: {
                cout << "\n________________________________________" << endl;
                cout << "      PRODUCTOS DISPONIBLES" << endl;
                cout << "________________________________________" << endl;
                
                vector<string> productos = obtenerProductosUnicos(grafo_compras);
                cout << "\nTotal de productos: " << productos.size() << "\n" << endl;
                
                for (size_t i = 0; i < productos.size(); i++) {
                    cout << (i + 1) << ". " << productos[i] << endl;
                }
                cout << "\n________________________________________\n" << endl;
                break;
            }
            case 2: {
                cout << "\n________________________________________" << endl;
                cout << "    GRAFO DE RELACIONES DE PRODUCTOS" << endl;
                cout << "________________________________________\n" << endl;
                
                for (int i = 0; i < grafo_compras.vertexList.size(); i++) {
                    Vertex<string>* nodo = grafo_compras.vertexList.get(i);
                    cout << "Producto: " << nodo->data << endl;
                    cout << "  Grado (conexiones): " << nodo->outDegree << endl;
                    cout << "  Se compra frecuentemente con:" << endl;
                    
                    for (int j = 0; j < nodo->connectedTo.size(); j++) {
                        Edge<string>* arista = nodo->connectedTo.get(j);
                        cout << "    -> " << arista->to->data << " (" << arista->weight << " veces)" << endl;
                    }
                    cout << endl;
                }
                cout << "________________________________________\n" << endl;
                break;
            }
            case 3: {
                cout << "\n________________________________________" << endl;
                cout << "  PRODUCTOS CON MAYOR FRECUENCIA" << endl;
                cout << "________________________________________\n" << endl;
                
                vector<pair<string, int>> productos_grado;
                for (int i = 0; i < grafo_compras.vertexList.size(); i++) {
                    Vertex<string>* nodo = grafo_compras.vertexList.get(i);
                    productos_grado.push_back({nodo->data, nodo->outDegree});
                }
                
                sort(productos_grado.begin(), productos_grado.end(),
                     [](const pair<string, int>& a, const pair<string, int>& b) {
                         return a.second > b.second;
                     });
                
                cout << "Productos ordenados por frecuencia de compra:\n" << endl;
                for (size_t i = 0; i < productos_grado.size(); i++) {
                    cout << (i + 1) << ". " << productos_grado[i].first 
                         << " - Grado: " << productos_grado[i].second << endl;
                }
                cout << "\n________________________________________\n" << endl;
                break;
            }
            case 4: {
                cout << "\n________________________________________" << endl;
                cout << "PRODUCTOS RELACIONADOS A UN PRODUCTO" << endl;
                cout << "________________________________________\n" << endl;
                
                vector<string> productos = obtenerProductosUnicos(grafo_compras);

                if (productos.empty()) {
                    cout << "No hay productos disponibles." << endl;
                    cout << "\n________________________________________\n" << endl;
                    break;
                }
                
                cout << "Seleccione un producto:\n" << endl;
                for (size_t i = 0; i < productos.size(); i++) {
                    cout << (i + 1) << ". " << productos[i] << endl;
                }
                
                cout << "\nIngrese el número del producto: ";
                int opcionProducto;
                if (!(cin >> opcionProducto)) {
                    cout << "Entrada inválida." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (opcionProducto < 1 || opcionProducto > (int)productos.size()) {
                    cout << "Opción inválida." << endl;
                } else {
                    string producto_seleccionado = productos[opcionProducto - 1];
                    Vertex<string>* nodo = grafo_compras.getVertex(producto_seleccionado);
                    
                    if (nodo) {
                        cout << "\n________________________________________" << endl;
                        cout << "Productos relacionados con: " << producto_seleccionado << endl;
                        cout << "________________________________________\n" << endl;
                        
                        vector<pair<string, int>> relacionados;
                        for (int i = 0; i < nodo->connectedTo.size(); i++) {
                            Edge<string>* arista = nodo->connectedTo.get(i);
                            relacionados.push_back({arista->to->data, arista->weight});
                        }
                        
                        sort(relacionados.begin(), relacionados.end(),
                             [](const pair<string, int>& a, const pair<string, int>& b) {
                                 return a.second > b.second;
                             });
                        
                        if (relacionados.empty()) {
                            cout << "No hay productos relacionados." << endl;
                        } else {
                            cout << "Productos que se compran frecuentemente con " << producto_seleccionado << ":\n" << endl;
                            for (size_t i = 0; i < relacionados.size(); i++) {
                                cout << (i + 1) << ". " << relacionados[i].first 
                                     << " (" << relacionados[i].second << " veces)" << endl;
                            }
                        }
                    }
                    cout << "\n________________________________________\n" << endl;
                }
                break;
            }
            case 5:
                cout << "\n¡Hasta luego!\n" << endl;
                break;
            default:
                cout << "\nOpción inválida.\n" << endl;
        }
    }

    return 0;
}