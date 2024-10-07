
#include <iostream>
#include <sqlite3.h>

using namespace std;

// Función para abrir la base de datos
sqlite3* openDatabase(const char* filename) {
    sqlite3* db;
    int exit = sqlite3_open(filename, &db);

    if (exit != SQLITE_OK) {
        cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    cout << "Conexion exitosa a la base de datos!" << endl;
    return db;
}

// Función para ejecutar una consulta SQL (select)
void executeQuery(sqlite3* db, const char* sql) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Resultados de la consulta:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int age = sqlite3_column_int(stmt, 2);
        cout << "ID: " << id << ", Nombre: " << name << ", Edad: " << age << endl;
    }
    sqlite3_finalize(stmt);
}

// Función para modificar valores (update)
void modifyValue(sqlite3* db, const char* sql) {
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error al modificar datos: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "Valor modificado correctamente!" << endl;
    }
}

int main() {
    // Abrir la base de datos
    sqlite3* db = openDatabase("test.db");

    if (db == nullptr) {
        return -1;
    }

    // Crear tabla si no existe
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS personas (ID INTEGER PRIMARY KEY AUTOINCREMENT, NOMBRE TEXT, EDAD INTEGER);";
    modifyValue(db, createTableSQL);

    // Insertar un valor en la tabla
    const char* insertSQL = "INSERT INTO personas (NOMBRE, EDAD) VALUES ('Joan', 25);";
    modifyValue(db, insertSQL);

    // Mostrar los valores actuales de la tabla
    const char* selectSQL = "SELECT * FROM personas;";
    executeQuery(db, selectSQL);

    // Modificar el valor de un registro
    const char* updateSQL = "UPDATE personas SET EDAD = 30 WHERE NOMBRE = 'Joan';";
    modifyValue(db, updateSQL);

    // Mostrar nuevamente los valores para verificar la modificación
    executeQuery(db, selectSQL);

    // Cerrar la base de datos
    sqlite3_close(db);

    return 0;
}